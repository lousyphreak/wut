#pragma once
#include <wut.h>
#include <coreinit/memres.h>

/**
 * \defgroup mic Microphone
 * \ingroup mic
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef int MICHandle;

typedef int16_t MICPCMSample;
typedef struct MICRingBuffer MICRingBuffer;
typedef struct MICStatus MICStatus;

//! A value from enum MIC_STATUS_FLAGS.
typedef uint32_t MICStatusFlags;

typedef enum MICError
{
   MIC_ERROR_OK              = 0,
   MIC_ERROR_NOT_SUPPORTED   = -1,
   MIC_ERROR_INVALID_ARG     = -2,
   MIC_ERROR_INVALID_STATE   = -3,
   MIC_ERROR_NO_MEMORY       = -4,
   MIC_ERROR_ALREADY_OPEN    = -5,
   MIC_ERROR_NOT_OPEN        = -6,
   MIC_ERROR_NOT_INITIALIZED = -7,
   MIC_ERROR_NOT_CONNECTED   = -8,
} MICError;

typedef enum MICState
{
   MIC_STATE_SAMPLE_RATE       = 0,
   MIC_STATE_GAIN_DB           = 1,
   MIC_STATE_GAIN_MIN          = 2,
   MIC_STATE_GAIN_MAX          = 3,
   MIC_STATE_GAIN_STEP         = 4,
   MIC_STATE_MUTE              = 5,
   MIC_STATE_RESERVED_6        = 6,
   MIC_STATE_ECHO_CANCELLATION = 7,
   MIC_STATE_AUTO_SELECTION    = 8,
   MIC_STATE_DIGITAL_GAIN_DB   = 9,
} MICState;

enum MIC_STATUS_FLAGS
{
   MIC_STATUS_FLAG_PCM16     = 1 << 0,
   MIC_STATUS_FLAG_OPEN      = 1 << 1,
   MIC_STATUS_FLAG_CONNECTED = 1 << 2,
};

typedef enum MICInstance
{
   MIC_INSTANCE_0 = 0,
   MIC_INSTANCE_1 = 1
} MICInstance;

struct MICRingBuffer
{
   //! Capacity in signed 16-bit PCM samples.
   uint32_t sampleCapacity;
   //! A 0x40-aligned buffer containing sampleCapacity samples.
   MICPCMSample *sampleBuffer;
};
WUT_CHECK_OFFSET(MICRingBuffer, 0x00, sampleCapacity);
WUT_CHECK_OFFSET(MICRingBuffer, 0x04, sampleBuffer);
WUT_CHECK_SIZE(MICRingBuffer, 0x08);

struct MICStatus
{
   MICStatusFlags flags;
   uint32_t availableSamples;
   uint32_t readIndex;
};
WUT_CHECK_OFFSET(MICStatus, 0x00, flags);
WUT_CHECK_OFFSET(MICStatus, 0x04, availableSamples);
WUT_CHECK_OFFSET(MICStatus, 0x08, readIndex);
WUT_CHECK_SIZE(MICStatus, 0x0C);

MICHandle
MICInit(MICInstance instance,
        MEMResource *resource,
        MICRingBuffer *ringBuffer,
        MICError *outError);

MICError
MICOpen(MICHandle handle);

MICError
MICGetState(MICHandle handle,
            MICState state,
            uint32_t *outStateVal);

MICError
MICSetState(MICHandle handle,
            MICState state,
            uint32_t stateVal);

MICError
MICGetStatus(MICHandle handle,
             MICStatus *outStatus);

MICError
MICSetDataConsumed(MICHandle handle,
                   int dataAmountConsumed);

MICError
MICClose(MICHandle handle);

MICError
MICUninit(MICHandle handle);

#ifdef __cplusplus
}
#endif

/** @} */
