#include "wut_newlib.h"
#include "wut_thread_specific.h"

#include <coreinit/dynload.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>

struct WUTThreadKey
{
   uint32_t key;
   void (*destructor)(void *);
};

struct WUTThreadValue
{
   uint32_t key;
   const void *value;
};

static struct WUTThreadKey sKeys[__WUT_MAX_KEYS];
static uint32_t sNextKey = __WUT_MAX_KEYS;
static _LOCK_T sKeyLock;

int
__SYSCALL(tls_create)(uint32_t *key, void (*destructor)(void *))
{
   int result = EAGAIN;
   __SYSCALL(lock_acquire)(&sKeyLock);
   for (uint32_t index = 0; index < __WUT_MAX_KEYS; ++index) {
      if (!sKeys[index].key) {
         *key = sNextKey | index;
         sNextKey += __WUT_MAX_KEYS;
         sKeys[index].key = *key;
         sKeys[index].destructor = destructor;
         result = 0;
         break;
      }
   }
   __SYSCALL(lock_release)(&sKeyLock);
   return result;
}

int
__SYSCALL(tls_delete)(uint32_t key)
{
   __SYSCALL(lock_acquire)(&sKeyLock);
   sKeys[key & (__WUT_MAX_KEYS - 1)].key = 0;
   __SYSCALL(lock_release)(&sKeyLock);
   return 0;
}

void *
__SYSCALL(tls_get)(uint32_t key)
{
   struct WUTThreadValue *values = wut_get_thread_specific(__WUT_KEY_THREAD_SPECIFIC_ID);
   if (!values) {
      return NULL;
   }
   struct WUTThreadValue *entry = &values[key & (__WUT_MAX_KEYS - 1)];
   return entry->key == key ? (void *)entry->value : NULL;
}

int
__SYSCALL(tls_set)(uint32_t key, const void *value)
{
   struct WUTThreadValue *values = wut_get_thread_specific(__WUT_KEY_THREAD_SPECIFIC_ID);
   if (!values) {
      values = calloc(__WUT_MAX_KEYS, sizeof(*values));
      if (!values) {
         return ENOMEM;
      }
      wut_set_thread_specific(__WUT_KEY_THREAD_SPECIFIC_ID, values);
   }
   values[key & (__WUT_MAX_KEYS - 1)] = (struct WUTThreadValue){key, value};
   return 0;
}

void
__wut_tls_key_cleanup(void)
{
   struct WUTThreadValue *values = wut_get_thread_specific(__WUT_KEY_THREAD_SPECIFIC_ID);
   if (!values) {
      return;
   }
   for (int pass = 0; pass < PTHREAD_DESTRUCTOR_ITERATIONS; ++pass) {
      for (uint32_t index = 0; index < __WUT_MAX_KEYS; ++index) {
         __SYSCALL(lock_acquire)(&sKeyLock);
         struct WUTThreadKey key = sKeys[index];
         __SYSCALL(lock_release)(&sKeyLock);
         if (key.key && key.key == values[index].key && key.destructor && values[index].value) {
            void *value = (void *)values[index].value;
            values[index].value = NULL;
            key.destructor(value);
         }
      }
   }
   wut_set_thread_specific(__WUT_KEY_THREAD_SPECIFIC_ID, NULL);
   free(values);
}

struct WUTTLSDestructor
{
   void (*destructor)(void *);
   void *object;
   struct WUTTLSDestructor *next;
   OSDynLoad_Module module;
};

static pthread_key_t sDestructorKey;
static pthread_once_t sDestructorOnce = PTHREAD_ONCE_INIT;

static void
__wut_run_tls_destructors(void *head)
{
   pthread_setspecific(sDestructorKey, head);
   struct WUTTLSDestructor *entry;
   while ((entry = pthread_getspecific(sDestructorKey))) {
      pthread_setspecific(sDestructorKey, entry->next);
      entry->destructor(entry->object);
      OSDynLoad_Module module = entry->module;
      free(entry);
      if (module) {
         OSDynLoad_Release(module);
      }
   }
}

static void
__wut_init_tls_destructors(void)
{
   pthread_key_create(&sDestructorKey, __wut_run_tls_destructors);
}

int
__cxa_thread_atexit(void (*destructor)(void *), void *object, void *dso)
{
   pthread_once(&sDestructorOnce, __wut_init_tls_destructors);
   struct WUTTLSDestructor *entry = malloc(sizeof(*entry));
   if (!entry) {
      return -1;
   }
   OSDynLoad_Module module = NULL;
   if (dso && OSDynLoad_AcquireContainingModule(dso, OS_DYNLOAD_SECTION_DATA, &module)) {
      free(entry);
      return -1;
   }
   *entry = (struct WUTTLSDestructor){destructor, object, pthread_getspecific(sDestructorKey), module};
   int result = pthread_setspecific(sDestructorKey, entry);
   if (result) {
      free(entry);
      if (module) {
         OSDynLoad_Release(module);
      }
   }
   return result;
}

void __real___call_exitprocs(int status, void *dso);

void
__wrap___call_exitprocs(int status, void *dso)
{
   // Language TLS is destroyed before static objects and atexit callbacks.
   if (!dso) {
      void *head = pthread_getspecific(sDestructorKey);
      if (head) {
         __wut_run_tls_destructors(head);
      }
   }
   __real___call_exitprocs(status, dso);
}
