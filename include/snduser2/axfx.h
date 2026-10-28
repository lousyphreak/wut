#pragma once
#include <wut.h>
#include <sndcore2/device.h>

/**
 * \defgroup snduser2_axfx AXFX
 * \ingroup snduser2
 *
 * AUX bus effect parameters and callbacks.
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AXFXBufferUpdate AXFXBufferUpdate;
typedef struct AXFX2Reverb AXFX2Reverb;

typedef void *(*AXFXAllocCallback)(uint32_t size);
typedef void (*AXFXFreeCallback)(void *ptr);

//! A value from enum AXFX_REVERB_MODE.
typedef uint32_t AXFXReverbMode;
//! A value from enum AXFX_SAMPLE_RATE.
typedef uint32_t AXFXSampleRate;
//! A value from enum AXFX_REVERB_PRESET.
typedef uint32_t AXFXReverbPreset;

enum AXFX_REVERB_MODE
{
   AXFX_REVERB_MODE_2CH = 0,
   AXFX_REVERB_MODE_4CH = 1,
   AXFX_REVERB_MODE_6CH = 2,
};

enum AXFX_SAMPLE_RATE
{
   AXFX_SAMPLE_RATE_32KHZ = 0,
   AXFX_SAMPLE_RATE_48KHZ = 1,
};

enum AXFX_REVERB_PRESET
{
   AXFX_REVERB_PRESET_SMALL_ROOM     = 0,
   AXFX_REVERB_PRESET_LARGE_ROOM     = 1,
   AXFX_REVERB_PRESET_HALL           = 2,
   AXFX_REVERB_PRESET_CATHEDRAL      = 3,
   AXFX_REVERB_PRESET_METAL_CORRIDOR = 4,
};

struct AXFXBufferUpdate
{
   int32_t *left;
   int32_t *right;
   int32_t *surroundLeft;
   int32_t *surroundRight;
   int32_t *center;
   int32_t *subwoofer;
};
WUT_CHECK_OFFSET(AXFXBufferUpdate, 0x00, left);
WUT_CHECK_OFFSET(AXFXBufferUpdate, 0x04, right);
WUT_CHECK_OFFSET(AXFXBufferUpdate, 0x08, surroundLeft);
WUT_CHECK_OFFSET(AXFXBufferUpdate, 0x0C, surroundRight);
WUT_CHECK_OFFSET(AXFXBufferUpdate, 0x10, center);
WUT_CHECK_OFFSET(AXFXBufferUpdate, 0x14, subwoofer);
WUT_CHECK_SIZE(AXFXBufferUpdate, 0x18);

struct AXFX2Reverb
{
   float reverbGain;
   uint32_t earlyMode;
   uint32_t lateMode;
   float predelayTime;
   float decayTime;
   float hfDecayRatio;
   float coloration;
   float earlyGain;
   float lateGain;
   float outputGain;
   float dryGain;
};
WUT_CHECK_OFFSET(AXFX2Reverb, 0x00, reverbGain);
WUT_CHECK_OFFSET(AXFX2Reverb, 0x04, earlyMode);
WUT_CHECK_OFFSET(AXFX2Reverb, 0x08, lateMode);
WUT_CHECK_OFFSET(AXFX2Reverb, 0x0C, predelayTime);
WUT_CHECK_OFFSET(AXFX2Reverb, 0x10, decayTime);
WUT_CHECK_OFFSET(AXFX2Reverb, 0x14, hfDecayRatio);
WUT_CHECK_OFFSET(AXFX2Reverb, 0x18, coloration);
WUT_CHECK_OFFSET(AXFX2Reverb, 0x1C, earlyGain);
WUT_CHECK_OFFSET(AXFX2Reverb, 0x20, lateGain);
WUT_CHECK_OFFSET(AXFX2Reverb, 0x24, outputGain);
WUT_CHECK_OFFSET(AXFX2Reverb, 0x28, dryGain);
WUT_CHECK_SIZE(AXFX2Reverb, 0x2C);

void
AXFXSetHooks(AXFXAllocCallback alloc,
             AXFXFreeCallback free);

int32_t
AXFX2ReverbGetMemSize(AXFXReverbMode mode,
                      AXFXSampleRate sampleRate);

BOOL
AXFX2ReverbInit(AXFX2Reverb *reverb,
                AXFXReverbMode mode,
                AXFXSampleRate sampleRate);

void
AXFX2ReverbShutdown(AXFX2Reverb *reverb);

BOOL
AXFX2ReverbParametersPreset(AXFX2Reverb *reverb,
                            AXFXReverbPreset preset);

BOOL
AXFX2ReverbSettingsUpdate(AXFX2Reverb *reverb);

BOOL
AXFX2ReverbSettingsUpdateNoReset(AXFX2Reverb *reverb);

void
AXFX2ReverbCallback(AXFXBufferUpdate *bufferUpdate,
                    AXFX2Reverb *reverb,
                    AXAuxCallbackData *info);

#ifdef __cplusplus
}
#endif

/** @} */
