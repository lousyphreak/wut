#pragma once
#include <wut.h>
#include "enum.h"

/**
 * \defgroup gx2_query Query
 * \ingroup gx2
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#define GX2_GPU_TIMESTAMP_FREQUENCY (27000000)
#define GX2_GPU_TIMESTAMP_INVALID   UINT64_MAX

typedef struct GX2QueryData GX2QueryData;

/** Raw query counters are GPU-owned little-endian data. */
struct GX2QueryData
{
   uint64_t regs[8];
};
WUT_CHECK_OFFSET(GX2QueryData, 0x00, regs);
WUT_CHECK_SIZE(GX2QueryData, 0x40);

/** Begin/end are asynchronous and must use the same type and storage. */
void
GX2QueryBegin(GX2QueryType type,
              GX2QueryData *query);

void
GX2QueryEnd(GX2QueryType type,
            GX2QueryData *query);

/** Return FALSE while the CPU query result is unavailable; never wait. */
BOOL
GX2QueryGetOcclusionResult(const GX2QueryData *query,
                           uint64_t *zPassCount);

/**
 * Use a GPU query result to predicate subsequent draws.
 * With hint TRUE an unavailable occlusion result permits drawing immediately;
 * with hint FALSE the GPU waits. Predicate TRUE draws for a nonzero result,
 * FALSE for zero. End conditional rendering before ordinary draws resume.
 * Display lists containing conditional rendering must use
 * GX2DirectCallDisplayList rather than GX2CallDisplayList.
 */
void
GX2QueryBeginConditionalRender(GX2QueryType type,
                               const GX2QueryData *query,
                               BOOL hint,
                               BOOL predicate);

void
GX2QueryEndConditionalRender(void);

/**
 * Asynchronously sample the 27 MHz clock at pipeline entry or completion.
 * These calls initially write GX2_GPU_TIMESTAMP_INVALID on the CPU.
 * Retain destination storage until GPU completion and invalidate CPU cache
 * before reading GPU output. The clock differs from the GPU system clock.
 */
void
GX2SampleTopGPUCycle(uint64_t *timestamp);

void
GX2SampleBottomGPUCycle(uint64_t *timestamp);

uint64_t
GX2CPUTimeToGPUTime(uint64_t cpuTime);

uint64_t
GX2GPUTimeToCPUTime(uint64_t gpuTime);

#ifdef __cplusplus
}
#endif

/** @} */
