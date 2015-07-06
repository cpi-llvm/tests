#include <stdio.h>
#include <dlfcn.h>
#include <pthread.h>
#include <unistd.h>

#include "lib.h"

pthread_mutex_t mutex;

static void *start(void *str)
{
  char buffer[1024];

  snprintf(buffer, sizeof (buffer), "thread in %s\n", str);
  printf("%s", buffer);
  pthread_mutex_lock(&mutex);
  return NULL;
}

int main(int argc, char **argv)
{
  char buffer[1024];
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_lock(&mutex);

  pthread_t thread;
  if (pthread_create(&thread, NULL, start, argv[0])) {
    fprintf(stderr, "pthread_create(): %m\n");
    return 1;
  }

  void *handle = dlopen(SHARED_LIBRARY, RTLD_NOW | RTLD_LOCAL);
  if (!handle) {
    fprintf(stderr, "dlopen(%s): %s\n", SHARED_LIBRARY, dlerror());
  } else {
    union {
      void *addr;
      void *(*fn)(void *);
    } sym;

    sym.addr = dlsym(handle, "hello");
    if (!sym.addr) {
      fprintf(stderr, "%s: symbol 'hello' not found\n", SHARED_LIBRARY);
      dlclose(handle);
      return 1;
    }

    sym.fn(argv[0]);

    dlclose(handle);
  }

  pthread_mutex_unlock(&mutex);
  pthread_join(thread, NULL);
  pthread_mutex_destroy(&mutex);
  return 0;
}
