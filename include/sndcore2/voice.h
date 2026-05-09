#pragma once
#include <wut.h>
#include "device.h"
#include "result.h"

/**
 * \defgroup sndcore2_voice Voice
 * \ingroup sndcore2
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AXVoice AXVoice;
typedef struct AXVoiceAdpcmLoopData AXVoiceAdpcmLoopData;
typedef struct AXVoiceAdpcm AXVoiceAdpcm;
typedef struct AXMultiVoice AXMultiVoice;
typedef struct AXVoiceBiquadFilter AXVoiceBiquadFilter;
typedef struct AXVoiceDRCMixData AXVoiceDRCMixData;
typedef struct AXVoiceDeviceBusMixData AXVoiceDeviceBusMixData;
typedef struct AXVoiceDeviceMixData AXVoiceDeviceMixData;
typedef struct AXVoiceLink AXVoiceLink;
typedef struct AXVoiceLowPassFilter AXVoiceLowPassFilter;
typedef struct AXVoiceMixData AXVoiceMixData;
typedef struct AXVoiceOffsets AXVoiceOffsets;
typedef union AXVoiceRemoteIIRFilter AXVoiceRemoteIIRFilter;
typedef struct AXVoiceRemoteMixData AXVoiceRemoteMixData;
typedef struct AXVoiceRemoteSrc AXVoiceRemoteSrc;
typedef struct AXVoiceSrc AXVoiceSrc;
typedef struct AXVoiceVeData AXVoiceVeData;

//! A value from enum AX_VOICE_FORMAT.
typedef uint16_t AXVoiceFormat;

//! A value from enum AX_VOICE_LOOP.
typedef uint16_t AXVoiceLoop;

//! A value from enum AX_VOICE_SRC_TYPE.
typedef uint32_t AXVoiceSrcType;

//! A value from enum AX_VOICE_STATE.
typedef uint32_t AXVoiceState;

//! A value from enum AX_VOICE_RENDERER.
typedef uint32_t AXVoiceRenderer;

//! A value from enum AX_VOICE_RATIO_RESULT.
typedef int32_t AXVoiceSrcRatioResult;

//! A value from enum AX_VOICE_TYPE.
typedef uint32_t AXVoiceType;

//! A value from enum AX_BUS_TYPE.
typedef uint32_t AXBusType;

typedef void (*AXVoiceCallbackFn)(void *);
typedef void (*AXVoiceCallbackExFn)(void *, uint32_t, uint32_t);

enum AX_VOICE_FORMAT
{
   AX_VOICE_FORMAT_ADPCM  = 0,
   AX_VOICE_FORMAT_LPCM16 = 10,
   AX_VOICE_FORMAT_LPCM8  = 25,
};

enum AX_VOICE_LOOP
{
   AX_VOICE_LOOP_DISABLED = 0,
   AX_VOICE_LOOP_ENABLED  = 1,
};

enum AX_VOICE_RENDERER
{
   AX_VOICE_RENDERER_DSP  = 0,
   AX_VOICE_RENDERER_CPU  = 1,
   AX_VOICE_RENDERER_AUTO = 2,
};

enum AX_VOICE_RATIO_RESULT
{
   AX_VOICE_RATIO_RESULT_SUCCESS                = 0,
   AX_VOICE_RATIO_RESULT_LESS_THAN_ZERO         = -1,
   AX_VOICE_RATIO_RESULT_GREATER_THAN_SOMETHING = -2,
};

enum AX_VOICE_SRC_TYPE
{
   AX_VOICE_SRC_TYPE_POLYPHASE = 0,
   AX_VOICE_SRC_TYPE_LINEAR    = 1,
   AX_VOICE_SRC_TYPE_NONE      = 2,
   AX_VOICE_SRC_TYPE_UNK3      = 3,
   AX_VOICE_SRC_TYPE_UNK4      = 4,
};

enum AX_VOICE_STATE
{
   AX_VOICE_STATE_STOPPED = 0,
   AX_VOICE_STATE_PLAYING = 1,
};

enum AX_VOICE_TYPE
{
   AX_VOICE_TYPE_NORMAL = 0,
   AX_VOICE_TYPE_STREAM = 1,
};

enum AX_BUS_TYPE
{
   AX_BUS_TYPE_MAIN  = 0,
   AX_BUS_TYPE_AUX_A = 1,
   AX_BUS_TYPE_AUX_B = 2,
   AX_BUS_TYPE_AUX_C = 3,
};

struct AXVoiceLink
{
   AXVoice *next;
   AXVoice *prev;
};
WUT_CHECK_OFFSET(AXVoiceLink, 0x0, next);
WUT_CHECK_OFFSET(AXVoiceLink, 0x4, prev);
WUT_CHECK_SIZE(AXVoiceLink, 0x8);

struct AXVoiceOffsets
{
   AXVoiceFormat dataType;
   AXVoiceLoop loopingEnabled;
   uint32_t loopOffset;
   uint32_t endOffset;
   uint32_t currentOffset;
   const void *data;
};
WUT_CHECK_OFFSET(AXVoiceOffsets, 0x0, dataType);
WUT_CHECK_OFFSET(AXVoiceOffsets, 0x2, loopingEnabled);
WUT_CHECK_OFFSET(AXVoiceOffsets, 0x4, loopOffset);
WUT_CHECK_OFFSET(AXVoiceOffsets, 0x8, endOffset);
WUT_CHECK_OFFSET(AXVoiceOffsets, 0xc, currentOffset);
WUT_CHECK_OFFSET(AXVoiceOffsets, 0x10, data);
WUT_CHECK_SIZE(AXVoiceOffsets, 0x14);

struct AXVoice
{
   //! The index of this voice out of the total voices
   uint32_t index;

   //! Current play state of this voice
   AXVoiceState state;

   //! Current volume of this voice
   uint32_t volume;

   //! The renderer to use for this voice
   AXVoiceRenderer renderer;

   //! this is a link used in the stack, we do this in host-memory currently
   AXVoiceLink link;

   //! A link to the next callback to invoke
   AXVoice *cbNext;

   //! The priority of this voice used for force-acquiring a voice
   uint32_t priority;

   //! The callback to call if this is force-free'd by another acquire
   AXVoiceCallbackFn callback;

   //! The user context to send to the callbacks
   void *userContext;

   //! A bitfield representing different things needing to be synced.
   uint32_t syncBits;

   WUT_UNKNOWN_BYTES(0x8);

   //! The current offset data!
   AXVoiceOffsets offsets;

   //! An extended version of the callback above
   AXVoiceCallbackExFn callbackEx;

   //! The reason for the callback being invoked
   uint32_t callbackReason;

   float unk0;
   float unk1;
};
WUT_CHECK_OFFSET(AXVoice, 0x0, index);
WUT_CHECK_OFFSET(AXVoice, 0x4, state);
WUT_CHECK_OFFSET(AXVoice, 0x8, volume);
WUT_CHECK_OFFSET(AXVoice, 0xc, renderer);
WUT_CHECK_OFFSET(AXVoice, 0x10, link);
WUT_CHECK_OFFSET(AXVoice, 0x18, cbNext);
WUT_CHECK_OFFSET(AXVoice, 0x1c, priority);
WUT_CHECK_OFFSET(AXVoice, 0x20, callback);
WUT_CHECK_OFFSET(AXVoice, 0x24, userContext);
WUT_CHECK_OFFSET(AXVoice, 0x28, syncBits);
WUT_CHECK_OFFSET(AXVoice, 0x34, offsets);
WUT_CHECK_OFFSET(AXVoice, 0x48, callbackEx);
WUT_CHECK_OFFSET(AXVoice, 0x4c, callbackReason);
WUT_CHECK_OFFSET(AXVoice, 0x50, unk0);
WUT_CHECK_OFFSET(AXVoice, 0x54, unk1);
WUT_CHECK_SIZE(AXVoice, 0x58);

struct AXVoiceDeviceBusMixData
{
   uint16_t volume;
   int16_t delta;
};
WUT_CHECK_OFFSET(AXVoiceDeviceBusMixData, 0x0, volume);
WUT_CHECK_OFFSET(AXVoiceDeviceBusMixData, 0x2, delta);
WUT_CHECK_SIZE(AXVoiceDeviceBusMixData, 0x4);

struct AXVoiceDeviceMixData
{
   AXVoiceDeviceBusMixData bus[4];
};
WUT_CHECK_OFFSET(AXVoiceDeviceMixData, 0x0, bus);
WUT_CHECK_SIZE(AXVoiceDeviceMixData, 0x10);

struct AXVoiceVeData
{
   uint16_t volume;
   int16_t delta;
};
WUT_CHECK_OFFSET(AXVoiceVeData, 0x0, volume);
WUT_CHECK_OFFSET(AXVoiceVeData, 0x2, delta);
WUT_CHECK_SIZE(AXVoiceVeData, 0x4);

struct AXVoiceAdpcmLoopData
{
   uint16_t predScale;
   int16_t prevSample[2];
};
WUT_CHECK_OFFSET(AXVoiceAdpcmLoopData, 0x0, predScale);
WUT_CHECK_OFFSET(AXVoiceAdpcmLoopData, 0x2, prevSample);
WUT_CHECK_SIZE(AXVoiceAdpcmLoopData, 0x6);

struct AXVoiceAdpcm
{
   int16_t coefficients[16];
   uint16_t gain;
   uint16_t predScale;
   int16_t prevSample[2];
};
WUT_CHECK_OFFSET(AXVoiceAdpcm, 0x0, coefficients);
WUT_CHECK_OFFSET(AXVoiceAdpcm, 0x20, gain);
WUT_CHECK_OFFSET(AXVoiceAdpcm, 0x22, predScale);
WUT_CHECK_OFFSET(AXVoiceAdpcm, 0x24, prevSample);
WUT_CHECK_SIZE(AXVoiceAdpcm, 0x28);

struct AXVoiceLowPassFilter
{
   uint16_t enabled;
   int16_t previousSample;
   uint16_t a0;
   uint16_t b0;
};
WUT_CHECK_OFFSET(AXVoiceLowPassFilter, 0x0, enabled);
WUT_CHECK_OFFSET(AXVoiceLowPassFilter, 0x2, previousSample);
WUT_CHECK_OFFSET(AXVoiceLowPassFilter, 0x4, a0);
WUT_CHECK_OFFSET(AXVoiceLowPassFilter, 0x6, b0);
WUT_CHECK_SIZE(AXVoiceLowPassFilter, 0x8);

struct AXVoiceBiquadFilter
{
   uint16_t enabled;
   int16_t previousInput0;
   int16_t previousInput1;
   int16_t previousOutput0;
   int16_t previousOutput1;
   int16_t b0;
   int16_t b1;
   int16_t b2;
   int16_t a1;
   int16_t a2;
};
WUT_CHECK_OFFSET(AXVoiceBiquadFilter, 0x0, enabled);
WUT_CHECK_OFFSET(AXVoiceBiquadFilter, 0x2, previousInput0);
WUT_CHECK_OFFSET(AXVoiceBiquadFilter, 0x4, previousInput1);
WUT_CHECK_OFFSET(AXVoiceBiquadFilter, 0x6, previousOutput0);
WUT_CHECK_OFFSET(AXVoiceBiquadFilter, 0x8, previousOutput1);
WUT_CHECK_OFFSET(AXVoiceBiquadFilter, 0xA, b0);
WUT_CHECK_OFFSET(AXVoiceBiquadFilter, 0xC, b1);
WUT_CHECK_OFFSET(AXVoiceBiquadFilter, 0xE, b2);
WUT_CHECK_OFFSET(AXVoiceBiquadFilter, 0x10, a1);
WUT_CHECK_OFFSET(AXVoiceBiquadFilter, 0x12, a2);
WUT_CHECK_SIZE(AXVoiceBiquadFilter, 0x14);

union AXVoiceRemoteIIRFilter
{
   AXVoiceLowPassFilter lpf;
   AXVoiceBiquadFilter biquad;
};
WUT_CHECK_SIZE(AXVoiceRemoteIIRFilter, 0x14);

#pragma pack(push, 1)

/**
 * AXVoice Sample Rate Converter
 */
struct AXVoiceSrc
{
   //! Playback rate, fixed 16.16
   uint32_t ratio;

   //! Used by the resampler, fixed 0.16
   uint16_t currentOffsetFrac;
   int16_t lastSample[4];
};
WUT_CHECK_OFFSET(AXVoiceSrc, 0x0, ratio);
WUT_CHECK_OFFSET(AXVoiceSrc, 0x4, currentOffsetFrac);
WUT_CHECK_OFFSET(AXVoiceSrc, 0x6, lastSample);
WUT_CHECK_SIZE(AXVoiceSrc, 0xe);

struct AXVoiceRemoteSrc
{
   uint16_t currentOffsetFrac;
   int16_t lastSample[4];
};
WUT_CHECK_OFFSET(AXVoiceRemoteSrc, 0x0, currentOffsetFrac);
WUT_CHECK_OFFSET(AXVoiceRemoteSrc, 0x2, lastSample);
WUT_CHECK_SIZE(AXVoiceRemoteSrc, 0x0A);

#pragma pack(pop)

struct AXVoiceMixData
{
   uint16_t left;
   int16_t leftDelta;
   uint16_t right;
   int16_t rightDelta;
   uint16_t auxALeft;
   int16_t auxALeftDelta;
   uint16_t auxARight;
   int16_t auxARightDelta;
   uint16_t auxBLeft;
   int16_t auxBLeftDelta;
   uint16_t auxBRight;
   int16_t auxBRightDelta;
   uint16_t auxCLeft;
   int16_t auxCLeftDelta;
   uint16_t auxCRight;
   int16_t auxCRightDelta;
   uint16_t surround;
   int16_t surroundDelta;
   uint16_t auxASurround;
   int16_t auxASurroundDelta;
   uint16_t auxBSurround;
   int16_t auxBSurroundDelta;
   uint16_t auxCSurround;
   int16_t auxCSurroundDelta;
};
WUT_CHECK_SIZE(AXVoiceMixData, 0x30);

struct AXVoiceDRCMixData
{
   uint16_t left;
   int16_t leftDelta;
   uint16_t right;
   int16_t rightDelta;
   uint16_t auxLeft;
   int16_t auxLeftDelta;
   uint16_t auxRight;
   int16_t auxRightDelta;
   uint16_t surround;
   int16_t surroundDelta;
   uint16_t auxSurround;
   int16_t auxSurroundDelta;
};
WUT_CHECK_SIZE(AXVoiceDRCMixData, 0x18);

struct AXVoiceRemoteMixData
{
   uint16_t main0;
   int16_t main0Delta;
   uint16_t aux0;
   int16_t aux0Delta;
   uint16_t main1;
   int16_t main1Delta;
   uint16_t aux1;
   int16_t aux1Delta;
   uint16_t main2;
   int16_t main2Delta;
   uint16_t aux2;
   int16_t aux2Delta;
   uint16_t main3;
   int16_t main3Delta;
   uint16_t aux3;
   int16_t aux3Delta;
};
WUT_CHECK_SIZE(AXVoiceRemoteMixData, 0x20);

struct AXMultiVoice
{
   BOOL active;
   uint32_t count;
   AXVoice *voices[6];
};
WUT_CHECK_OFFSET(AXMultiVoice, 0x0, active);
WUT_CHECK_OFFSET(AXMultiVoice, 0x4, count);
WUT_CHECK_OFFSET(AXMultiVoice, 0x8, voices);
WUT_CHECK_SIZE(AXMultiVoice, 0x20);

int32_t
AXVoiceBegin(AXVoice *v);

int32_t
AXVoiceEnd(AXVoice *v);

void
AXFreeVoice(AXVoice *voice);

AXVoice *
AXAcquireVoice(uint32_t priority,
               AXVoiceCallbackFn callback,
               void *userContext)
   WUT_FREED_BY(AXFreeVoice, 1);

AXVoice *
AXAcquireVoiceEx(uint32_t priority,
                 AXVoiceCallbackExFn callback,
                 void *userContext)
   WUT_FREED_BY(AXFreeVoice, 1);

BOOL
AXCheckVoiceOffsets(AXVoiceOffsets *offsets);


uint32_t
AXGetMaxVoices();

uint32_t
AXGetVoiceCurrentOffsetEx(AXVoice *voice,
                          const void *samples);

uint32_t
AXGetVoiceLoopCount(AXVoice *voice);

void
AXGetVoiceOffsets(AXVoice *voice,
                  AXVoiceOffsets *offsets);

BOOL
AXIsVoiceRunning(AXVoice *voice);

void
AXSetVoiceSamplesAddr(AXVoice *voice,
                      const void *samples);

void
AXSetVoiceAdpcm(AXVoice *voice,
                AXVoiceAdpcm *adpcm);

void
AXSetVoiceAdpcmLoop(AXVoice *voice,
                    AXVoiceAdpcmLoopData *loopData);

void
AXSetVoiceCurrentOffset(AXVoice *voice,
                        uint32_t offset);

void
AXSetVoiceCurrentOffsetEx(AXVoice *voice,
                          uint32_t offset,
                          const void *samples);

AXResult
AXSetVoiceDeviceMix(AXVoice *voice,
                    AXDeviceType type,
                    uint32_t id,
                    AXVoiceDeviceMixData *mixData);

void
AXSetVoiceMix(AXVoice *voice,
              const AXVoiceMixData *mixData);

void
AXSetVoiceDRCMix(AXVoice *voice,
                 const AXVoiceDRCMixData *mixData);

void
AXSetVoiceEndOffset(AXVoice *voice,
                    uint32_t offset);

void
AXSetVoiceEndOffsetEx(AXVoice *voice,
                      uint32_t offset,
                      const void *samples);

AXResult
AXSetVoiceInitialTimeDelay(AXVoice *voice,
                           uint16_t delay);

void
AXSetVoiceLoopOffset(AXVoice *voice,
                     uint32_t offset);

void
AXSetVoiceLoopOffsetEx(AXVoice *voice,
                       uint32_t offset,
                       const void *samples);

void
AXSetVoiceLoop(AXVoice *voice,
               AXVoiceLoop loop);

void
AXGetVoiceOffsetsEx(AXVoice *voice,
                    AXVoiceOffsets *offsets,
                    const void *samples);

void
AXSetVoiceOffsets(AXVoice *voice,
                  AXVoiceOffsets *offsets);

void
AXSetVoiceOffsetsEx(AXVoice *voice,
                    const AXVoiceOffsets *offsets,
                    const void *samples);

void
AXSetVoicePriority(AXVoice *voice,
                   uint32_t priority);

void
AXSetVoiceItdOn(AXVoice *voice);

void
AXSetVoiceItdTarget(AXVoice *voice,
                    uint16_t leftShift,
                    uint16_t rightShift);

void
AXSetVoiceLpf(AXVoice *voice,
              const AXVoiceLowPassFilter *filter);

void
AXSetVoiceLpfCoefs(AXVoice *voice,
                   uint16_t a0,
                   uint16_t b0);

void
AXSetVoiceBiquad(AXVoice *voice,
                 const AXVoiceBiquadFilter *filter);

void
AXSetVoiceBiquadCoefs(AXVoice *voice,
                      uint16_t b0,
                      uint16_t b1,
                      uint16_t b2,
                      uint16_t a1,
                      uint16_t a2);

void
AXSetVoiceRmtIIRCoefs(AXVoice *voice,
                      uint16_t filter,
                      ...);

void
AXSetVoiceRmtIIR(AXVoice *voice,
                 const AXVoiceRemoteIIRFilter *filter);

void
AXSetVoiceRmtMix(AXVoice *voice,
                 const AXVoiceRemoteMixData *mixData);

void
AXSetVoiceRmtSrc(AXVoice *voice,
                 const AXVoiceRemoteSrc *src);

void
AXSetVoiceSrc(AXVoice *voice,
              AXVoiceSrc *src);

AXVoiceSrcRatioResult
AXSetVoiceSrcRatio(AXVoice *voice,
                   float ratio);

void
AXSetVoiceSrcType(AXVoice *voice,
                  AXVoiceSrcType type);

void
AXSetVoiceState(AXVoice *voice,
                AXVoiceState state);

void
AXSetVoiceType(AXVoice *voice,
               AXVoiceType type);

void
AXSetVoiceVe(AXVoice *voice,
             AXVoiceVeData *veData);

void
AXSetVoiceVeDelta(AXVoice *voice,
                  int16_t delta);

/**
 * Enable generating samples for Wiimotes
 * \param voice voice to generate samples from
 * \param on set TRUE to generate samples for Wiimotes
 *
 * \see AXRmtGetSamples
 */
void
AXSetVoiceRmtOn(AXVoice *voice, uint16_t on);

void
AXComputeLpfCoefs(uint16_t frequency,
                  uint16_t *a0,
                  uint16_t *b0);

void
AXPrepareEfxData(const void *samples,
                 uint32_t size);

AXResult
AXAcquireMultiVoice(uint32_t priority,
                    AXVoiceCallbackExFn callback,
                    void *userContext,
                    AXVoiceAdpcm *adpcm,
                    AXMultiVoice **outVoice);

void
AXFreeMultiVoice(AXMultiVoice *voice);

void
AXSetMultiVoicePriority(AXMultiVoice *voice,
                        uint32_t priority);

void
AXSetMultiVoiceSrcType(AXMultiVoice *voice,
                       AXVoiceSrcType type);

void
AXSetMultiVoiceState(AXMultiVoice *voice,
                     AXVoiceState state);

void
AXSetMultiVoiceType(AXMultiVoice *voice,
                    AXVoiceType type);

void
AXSetMultiVoiceVe(AXMultiVoice *voice,
                  const AXVoiceVeData *veData);

void
AXSetMultiVoiceVeDelta(AXMultiVoice *voice,
                       int16_t delta);

void
AXSetMultiVoiceOffsets(AXMultiVoice *voice,
                       AXVoiceOffsets *offsets);

void
AXSetMultiVoiceLoop(AXMultiVoice *voice,
                    AXVoiceLoop loop);

void
AXSetMultiVoiceAdpcm(AXMultiVoice *voice,
                     AXVoiceAdpcm *adpcm);

void
AXSetMultiVoiceSrc(AXMultiVoice *voice,
                   const AXVoiceSrc *src);

AXVoiceSrcRatioResult
AXSetMultiVoiceSrcRatio(AXMultiVoice *voice,
                        float ratio);

void
AXSetMultiVoiceAdpcmLoop(AXMultiVoice *voice,
                         AXVoiceAdpcmLoopData *loopData);

void
AXSetMultiVoiceLpf(AXMultiVoice *voice,
                   const AXVoiceLowPassFilter *filter);

void
AXSetMultiVoiceLpfCoefs(AXMultiVoice *voice,
                        uint16_t a0,
                        uint16_t b0);

void
AXSetMultiVoiceBiquad(AXMultiVoice *voice,
                      const AXVoiceBiquadFilter *filter);

void
AXSetMultiVoiceBiquadCoefs(AXMultiVoice *voice,
                           uint16_t b0,
                           uint16_t b1,
                           uint16_t b2,
                           uint16_t a1,
                           uint16_t a2);

AXVoiceRenderer
AXSetMultiVoiceRenderer(AXMultiVoice *voice,
                        AXVoiceRenderer renderer);

AXVoiceRenderer
AXGetMultiVoiceRenderer(AXMultiVoice *voice);

void
AXSetMultiVoiceLoopOffset(AXMultiVoice *voice,
                          uint32_t offset);

void
AXSetMultiVoiceEndOffset(AXMultiVoice *voice,
                         uint32_t offset);

void
AXSetMultiVoiceCurrentOffset(AXMultiVoice *voice,
                             uint32_t offset);

void
AXGetMultiVoiceOffsets(AXMultiVoice *voice,
                       AXVoiceOffsets *offsets);

BOOL
AXIsMultiVoiceRunning(AXMultiVoice *voice);

AXResult
AXSetMultiVoiceDeviceMix(AXMultiVoice *voice,
                         AXDeviceType type,
                         uint32_t id,
                         AXBusType bus,
                         uint16_t volume,
                         int16_t delta);

AXResult
AXGetMultiVoiceReformatBufferSize(AXVoiceFormat format,
                                  uint32_t numChannels,
                                  uint32_t numBytes,
                                  uint32_t *size);

#ifdef __cplusplus
}
#endif

/** @} */
