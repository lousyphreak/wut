#pragma once
#include <wut.h>
#include <coreinit/event.h>
#include <coreinit/memres.h>
#include <uac/uac.h>

/**
 * \defgroup usb_mic USB Microphone
 * \ingroup usb_mic
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef int16_t USBMICPCMSample;
typedef int32_t USBMICHandle;

typedef struct USBMICRingBuffer USBMICRingBuffer;
typedef struct USBMICStatus USBMICStatus;

typedef UACEventType USBMICEventType;
typedef UACEventData USBMICEventData;
typedef UACEventDataEx USBMICEventDataEx;
typedef UACDeviceCaps USBMICDeviceCaps;
typedef UACSampleRateFlags USBMICSampleRateFlags;

typedef void (*USBMICEventHandler)(void *context,
                                   USBMICEventType type,
                                   USBMICEventData *event,
                                   int32_t error);

//! A value from enum USBMIC_STATE.
typedef uint32_t USBMICState;

//! A value from enum USBMIC_STATUS_FLAGS.
typedef uint32_t USBMICStatusFlags;

enum USBMIC_STATE
{
   USBMIC_STATE_SAMPLE_RATE         = 0,
   USBMIC_STATE_GAIN_DB             = 1,
   USBMIC_STATE_GAIN_MIN            = 2,
   USBMIC_STATE_GAIN_MAX            = 3,
   USBMIC_STATE_GAIN_STEP           = 4,
   USBMIC_STATE_MUTE                = 5,
   USBMIC_STATE_DIGITAL_GAIN_DB     = 6,
   USBMIC_STATE_QUEUE_DEPTH         = 7,
   USBMIC_STATE_XFERS_PER_ENTRY     = 8,
   USBMIC_STATE_MAX_QUEUE_DEPTH     = 9,
   USBMIC_STATE_MAX_XFERS_PER_ENTRY = 10,
   USBMIC_STATE_CORE                = 11,
};

enum USBMIC_STATUS_FLAGS
{
   USBMIC_STATUS_FLAG_PCM16 = 1 << 0,
};

struct USBMICRingBuffer
{
   uint32_t sampleCapacity;
   USBMICPCMSample *sampleBuffer;
};
WUT_CHECK_OFFSET(USBMICRingBuffer, 0x00, sampleCapacity);
WUT_CHECK_OFFSET(USBMICRingBuffer, 0x04, sampleBuffer);
WUT_CHECK_SIZE(USBMICRingBuffer, 0x08);

struct USBMICStatus
{
   USBMICStatusFlags flags;
   uint32_t availableSamples;
   uint32_t readIndex;
};
WUT_CHECK_OFFSET(USBMICStatus, 0x00, flags);
WUT_CHECK_OFFSET(USBMICStatus, 0x04, availableSamples);
WUT_CHECK_OFFSET(USBMICStatus, 0x08, readIndex);
WUT_CHECK_SIZE(USBMICStatus, 0x0C);

int32_t
USBMICInit(void *context,
           USBMICEventHandler eventHandler);

int32_t
USBMICUninit(void);

int32_t
USBMICOpenQuery(USBMICHandle handle,
                MEMResourceQuery *resourceQuery);

int32_t
USBMICOpen(USBMICHandle handle,
           MEMResource *resource,
           USBMICRingBuffer *ringBuffer);

int32_t
USBMICClose(USBMICHandle handle);

int32_t
USBMICStart(USBMICHandle handle);

int32_t
USBMICStartExt(USBMICHandle handle,
               OSEvent *waitEvent,
               uint32_t delayUs);

int32_t
USBMICStop(USBMICHandle handle);

int32_t
USBMICGetStatus(USBMICHandle handle,
                USBMICStatus *status);

int32_t
USBMICSetDataConsumed(USBMICHandle handle,
                      uint32_t samples);

int32_t
USBMICSetState(USBMICHandle handle,
               USBMICState state,
               uint32_t value);

int32_t
USBMICGetState(USBMICHandle handle,
               USBMICState state,
               uint32_t *value);

#ifdef __cplusplus
}
#endif

/** @} */
