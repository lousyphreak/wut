#pragma once
#include <wut.h>

/**
 * \defgroup gx2_perf Performance Counters
 * \ingroup gx2
 *
 * Low-level GPU counters. Do not mix with high-level metric collection.
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum GX2PerfCounter
{
   GX2_PERF_COUNTER_GRBM_0  = 0x01,
   GX2_PERF_COUNTER_GRBM_1  = 0x02,
   GX2_PERF_COUNTER_TA_0_0  = 0x21,
   GX2_PERF_COUNTER_TCP_0_0 = 0x25,
   GX2_PERF_COUNTER_TCP_1_0 = 0x27,
   GX2_PERF_COUNTER_TCP_2_0 = 0x29,
   GX2_PERF_COUNTER_TCP_3_0 = 0x2b,
   GX2_PERF_COUNTER_TCC_0_0 = 0x2d,
   GX2_PERF_COUNTER_TCC_1_0 = 0x2f,
   GX2_PERF_COUNTER_TCC_2_0 = 0x31,
   GX2_PERF_COUNTER_TCC_3_0 = 0x33,
} GX2PerfCounter;

typedef enum GX2PerfEvent
{
   GX2_PERF_EVENT_GRBM_COUNT                    = 0x1000,
   GX2_PERF_EVENT_GRBM_GUI_ACTIVE               = 0x1001,
   GX2_PERF_EVENT_TA_ALIGNER_CYCLES             = 0xa016,
   GX2_PERF_EVENT_TCP_TCC_STALL_CYCLES          = 0xb003,
   GX2_PERF_EVENT_TCP_TAG_CONFLICT_STALL_CYCLES = 0xb00d,
   GX2_PERF_EVENT_TCP_TCC_REQUEST_LATENCY       = 0xb017,
   GX2_PERF_EVENT_TCP_TOTAL_PIXELS              = 0xb03b,
   GX2_PERF_EVENT_TCC_CYCLES                    = 0xc001,
   GX2_PERF_EVENT_TCC_BUSY                      = 0xc002,
   GX2_PERF_EVENT_TCC_TEXTURE_REQUEST_HITS      = 0xc023,
   GX2_PERF_EVENT_TCC_TEXTURE_REQUEST_MISSES    = 0xc024,
} GX2PerfEvent;

/** Allocate with at least 32-byte alignment. Configuration is CPU-owned. */
typedef struct GX2PerfCounterInfo GX2PerfCounterInfo;

struct GX2PerfCounterInfo
{
   uint8_t config[0x4a0];
   WUT_ALIGNAS(32)
   uint8_t results[0x340];
};
WUT_CHECK_OFFSET(GX2PerfCounterInfo, 0x4a0, results);
WUT_CHECK_SIZE(GX2PerfCounterInfo, 0x7e0);

/** GPU system clock, distinct from the 27 MHz timestamp clock. */
uint32_t
GX2GetGPUSystemClock(void);

void
GX2ResetCounterInfo(GX2PerfCounterInfo *info);

BOOL
GX2InitCounterInfo(GX2PerfCounterInfo *info,
                   GX2PerfCounter counter,
                   GX2PerfEvent event);

BOOL
GX2GetCounterSetting(GX2PerfCounterInfo *info,
                     GX2PerfCounter counter,
                     GX2PerfEvent *outEvent);

uint32_t
GX2GetCounterResultSize(GX2PerfCounterInfo *info);

void
GX2SetCounterInfo(const GX2PerfCounterInfo *info);

void
GX2ResetCounters(const GX2PerfCounterInfo *info);

void
GX2StartCounters(const GX2PerfCounterInfo *info);

void
GX2StopCounters(const GX2PerfCounterInfo *info);

/**
 * Queue GPU result writes. The call initializes/publishes the result sentinel.
 * Keep the result buffer alive through GPU completion; it must be 32-byte aligned
 * and large enough for GX2GetCounterResultSize(). Use separate cache-line-owned
 * storage for asynchronous samples. These functions do not wait for completion.
 */
void
GX2SampleCountersEx(GX2PerfCounterInfo *info,
                    uint64_t *results);

void
GX2SampleCounters(GX2PerfCounterInfo *info);

/**
 * Invalidate the result buffer's CPU cache lines before polling GPU writes.
 * Return UINT64_MAX while the requested GPU result is unavailable.
 */
uint64_t
GX2GetCounterResultEx(GX2PerfCounterInfo *info,
                      uint64_t *results,
                      GX2PerfEvent event);

uint64_t
GX2GetCounterResult(GX2PerfCounterInfo *info,
                    GX2PerfEvent event);

#ifdef __cplusplus
}
#endif

/** @} */
