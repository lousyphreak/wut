#pragma once
#include <wut.h>
#include <sndcore2/voice.h>

/**
 * \defgroup snduser2_axart AXART
 * \ingroup snduser2
 *
 * Sound lifecycle and positional articulators.
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AXARTArticulator AXARTArticulator;
typedef struct AXART3D AXART3D;
typedef struct AXARTPanning AXARTPanning;
typedef struct AXARTSrcType AXARTSrcType;
typedef struct AXARTItd AXARTItd;
typedef struct AXARTLpf AXARTLpf;
typedef struct AXARTSound AXARTSound;

//! A value from enum AXART_ARTICULATOR_TYPE.
typedef uint32_t AXARTArticulatorType;

//! A value from enum AXART_LPF_FREQUENCY.
typedef uint32_t AXARTLpfFrequency;

enum AXART_ARTICULATOR_TYPE
{
   AXART_TYPE_3D              = 1,
   AXART_TYPE_PANNING         = 2,
   AXART_TYPE_ITD             = 3,
   AXART_TYPE_SRC_TYPE        = 4,
   AXART_TYPE_PITCH           = 5,
   AXART_TYPE_PITCH_ENV       = 6,
   AXART_TYPE_PITCH_MOD       = 7,
   AXART_TYPE_VOLUME          = 8,
   AXART_TYPE_AUXA_VOLUME     = 9,
   AXART_TYPE_AUXB_VOLUME     = 10,
   AXART_TYPE_AUXC_VOLUME     = 11,
   AXART_TYPE_VOLUME_ENV      = 12,
   AXART_TYPE_AUXA_VOLUME_ENV = 13,
   AXART_TYPE_AUXB_VOLUME_ENV = 14,
   AXART_TYPE_AUXC_VOLUME_ENV = 15,
   AXART_TYPE_VOLUME_MOD      = 16,
   AXART_TYPE_AUXA_VOLUME_MOD = 17,
   AXART_TYPE_AUXB_VOLUME_MOD = 18,
   AXART_TYPE_AUXC_VOLUME_MOD = 19,
   AXART_TYPE_LPF             = 20,
   AXART_TYPE_FADER           = 21,
   AXART_TYPE_RMT             = 22,
   AXART_TYPE_RMT_FADER       = 24,
   AXART_TYPE_RMT_AUX_VOLUME  = 25,
   AXART_TYPE_DRC             = 26,
};

enum AXART_LPF_FREQUENCY
{
   AXART_LPF_FREQ_16000_HZ = 0,
   AXART_LPF_FREQ_12800_HZ = 1,
   AXART_LPF_FREQ_10240_HZ = 2,
   AXART_LPF_FREQ_8000_HZ  = 3,
   AXART_LPF_FREQ_6400_HZ  = 4,
   AXART_LPF_FREQ_5120_HZ  = 5,
   AXART_LPF_FREQ_4000_HZ  = 6,
   AXART_LPF_FREQ_3200_HZ  = 7,
   AXART_LPF_FREQ_2560_HZ  = 8,
   AXART_LPF_FREQ_2000_HZ  = 9,
   AXART_LPF_FREQ_1600_HZ  = 10,
   AXART_LPF_FREQ_1280_HZ  = 11,
   AXART_LPF_FREQ_1000_HZ  = 12,
   AXART_LPF_FREQ_800_HZ   = 13,
   AXART_LPF_FREQ_640_HZ   = 14,
   AXART_LPF_FREQ_500_HZ   = 15,
   AXART_LPF_FREQ_400_HZ   = 16,
   AXART_LPF_FREQ_320_HZ   = 17,
   AXART_LPF_FREQ_256_HZ   = 18,
   AXART_LPF_FREQ_200_HZ   = 19,
   AXART_LPF_FREQ_160_HZ   = 20,
   AXART_LPF_FREQ_128_HZ   = 21,
   AXART_LPF_FREQ_100_HZ   = 22,
   AXART_LPF_FREQ_80_HZ    = 23,
};

struct AXARTArticulator
{
   AXARTArticulator *next;
   AXARTArticulatorType type;
};
WUT_CHECK_OFFSET(AXARTArticulator, 0x00, next);
WUT_CHECK_OFFSET(AXARTArticulator, 0x04, type);
WUT_CHECK_SIZE(AXARTArticulator, 0x08);

struct AXART3D
{
   AXARTArticulator art;

   float horizontalAngle;
   float verticalAngle;
   float distance;
   float closingSpeed;
   BOOL update;

   uint8_t pan;
   uint8_t span;
   uint8_t srcType;
   uint16_t leftDelay;
   uint16_t rightDelay;
   float pitch;
   int32_t attenuation;
};
WUT_CHECK_OFFSET(AXART3D, 0x00, art);
WUT_CHECK_OFFSET(AXART3D, 0x08, horizontalAngle);
WUT_CHECK_OFFSET(AXART3D, 0x0C, verticalAngle);
WUT_CHECK_OFFSET(AXART3D, 0x10, distance);
WUT_CHECK_OFFSET(AXART3D, 0x14, closingSpeed);
WUT_CHECK_OFFSET(AXART3D, 0x18, update);
WUT_CHECK_OFFSET(AXART3D, 0x1C, pan);
WUT_CHECK_OFFSET(AXART3D, 0x1D, span);
WUT_CHECK_OFFSET(AXART3D, 0x1E, srcType);
WUT_CHECK_OFFSET(AXART3D, 0x20, leftDelay);
WUT_CHECK_OFFSET(AXART3D, 0x22, rightDelay);
WUT_CHECK_OFFSET(AXART3D, 0x24, pitch);
WUT_CHECK_OFFSET(AXART3D, 0x28, attenuation);
WUT_CHECK_SIZE(AXART3D, 0x2C);

struct AXARTPanning
{
   AXARTArticulator art;

   uint8_t pan;
   uint8_t span;
};
WUT_CHECK_OFFSET(AXARTPanning, 0x00, art);
WUT_CHECK_OFFSET(AXARTPanning, 0x08, pan);
WUT_CHECK_OFFSET(AXARTPanning, 0x09, span);
WUT_CHECK_SIZE(AXARTPanning, 0x0C);

struct AXARTSrcType
{
   AXARTArticulator art;

   uint8_t srcType;
};
WUT_CHECK_OFFSET(AXARTSrcType, 0x00, art);
WUT_CHECK_OFFSET(AXARTSrcType, 0x08, srcType);
WUT_CHECK_SIZE(AXARTSrcType, 0x0C);

struct AXARTItd
{
   AXARTArticulator art;

   uint16_t leftDelay;
   uint16_t rightDelay;
};
WUT_CHECK_OFFSET(AXARTItd, 0x00, art);
WUT_CHECK_OFFSET(AXARTItd, 0x08, leftDelay);
WUT_CHECK_OFFSET(AXARTItd, 0x0A, rightDelay);
WUT_CHECK_SIZE(AXARTItd, 0x0C);

struct AXARTLpf
{
   AXARTArticulator art;
   BOOL initializeFilter;
   AXARTLpfFrequency frequency;
   BOOL update;
};
WUT_CHECK_OFFSET(AXARTLpf, 0x00, art);
WUT_CHECK_OFFSET(AXARTLpf, 0x08, initializeFilter);
WUT_CHECK_OFFSET(AXARTLpf, 0x0C, frequency);
WUT_CHECK_OFFSET(AXARTLpf, 0x10, update);
WUT_CHECK_SIZE(AXARTLpf, 0x14);

struct AXARTSound
{
   AXARTSound *next;
   AXARTSound *prev;
   AXVoice *voice;
   float sampleRate;
   AXARTArticulator *articulators;
};
WUT_CHECK_OFFSET(AXARTSound, 0x00, next);
WUT_CHECK_OFFSET(AXARTSound, 0x04, prev);
WUT_CHECK_OFFSET(AXARTSound, 0x08, voice);
WUT_CHECK_OFFSET(AXARTSound, 0x0C, sampleRate);
WUT_CHECK_OFFSET(AXARTSound, 0x10, articulators);
WUT_CHECK_SIZE(AXARTSound, 0x14);

void
AXARTInit(void);

void
AXARTQuit(void);

void
AXARTServiceSounds(void);

void
AXARTInitSound(AXARTSound *sound,
               AXVoice *voice,
               uint32_t sampleRate);

void
AXARTAddSound(AXARTSound *sound);

void
AXARTRemoveSound(AXARTSound *sound);

void
AXARTSet3DDistanceScale(float scale);

void
AXARTSet3DDopplerScale(float scale);

void
AXARTAddArticulator(AXARTSound *sound,
                    AXARTArticulator *articulator);

BOOL
AXARTRemoveArticulator(AXARTSound *sound,
                       AXARTArticulator *articulator);

void
AXARTInitArt3D(AXART3D *articulator);

void
AXARTInitArtPanning(AXARTPanning *articulator);

void
AXARTInitArtItd(AXARTItd *articulator);

void
AXARTInitArtSrctype(AXARTSrcType *articulator);

void
AXARTInitArtLpf(AXARTLpf *articulator);

#ifdef __cplusplus
}
#endif

/** @} */
