#pragma once
#include <wut.h>
#include <coreinit/event.h>
#include <coreinit/memres.h>

/**
 * \defgroup uac USB Audio Class
 * \ingroup uac
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef int16_t UACPCMSample;
typedef int32_t UACHandle;

typedef struct UACRingBuffer UACRingBuffer;
typedef struct UACStatus UACStatus;
typedef struct UACEventData UACEventData;
typedef struct UACDeviceAttributes UACDeviceAttributes;
typedef struct UACEventDataEx UACEventDataEx;

//! A value from enum UAC_STATE.
typedef uint32_t UACState;

//! A value from enum UAC_EVENT_TYPE.
typedef uint32_t UACEventType;

//! A value from enum UAC_STATUS_FLAGS.
typedef uint32_t UACStatusFlags;

//! A value from enum UAC_DEVICE_CAPS.
typedef uint32_t UACDeviceCaps;

//! A value from enum UAC_SAMPLE_RATE_FLAGS.
typedef uint32_t UACSampleRateFlags;

typedef void (*UACEventHandler)(void *context,
                                UACEventType type,
                                UACEventData *event,
                                int32_t error);

enum UAC_STATE
{
   UAC_STATE_SAMPLE_RATE         = 0,
   UAC_STATE_GAIN_DB             = 1,
   UAC_STATE_GAIN_MIN            = 2,
   UAC_STATE_GAIN_MAX            = 3,
   UAC_STATE_GAIN_STEP           = 4,
   UAC_STATE_MUTE                = 5,
   UAC_STATE_RESERVED_6          = 6,
   UAC_STATE_ECHO_CANCELLATION   = 7,
   UAC_STATE_AUTO_SELECTION      = 8,
   UAC_STATE_DIGITAL_GAIN_DB     = 9,
   UAC_STATE_QUEUE_DEPTH         = 10,
   UAC_STATE_XFERS_PER_ENTRY     = 11,
   UAC_STATE_QUEUE_DEPTH_OUT     = 12,
   UAC_STATE_XFERS_PER_ENTRY_OUT = 13,
   UAC_STATE_MAX_QUEUE_DEPTH     = 14,
   UAC_STATE_MAX_XFERS_PER_ENTRY = 15,
   UAC_STATE_CORE                = 16,
};

enum UAC_EVENT_TYPE
{
   UAC_EVENT_TYPE_ATTACH = 0,
   UAC_EVENT_TYPE_DETACH = 1,
   UAC_EVENT_TYPE_ERROR  = 2,
};

enum UAC_STATUS_FLAGS
{
   UAC_STATUS_FLAG_PCM16     = 1 << 0,
   UAC_STATUS_FLAG_OPEN      = 1 << 1,
   UAC_STATUS_FLAG_CONNECTED = 1 << 2,
};

enum UAC_DEVICE_CAPS
{
   UAC_DEVICE_CAPS_INPUT  = 1 << 0,
   UAC_DEVICE_CAPS_OUTPUT = 1 << 1,
};

enum UAC_SAMPLE_RATE_FLAGS
{
   UAC_SAMPLE_RATE_FLAG_32KHZ = 1 << 5,
   UAC_SAMPLE_RATE_FLAG_48KHZ = 1 << 7,
};

struct UACRingBuffer
{
   uint32_t sampleCapacity;
   UACPCMSample *sampleBuffer;
};
WUT_CHECK_OFFSET(UACRingBuffer, 0x00, sampleCapacity);
WUT_CHECK_OFFSET(UACRingBuffer, 0x04, sampleBuffer);
WUT_CHECK_SIZE(UACRingBuffer, 0x08);

struct UACStatus
{
   UACStatusFlags flags;
   uint32_t availableSamples;
   uint32_t readIndex;
};
WUT_CHECK_OFFSET(UACStatus, 0x00, flags);
WUT_CHECK_OFFSET(UACStatus, 0x04, availableSamples);
WUT_CHECK_OFFSET(UACStatus, 0x08, readIndex);
WUT_CHECK_SIZE(UACStatus, 0x0C);

struct UACEventData
{
   uint32_t host;
   uint32_t port;
   uint32_t vendorId;
   uint32_t productId;
   UACHandle handle;
};
WUT_CHECK_OFFSET(UACEventData, 0x00, host);
WUT_CHECK_OFFSET(UACEventData, 0x04, port);
WUT_CHECK_OFFSET(UACEventData, 0x08, vendorId);
WUT_CHECK_OFFSET(UACEventData, 0x0C, productId);
WUT_CHECK_OFFSET(UACEventData, 0x10, handle);
WUT_CHECK_SIZE(UACEventData, 0x14);

struct UACDeviceAttributes
{
   uint32_t numChannels;
   UACSampleRateFlags sampleRates;
};
WUT_CHECK_OFFSET(UACDeviceAttributes, 0x00, numChannels);
WUT_CHECK_OFFSET(UACDeviceAttributes, 0x04, sampleRates);
WUT_CHECK_SIZE(UACDeviceAttributes, 0x08);

struct UACEventDataEx
{
   UACEventData eventData;
   UACDeviceCaps caps;
   UACDeviceAttributes input;
   UACDeviceAttributes output;
};
WUT_CHECK_OFFSET(UACEventDataEx, 0x00, eventData);
WUT_CHECK_OFFSET(UACEventDataEx, 0x14, caps);
WUT_CHECK_OFFSET(UACEventDataEx, 0x18, input);
WUT_CHECK_OFFSET(UACEventDataEx, 0x20, output);
WUT_CHECK_SIZE(UACEventDataEx, 0x28);

int32_t
UACInit(void *context,
        UACEventHandler eventHandler);

int32_t
UACUninit(void);

int32_t
UACOpenQuery(UACHandle handle,
             MEMResourceQuery *resourceQuery);

int32_t
UACOpen(UACHandle handle,
        MEMResource *resource,
        UACRingBuffer *ringBuffer);

int32_t
UACClose(UACHandle handle);

int32_t
UACINStart(UACHandle handle);

int32_t
UACINStartExt(UACHandle handle,
              OSEvent *waitEvent,
              uint32_t delayUs);

int32_t
UACINStop(UACHandle handle);

int32_t
UACINGetStatus(UACHandle handle,
               UACStatus *status);

int32_t
UACINSetDataConsumed(UACHandle handle,
                     uint32_t samples);

int32_t
UACSetState(UACHandle handle,
            UACState state,
            uint32_t value);

int32_t
UACGetState(UACHandle handle,
            UACState state,
            uint32_t *value);

int32_t
UACOUTStart(UACHandle handle,
            UACRingBuffer *ringBuffer,
            uint32_t sampleRate);

int32_t
UACOUTStartExt(UACHandle handle,
               OSEvent *waitEvent,
               uint32_t delayUs,
               UACRingBuffer *ringBuffer,
               uint32_t sampleRate);

int32_t
UACOUTStop(UACHandle handle);

int32_t
UACOUTGetStatus(UACHandle handle,
                UACStatus *status);

int32_t
UACOUTSetDataConsumed(UACHandle handle,
                      uint32_t samples);

#ifdef __cplusplus
}
#endif

/** @} */
