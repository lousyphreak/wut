#include <mutex>
#include "devoptab_fsa.h"

// Materialize a POSIX write gap only when the native positioned write rejects
// a cursor beyond EOF. Ordinary reads, writes and seeks need no extra FS calls.
static FSError
__wut_fsa_fill_write_gap(FSAClientHandle client, FSAFileHandle fd, uint32_t offset)
{
   FSAStat stat;
   FSError status = FSAGetStatFile(client, fd, &stat);
   if (status < 0) {
      return status;
   }
   if (offset <= stat.size) {
      return FS_ERROR_OUT_OF_RANGE;
   }

   const uint32_t chunkSize = MIN(offset - stat.size, 0x4000);
   void *zeroes             = memalign(0x40, (chunkSize + 0x3f) & ~0x3f);
   if (!zeroes) {
      return FS_ERROR_OUT_OF_RESOURCES;
   }
   memset(zeroes, 0, chunkSize);

   uint32_t position = stat.size;
   while (position < offset) {
      const uint32_t size = MIN(offset - position, chunkSize);
      status              = FSAWriteFileWithPos(client, zeroes, 1, size, position, fd, 0);
      if (status < 0 || uint32_t(status) != size) {
         free(zeroes);
         return status < 0 ? status : FS_ERROR_STORAGE_FULL;
      }
      position += status;
   }
   free(zeroes);
   return FS_ERROR_OK;
}

ssize_t
__wut_fsa_write(struct _reent *r, void *fd, const char *ptr, size_t len)
{
   FSError status;
   __wut_fsa_file_t *file;
   __wut_fsa_device_t *deviceData;

   if (!fd || !ptr) {
      r->_errno = EINVAL;
      return -1;
   }

   // Check that the file was opened with write access
   file = (__wut_fsa_file_t *)fd;
   if ((file->flags & O_ACCMODE) == O_RDONLY) {
      r->_errno = EBADF;
      return -1;
   }

   // cache-aligned, cache-line-sized
   __attribute__((aligned(0x40))) uint8_t alignedBuffer[0x40];

   deviceData = (__wut_fsa_device_t *)r->deviceData;

   std::scoped_lock lock(file->mutex);

   // If O_APPEND is set, we always write to the end of the file.
   // When writing we file->offset to the file size to keep in sync.
   if (file->flags & O_APPEND) {
      file->offset = file->appendOffset;
   }

   size_t bytesWritten = 0;
   while (bytesWritten < len) {
      // only use input buffer if cache-aligned and write size is a multiple of cache line size
      // otherwise write from alignedBuffer
      uint8_t *tmp = (uint8_t *)ptr;
      size_t size  = len - bytesWritten;

      if (size < 0x40) {
         // write partial cache-line back-end
         tmp = alignedBuffer;
      } else if ((uintptr_t)ptr & 0x3F) {
         // write partial cache-line front-end
         tmp  = alignedBuffer;
         size = MIN(size, 0x40 - ((uintptr_t)ptr & 0x3F));
      } else {
         // write whole cache lines
         size &= ~0x3F;
      }

      // Limit each request to 256 KiB
      if (size > 0x40000) {
         size = 0x40000;
      }

      if (tmp == alignedBuffer) {
         memcpy(tmp, ptr, size);
      }

      if (file->flags & O_APPEND) {
         status = FSAWriteFile(deviceData->clientHandle, tmp, 1, size, file->fd, 0);
      } else {
         status = FSAWriteFileWithPos(deviceData->clientHandle, tmp, 1, size, file->offset, file->fd, 0);
         if (status == FS_ERROR_OUT_OF_RANGE) {
            status = __wut_fsa_fill_write_gap(deviceData->clientHandle, file->fd, file->offset);
            if (status >= 0) {
               status = FSAWriteFileWithPos(deviceData->clientHandle, tmp, 1, size, file->offset, file->fd, 0);
            }
         }
      }
      if (status < 0) {
         WUT_DEBUG_REPORT("FSAWriteFile(0x%08X, %p, 1, 0x%08X, 0x%08X, 0) (%s) failed: %s\n",
                          deviceData->clientHandle, tmp, size, file->fd, file->fullPath, FSAGetStatusStr(status));
         if (bytesWritten != 0) {
            return bytesWritten; // error after partial write
         }

         r->_errno = __wut_fsa_translate_error(status);
         return -1;
      }

      file->appendOffset += status;
      file->offset += status;
      bytesWritten += status;
      ptr += status;

      if ((size_t)status != size) {
         return bytesWritten; // partial write
      }
   }

   return bytesWritten;
}
