#pragma once
#include <wut.h>
#include <gx2r/resource.h>
#include "enum.h"

/**
 * \defgroup gx2_surface Surface
 * \ingroup gx2
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GX2Surface GX2Surface;
typedef struct GX2DepthBuffer GX2DepthBuffer;
typedef struct GX2ColorBuffer GX2ColorBuffer;
typedef struct GX2Rect GX2Rect;
typedef struct GX2Point GX2Point;

struct GX2Surface
{
   GX2SurfaceDim dim;
   uint32_t width;
   uint32_t height;
   uint32_t depth;
   uint32_t mipLevels;
   GX2SurfaceFormat format;
   GX2AAMode aa;
   union
   {
      GX2SurfaceUse use;
      GX2RResourceFlags resourceFlags;
   };
   uint32_t imageSize;
   void *image;
   uint32_t mipmapSize;
   void *mipmaps;
   GX2TileMode tileMode;
   uint32_t swizzle;
   uint32_t alignment;
   uint32_t pitch;
   /**
    * Byte offsets calculated by GX2CalcSurfaceSizeAndAlignment.
    * Level 0 starts at image; level 1 starts at mipmaps; levels 2 and above
    * start at mipmaps + mipLevelOffset[level - 1].
    * Entry 0 only gives the padded spacing for placing mipmaps after image.
    */
   uint32_t mipLevelOffset[13];
};
WUT_CHECK_OFFSET(GX2Surface, 0x0, dim);
WUT_CHECK_OFFSET(GX2Surface, 0x4, width);
WUT_CHECK_OFFSET(GX2Surface, 0x8, height);
WUT_CHECK_OFFSET(GX2Surface, 0xc, depth);
WUT_CHECK_OFFSET(GX2Surface, 0x10, mipLevels);
WUT_CHECK_OFFSET(GX2Surface, 0x14, format);
WUT_CHECK_OFFSET(GX2Surface, 0x18, aa);
WUT_CHECK_OFFSET(GX2Surface, 0x1c, use);
WUT_CHECK_OFFSET(GX2Surface, 0x1c, resourceFlags);
WUT_CHECK_OFFSET(GX2Surface, 0x20, imageSize);
WUT_CHECK_OFFSET(GX2Surface, 0x24, image);
WUT_CHECK_OFFSET(GX2Surface, 0x28, mipmapSize);
WUT_CHECK_OFFSET(GX2Surface, 0x2c, mipmaps);
WUT_CHECK_OFFSET(GX2Surface, 0x30, tileMode);
WUT_CHECK_OFFSET(GX2Surface, 0x34, swizzle);
WUT_CHECK_OFFSET(GX2Surface, 0x38, alignment);
WUT_CHECK_OFFSET(GX2Surface, 0x3C, pitch);
WUT_CHECK_OFFSET(GX2Surface, 0x40, mipLevelOffset);
WUT_CHECK_SIZE(GX2Surface, 0x74);

struct GX2DepthBuffer
{
   GX2Surface surface;

   uint32_t viewMip;
   uint32_t viewFirstSlice;
   uint32_t viewNumSlices;
   void *hiZPtr;
   uint32_t hiZSize;
   float depthClear;
   uint32_t stencilClear;

   uint32_t regs[7];
};
WUT_CHECK_OFFSET(GX2DepthBuffer, 0x74, viewMip);
WUT_CHECK_OFFSET(GX2DepthBuffer, 0x78, viewFirstSlice);
WUT_CHECK_OFFSET(GX2DepthBuffer, 0x7C, viewNumSlices);
WUT_CHECK_OFFSET(GX2DepthBuffer, 0x80, hiZPtr);
WUT_CHECK_OFFSET(GX2DepthBuffer, 0x84, hiZSize);
WUT_CHECK_OFFSET(GX2DepthBuffer, 0x88, depthClear);
WUT_CHECK_OFFSET(GX2DepthBuffer, 0x8C, stencilClear);
WUT_CHECK_OFFSET(GX2DepthBuffer, 0x90, regs);
WUT_CHECK_SIZE(GX2DepthBuffer, 0xAC);

struct GX2ColorBuffer
{
   GX2Surface surface;

   uint32_t viewMip;
   uint32_t viewFirstSlice;
   uint32_t viewNumSlices;
   void *aaBuffer;
   uint32_t aaSize;

   uint32_t regs[5];
};
WUT_CHECK_OFFSET(GX2ColorBuffer, 0x74, viewMip);
WUT_CHECK_OFFSET(GX2ColorBuffer, 0x78, viewFirstSlice);
WUT_CHECK_OFFSET(GX2ColorBuffer, 0x7C, viewNumSlices);
WUT_CHECK_OFFSET(GX2ColorBuffer, 0x80, aaBuffer);
WUT_CHECK_OFFSET(GX2ColorBuffer, 0x84, aaSize);
WUT_CHECK_OFFSET(GX2ColorBuffer, 0x88, regs);
WUT_CHECK_SIZE(GX2ColorBuffer, 0x9C);

/** Pixel rectangle with inclusive left/top and exclusive right/bottom. */
struct GX2Rect
{
   int32_t left;
   int32_t top;
   int32_t right;
   int32_t bottom;
};
WUT_CHECK_OFFSET(GX2Rect, 0x0, left);
WUT_CHECK_OFFSET(GX2Rect, 0x4, top);
WUT_CHECK_OFFSET(GX2Rect, 0x8, right);
WUT_CHECK_OFFSET(GX2Rect, 0xc, bottom);
WUT_CHECK_SIZE(GX2Rect, 0x10);

struct GX2Point
{
   int32_t x;
   int32_t y;
};
WUT_CHECK_OFFSET(GX2Point, 0x0, x);
WUT_CHECK_OFFSET(GX2Point, 0x4, y);
WUT_CHECK_SIZE(GX2Point, 0x8);

void
GX2CalcSurfaceSizeAndAlignment(GX2Surface *surface);

void
GX2CalcDepthBufferHiZInfo(GX2DepthBuffer *depthBuffer,
                          uint32_t *outSize,
                          uint32_t *outAlignment);

void
GX2CalcColorBufferAuxInfo(GX2ColorBuffer *surface,
                          uint32_t *outSize,
                          uint32_t *outAlignment);

void
GX2SetColorBuffer(const GX2ColorBuffer *colorBuffer,
                  GX2RenderTarget target);

void
GX2SetDepthBuffer(const GX2DepthBuffer *depthBuffer);

void
GX2InitColorBufferRegs(GX2ColorBuffer *colorBuffer);

void
GX2InitDepthBufferRegs(GX2DepthBuffer *depthBuffer);

void
GX2InitDepthBufferHiZEnable(GX2DepthBuffer *depthBuffer,
                            BOOL enable);

/** Set AA mode explicitly, including when rendering without color target 0. */
void
GX2SetAAMode(GX2AAMode aa);

/** Pitch in pixels, or in 4x4 blocks for block-compressed formats. */
uint32_t
GX2GetSurfaceMipPitch(GX2Surface *surface,
                      uint32_t mipLevel);

/** Padded byte size of one 2D slice at the requested mip level. */
uint32_t
GX2GetSurfaceMipSliceSize(GX2Surface *surface,
                          uint32_t mipLevel);

uint32_t
GX2GetSurfaceSwizzle(const GX2Surface *surface);

void
GX2SetSurfaceSwizzle(GX2Surface *surface,
                     uint32_t swizzle);

void
GX2CopySurface(const GX2Surface *src,
               uint32_t srcLevel,
               uint32_t srcDepth,
               GX2Surface *dst,
               uint32_t dstLevel,
               uint32_t dstDepth);

/**
 * Copy up to GX2_MAX_COPY_SURFACE_EX_RECTS rectangles within one mip/slice.
 * Coordinates are mip-relative pixels, including for compressed formats,
 * which require 4x4 pixel alignment. Formats must match; no stretching or
 * LINEAR_SPECIAL surfaces are supported. Restore context state afterward.
 */
void
GX2CopySurfaceEx(const GX2Surface *src,
                 uint32_t srcLevel,
                 uint32_t srcDepth,
                 GX2Surface *dst,
                 uint32_t dstLevel,
                 uint32_t dstDepth,
                 uint32_t numRects,
                 GX2Rect *srcRects,
                 GX2Point *dstPoints);

void
GX2ResolveAAColorBuffer(const GX2ColorBuffer *srcColorBuffer,
                        GX2Surface *dstSurface,
                        uint32_t dstMip,
                        uint32_t dstSlice);

/**
 * Expand the selected mip/slices in place for multisample texture sampling.
 * Modifies color and auxiliary storage. Rendering makes expansion necessary
 * again. Not required before resolve. Restore context state afterward.
 */
void
GX2ExpandAAColorBuffer(GX2ColorBuffer *colorBuffer);

/**
 * Expand Hi-Z in place for texture-compatible, single-sample depth formats.
 * Modifies depth and Hi-Z storage. Rendering makes expansion necessary again.
 * D24S8 and multisample depth need conversion instead. Restore context state
 * afterward.
 */
void
GX2ExpandDepthBuffer(GX2DepthBuffer *depthBuffer);

/**
 * Convert depth tiling to texture/color tiling, preserving sample count.
 * Copies viewNumSlices from viewFirstSlice at viewMip into dstMip/dstSlice.
 * Destination use must exclude DEPTH_BUFFER. In-place conversion requires
 * matching tile modes and ends depth-buffer use of that storage.
 * Restore context state afterward.
 */
void
GX2ConvertDepthBufferToTextureSurface(const GX2DepthBuffer *srcDepthBuffer,
                                      GX2Surface *dstSurface,
                                      uint32_t dstMip,
                                      uint32_t dstSlice);

#ifdef __cplusplus
}
#endif

/** @} */
