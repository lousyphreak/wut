#pragma once
#include <wut.h>

/**
 * \defgroup sndcore2_result Result
 * \ingroup sndcore2
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

//! A value from enum AX_RESULT.
typedef int32_t AXResult;

enum AX_RESULT
{
   AX_RESULT_SUCCESS                            = 0,
   AX_RESULT_INVALID_DEVICE_TYPE                = -1,
   AX_RESULT_INVALID_DEVICE_ID                  = -2,
   AX_RESULT_NULL_MIX_POINTER                   = -3,
   AX_RESULT_NULL_VOICE_POINTER                 = -4,
   AX_RESULT_INVALID_AUX_ID                     = -5,
   AX_RESULT_ILLEGAL_COMPRESSOR_SETTINGS        = -6,
   AX_RESULT_ILLEGAL_REMIX_INPUT_CHANNEL_COUNT  = -7,
   AX_RESULT_ILLEGAL_REMIX_OUTPUT_CHANNEL_COUNT = -8,
   AX_RESULT_REMIX_EXCEEDS_MAX_ALLOWED          = -9,
   AX_RESULT_REMIX_MATRIX_UNINITIALIZED         = -10,
   AX_RESULT_FINAL_OUTPUT_BUFFER_TOO_SMALL      = -11,
   AX_RESULT_INVALID_DRC_VS_CONFIG              = -12,
   AX_RESULT_INVALID_DRC_VS_MODE                = -13,
   AX_RESULT_FRAME_CALLBACK_TRY_AGAIN           = -14,
   AX_RESULT_FRAME_CALLBACK_MAX_DEPTH_REACHED   = -15,
   AX_RESULT_FRAME_CALLBACK_NOT_FOUND           = -16,
   AX_RESULT_FRAME_CALLBACK_NULL_FUNCTION       = -17,
   AX_RESULT_VOICE_IS_RUNNING                   = -18,
   AX_RESULT_DELAY_TOO_BIG                      = -19,
   AX_RESULT_MULTI_VOICE_COUNT_EXCEEDED         = -20,
   AX_RESULT_MULTI_VOICE_INVALID_CHANNEL_COUNT  = -21,
   AX_RESULT_MULTI_VOICE_UNABLE_TO_ALLOCATE_ALL = -22,
   AX_RESULT_MULTI_VOICE_INVALID_FORMAT         = -23,
};

#ifdef __cplusplus
}
#endif

/** @} */
