#include "wut_module.h"
#include <coreinit/debug.h>
#include <coreinit/dynload.h>

int
__wut_bind_imports_v1(const WUTModuleImport *imports, uint32_t count)
{
   for (uint32_t i = 0; i < count; ++i) {
      const WUTModuleImport *import = &imports[i];
      void *address;
      OSDynLoad_Error result = OSDynLoad_FindExport((OSDynLoad_Module)-1,
         import->kind == 0 ? OS_DYNLOAD_EXPORT_FUNC : OS_DYNLOAD_EXPORT_DATA,
         import->exportName, &address);
      if (result) {
         OSReport("WUT module: unresolved application import %s (%08x)\n",
                  import->exportName, result);
         return result;
      }
      if (import->kind == 2) {
         const uint32_t *descriptor = address;
         import->target[0] = descriptor[0];
         import->target[1] = descriptor[1] + import->addend;
      } else {
         *import->target = (uintptr_t)address + import->addend;
      }
   }
   return 0;
}
