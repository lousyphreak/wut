#include <coreinit/dynload.h>
#include "wut_module.h"

void __init(void);
void __fini(void);
void __cxa_finalize(void *dso) __attribute__((weak));
int __cxa_atexit(void (*function)(void *), void *argument, void *dso);
extern void *__dso_handle;
extern const WUTModuleImportTable *__wut_module_imports __attribute__((weak));

int
atexit(void (*function)(void))
{
   return __cxa_atexit((void (*)(void *))function, 0, __dso_handle);
}

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
      if (&__wut_module_imports) {
         int (*bindImports)(const WUTModuleImport *, uint32_t);
         int result = OSDynLoad_FindExport((OSDynLoad_Module)-1, OS_DYNLOAD_EXPORT_FUNC,
                                          "__wut_bind_imports_v1", (void **)&bindImports);
         if (result) {
            return result;
         }
         result = bindImports(__wut_module_imports->imports, __wut_module_imports->count);
         if (result) {
            return result;
         }
      }
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
