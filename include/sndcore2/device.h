#pragma once
#include <wut.h>
#include "result.h"

/**
 * \defgroup sndcore2_device Device
 * \ingroup sndcore2
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AXAuxCallbackData AXAuxCallbackData;
typedef struct AXDeviceFinalMixData AXDeviceFinalMixData;
typedef struct AXDeviceFinalOutputInfo AXDeviceFinalOutputInfo;

typedef void (*AXDeviceFinalMixCallback)(AXDeviceFinalMixData *info);
typedef void (*AXAuxCallback)(int32_t **data,
                              void *context,
                              AXAuxCallbackData *info);

//! A value from enum AX_DEVICE_MODE.
typedef uint32_t AXDeviceMode;

//! A value from enum AX_DEVICE_TYPE.
typedef uint32_t AXDeviceType;

enum AX_DEVICE_MODE
{
   AX_DEVICE_MODE_STEREO    = 0,
   AX_DEVICE_MODE_SURROUND  = 1,
   AX_DEVICE_MODE_DPL2      = 2,
   AX_DEVICE_MODE_6_CHANNEL = 3,
   AX_DEVICE_MODE_MONO      = 5,
};

enum AX_DEVICE_TYPE
{
   AX_DEVICE_TYPE_TV         = 0,
   AX_DEVICE_TYPE_DRC        = 1,
   AX_DEVICE_TYPE_CONTROLLER = 2,
};

struct AXAuxCallbackData
{
   uint32_t numChannels;
   uint32_t numSamples;
};
WUT_CHECK_OFFSET(AXAuxCallbackData, 0x00, numChannels);
WUT_CHECK_OFFSET(AXAuxCallbackData, 0x04, numSamples);
WUT_CHECK_SIZE(AXAuxCallbackData, 0x08);

struct AXDeviceFinalMixData
{
   int32_t **data;
   uint16_t numInputChannels;
   uint16_t numSamples;
   uint16_t numDevices;
   uint16_t numOutputChannels;
};
WUT_CHECK_OFFSET(AXDeviceFinalMixData, 0x00, data);
WUT_CHECK_OFFSET(AXDeviceFinalMixData, 0x04, numInputChannels);
WUT_CHECK_OFFSET(AXDeviceFinalMixData, 0x06, numSamples);
WUT_CHECK_OFFSET(AXDeviceFinalMixData, 0x08, numDevices);
WUT_CHECK_OFFSET(AXDeviceFinalMixData, 0x0A, numOutputChannels);
WUT_CHECK_SIZE(AXDeviceFinalMixData, 0x0C);

struct AXDeviceFinalOutputInfo
{
   uint32_t numChannels;
   uint32_t sampleRate;
   uint32_t numDevices;
   uint32_t format;
   uint32_t length;
};
WUT_CHECK_OFFSET(AXDeviceFinalOutputInfo, 0x00, numChannels);
WUT_CHECK_OFFSET(AXDeviceFinalOutputInfo, 0x04, sampleRate);
WUT_CHECK_OFFSET(AXDeviceFinalOutputInfo, 0x08, numDevices);
WUT_CHECK_OFFSET(AXDeviceFinalOutputInfo, 0x0C, format);
WUT_CHECK_OFFSET(AXDeviceFinalOutputInfo, 0x10, length);
WUT_CHECK_SIZE(AXDeviceFinalOutputInfo, 0x14);

AXResult
AXSetDeviceMode(AXDeviceType type,
                AXDeviceMode mode);

AXResult
AXGetDeviceMode(AXDeviceType type,
                AXDeviceMode *mode);

AXResult
AXGetDeviceChannelCount(AXDeviceType type,
                        uint32_t id,
                        uint32_t *count);

AXResult
AXGetDeviceFinalMixCallback(AXDeviceType type,
                            AXDeviceFinalMixCallback *func);

AXResult
AXRegisterDeviceFinalMixCallback(AXDeviceType type,
                                 AXDeviceFinalMixCallback func);

AXResult
AXGetPostFinalMixCallback(AXDeviceFinalMixCallback *func);

AXResult
AXRegisterPostFinalMixCallback(AXDeviceFinalMixCallback func);

AXResult
AXGetAuxCallback(AXDeviceType type,
                 uint32_t id,
                 uint32_t auxId,
                 AXAuxCallback *callback,
                 void **userData);

AXResult
AXRegisterAuxCallback(AXDeviceType type,
                      uint32_t id,
                      uint32_t auxId,
                      AXAuxCallback callback,
                      void *userData);

AXResult
AXSetDeviceRemixMatrix(AXDeviceType type,
                       uint32_t numInputChannels,
                       uint32_t numOutputChannels,
                       const float *matrix);

AXResult
AXGetDeviceRemixMatrix(AXDeviceType type,
                       uint32_t numInputChannels,
                       uint32_t numOutputChannels,
                       const float **matrix);

AXResult
AXSetDeviceLinearUpsampler(AXDeviceType type,
                           uint32_t id,
                           BOOL enabled);

AXResult
AXSetDeviceCompressor(AXDeviceType type,
                      uint32_t enabled);

AXResult
AXSetDeviceCompressorTable(AXDeviceType type,
                           uint16_t *table,
                           uint16_t frames);

void
AXMakeCompressorTable(float gain,
                      uint16_t frames,
                      uint16_t *table);

AXResult
AXGetDeviceUpsampleStage(AXDeviceType type,
                         BOOL *postFinalMix);

AXResult
AXSetDeviceUpsampleStage(AXDeviceType type,
                         BOOL postFinalMix);

AXResult
AXGetDeviceVolume(AXDeviceType type,
                  uint32_t id,
                  uint16_t *volume);

AXResult
AXSetDeviceVolume(AXDeviceType type,
                  uint32_t id,
                  uint16_t volume);

AXResult
AXGetAuxReturnVolume(AXDeviceType type,
                     uint32_t id,
                     uint32_t auxId,
                     uint16_t *volume);

AXResult
AXSetAuxReturnVolume(AXDeviceType type,
                     uint32_t id,
                     uint32_t auxId,
                     uint16_t volume);

AXResult
AXGetDeviceFinalOutput(AXDeviceType type,
                       int16_t *data,
                       uint32_t size,
                       AXDeviceFinalOutputInfo *info);

#ifdef __cplusplus
}
#endif

/** @} */
