#pragma once
#include <wut.h>
#include "result.h"

/**
 * \defgroup sndcore2_drcvs DRC VS
 * \ingroup sndcore2
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

//! A value from enum AX_DRC_VS_MODE.
typedef uint32_t AXDRCVSMode;

//! A value from enum AX_DRC_VS_OUTPUT.
typedef uint32_t AXDRCVSOutput;

//! A value from enum AX_DRC_VS_LC.
typedef uint32_t AXDRCVSLC;

//! A value from enum AX_DRC_VS_SPEAKER_POS.
typedef uint32_t AXDRCVSSpeakerPosition;

//! A value from enum AX_DRC_VS_SURROUND_GAIN.
typedef uint32_t AXDRCVSSurroundLevelGain;

enum AX_DRC_VS_MODE
{
   AX_DRC_VS_MODE_OFF             = 0,
   AX_DRC_VS_MODE_ON_NORMAL       = 1,
   AX_DRC_VS_MODE_ON_FRONT_BYPASS = 2,
};

enum AX_DRC_VS_OUTPUT
{
   AX_DRC_VS_OUTPUT_SPEAKER   = 0,
   AX_DRC_VS_OUTPUT_HEADPHONE = 1,
};

enum AX_DRC_VS_LC
{
   AX_DRC_VS_LC_NOT_USE = 0,
   AX_DRC_VS_LC_USE     = 1,
   AX_DRC_VS_LC_USE_MAX = 2,
};

enum AX_DRC_VS_SPEAKER_POS
{
   AX_DRC_VS_SPEAKER_POS_WIDE   = 0,
   AX_DRC_VS_SPEAKER_POS_SQUARE = 1,
};

enum AX_DRC_VS_SURROUND_GAIN
{
   AX_DRC_VS_SURROUND_GAIN_1_0   = 0,
   AX_DRC_VS_SURROUND_GAIN_0_707 = 1,
   AX_DRC_VS_SURROUND_GAIN_0_5   = 2,
};

AXResult
AXGetDRCVSMode(AXDRCVSMode *mode);

AXResult
AXSetDRCVSMode(AXDRCVSMode mode);

AXResult
AXSetDRCVSDownmixBalance(AXDRCVSOutput output,
                         float balance);

AXResult
AXSetDRCVSLC(AXDRCVSLC lc);

AXResult
AXSetDRCVSLimiter(BOOL limit);

AXResult
AXSetDRCVSLimiterThreshold(float threshold);

AXResult
AXSetDRCVSOutputGain(AXDRCVSOutput output,
                     float gain);

AXResult
AXSetDRCVSSpeakerPosition(AXDRCVSOutput output,
                          AXDRCVSSpeakerPosition pos);

AXResult
AXSetDRCVSSurroundDepth(AXDRCVSOutput output,
                        float depth);

AXResult
AXSetDRCVSSurroundLevelGain(AXDRCVSSurroundLevelGain gain);

#ifdef __cplusplus
}
#endif

/** @} */
