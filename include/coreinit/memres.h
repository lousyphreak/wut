#pragma once
#include <wut.h>

/**
 * \defgroup coreinit_memres Memory Resources
 * \ingroup coreinit
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MEMResourceQueryElement MEMResourceQueryElement;
typedef struct MEMResourceQuery MEMResourceQuery;
typedef struct MEMResourceElement MEMResourceElement;
typedef struct MEMResource MEMResource;

/** Requirements for one memory resource. */
struct MEMResourceQueryElement
{
   uint32_t size;
   uint32_t alignment;
   uint32_t quantumSize;
   uint32_t quantumCount;
   uint32_t maxQuantumCount;
   uint32_t flags;
   WUT_UNKNOWN_BYTES(0x0C);
};
WUT_CHECK_OFFSET(MEMResourceQueryElement, 0x00, size);
WUT_CHECK_OFFSET(MEMResourceQueryElement, 0x04, alignment);
WUT_CHECK_OFFSET(MEMResourceQueryElement, 0x08, quantumSize);
WUT_CHECK_OFFSET(MEMResourceQueryElement, 0x0C, quantumCount);
WUT_CHECK_OFFSET(MEMResourceQueryElement, 0x10, maxQuantumCount);
WUT_CHECK_OFFSET(MEMResourceQueryElement, 0x14, flags);
WUT_CHECK_SIZE(MEMResourceQueryElement, 0x24);

/** Array of resource requirements returned by an audio-device query. */
struct MEMResourceQuery
{
   uint32_t count;
   MEMResourceQueryElement *elements;
};
WUT_CHECK_OFFSET(MEMResourceQuery, 0x00, count);
WUT_CHECK_OFFSET(MEMResourceQuery, 0x04, elements);
WUT_CHECK_SIZE(MEMResourceQuery, 0x08);

/** Caller-provided allocation for one memory resource. */
struct MEMResourceElement
{
   uint32_t size;
   uint32_t alignment;
   uint32_t quantumSize;
   uint32_t quantumCount;
   uint32_t flags;
   void *buffer;
};
WUT_CHECK_OFFSET(MEMResourceElement, 0x00, size);
WUT_CHECK_OFFSET(MEMResourceElement, 0x04, alignment);
WUT_CHECK_OFFSET(MEMResourceElement, 0x08, quantumSize);
WUT_CHECK_OFFSET(MEMResourceElement, 0x0C, quantumCount);
WUT_CHECK_OFFSET(MEMResourceElement, 0x10, flags);
WUT_CHECK_OFFSET(MEMResourceElement, 0x14, buffer);
WUT_CHECK_SIZE(MEMResourceElement, 0x18);

/** Array of allocations supplied to an audio-device initializer. */
struct MEMResource
{
   uint32_t count;
   MEMResourceElement *elements;
};
WUT_CHECK_OFFSET(MEMResource, 0x00, count);
WUT_CHECK_OFFSET(MEMResource, 0x04, elements);
WUT_CHECK_SIZE(MEMResource, 0x08);

#ifdef __cplusplus
}
#endif

/** @} */
