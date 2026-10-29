.extern __preinit_user

.section .fexports, "ax", @0x80000001
.align 4
.global __wut_rpx_exports
__wut_rpx_exports:
.long 1
.long 0xa7fef0c2
.long __preinit_user
.long 0x10
.string "__preinit_user"
