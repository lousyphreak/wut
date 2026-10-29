#include <coreinit/dynload.h>

void __init(void);
void __fini(void);
void __cxa_finalize(void *dso) __attribute__((weak));
extern void *__dso_handle;

static void
__wut_rpl_fini(void)
{
   if (__cxa_finalize) {
      __cxa_finalize(__dso_handle);
   }
   __fini();
}

int __attribute__((section(".crt0")))
__rpl_start(OSDynLoad_Module module, OSDynLoad_EntryReason reason)
{
   if (reason == OS_DYNLOAD_LOADED) {
      __init();
   }

   int result = rpl_entry(module, reason);
   if (reason == OS_DYNLOAD_UNLOADED || result) {
      __wut_rpl_fini();
   }
   return result;
}

int __attribute__((weak))
rpl_entry(OSDynLoad_Module module, OSDynLoad_EntryReason reason)
{
   return 0;
}
