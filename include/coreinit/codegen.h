#pragma once

#include <wut.h>

/**
 * \defgroup coreinit_codegen Codegen
 * \ingroup coreinit
 *
 * Controls a memory area that can switch between writable and
 * executable states, allowing dynamic code generation.
 * For some applications, a codegen area is avilable for things like JIT or
 * otherwise generating CPU instructions on the fly. Enabled apps can use
 * \link OSCodegenGetVirtAddrRange \endlink to discover this area, then use
 * \link OSSwitchSecCodeGenMode \endlink or \link OSCodegenCopy \endlink to
 * move instructions in and out before executing them. Codegen can only be used
 * from a specific, predetermined CPU core - see
 * \link OSGetCodegenCore \endlink. Codegen can only be used when enabled in an
 * app's cos.xml.
 *
 * <!-- TODO mark up cos.xml permission requirements, figure out sizing -->
 * <!--
 *  sources:
 *  https://web.archive.org/web/20190327131828/https://wiiubrew.org/wiki/Coreinit.rpl#Codegen
 *  https://web.archive.org/web/20190421120656/https://github.com/decaf-emu/decaf-emu/blob/master/src/libdecaf/src/cafe/libraries/coreinit/coreinit_codegen.cpp
 * -->
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

//! The memory permissions for the codegen area.
typedef enum OSCodegenSecMode
{
   //! The area can be read or written to, but not executed.
   CODEGEN_RW_ = 0,
   //! The area can be read or executed, but not written to.
   CODEGEN_R_X = 1,
} OSCodegenSecMode;

/**
 * Gets the location and size of codegen memory, if available.
 *
 * \param outVirtualAddress
 * Pointer to write the virtual address of the codegen area to. Will write 0 if
 * codegen isn't available.
 *
 * \param outSize
 * Pointer to write the size of the codegen area to. Will write 0 if codegen
 * isn't available.
 *
 * \sa
 * - \link OSGetSecCodeGenMode \endlink
 */
void
OSGetCodegenVirtAddrRange(void **outVirtualAddress,
                          uint32_t *outSize);

/**
 * Gets the CPU core that's allowed to use codegen.
 *
 * \return
 * The core id of the core that can use codegen.
 *
 * \sa
 * - \link OSGetCoreId \endlink
 *
 * <!-- NOTE this appears to come from cos.xml: bottom few bits -->
 */
uint32_t
OSGetCodegenCore();

uint32_t
OSGetCodegenMode();

/**
 * Switches the permissions on the codegen memory area. Use this function to
 * move between writing code and executing it.
 *
 * \param mode
 * The memory permission mode to use - one of \link OSCodegenSecMode \endlink.
 *
 * The application must enable explicit codegen mode switching, and the calling
 * thread must run on the core returned by \link OSGetCodegenCore \endlink.
 * Otherwise, this function leaves the current mode unchanged.
 * Use \link OSGetSecCodeGenMode \endlink to query the resulting mode.
 */
void
OSSwitchSecCodeGenMode(OSCodegenSecMode mode);

/**
 * Queries the execution state of a switchable codegen area on the calling core.
 *
 * \return
 * 1 when explicit codegen mode switching is enabled, the calling thread runs on
 * the designated codegen core, and the area is in \link CODEGEN_R_X \endlink
 * mode. Returns 0 otherwise, including writable mode, another core, unavailable
 * codegen, or copy-only codegen mode.
 *
 * This does not report general codegen availability. Use
 * \link OSGetCodegenVirtAddrRange \endlink for that purpose.
 */
uint32_t
OSGetSecCodeGenMode();

/**
 * Copies data from normal memory into the codegen area, leaving the area in
 * \link CODEGEN_R_X \endlink mode.
 * This function copies into the codegen area regardless
 * of the current permission status by switching modes as required for the copy.
 *
 * \param dst
 * The starting virtual address of the area in codegen memory to copy to. Must
 * be a valid pointer
 *
 * \param src
 * The starting virtual address of the source to copy from. Must be a valid
 * pointer.
 *
 * \param size
 * The size of the data to copy. Must not be 0, and must be small
 * enough such that <tt>dst + size</tt> does not pass the end of the codegen
 * area (see \link OSCodegenGetVirtAddrRange \endlink).
 *
 * \return
 * \c TRUE on success, indicating the codegen area is now in
 * \link CODEGEN_R_X \endlink, or \c FALSE on an error, such as invalid pointers
 * or codegen not being available.
 *
 * <!-- NOTE this function has a specific bit required in cox.xml: 1 << 30? -->
 */
BOOL
OSCodegenCopy(void *dst,
              void *src,
              size_t size);

#ifdef __cplusplus
}
#endif

/** @} */
