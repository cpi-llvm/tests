#include <stdio.h>
#include <dlfcn.h>

#include "lib.h"

int main(int argc, char **argv)
{
  void *handle = dlopen(SHARED_LIBRARY, RTLD_NOW | RTLD_LOCAL);
  if (!handle) {
    fprintf(stderr, "dlopen(%s): %s\n", SHARED_LIBRARY, dlerror());
    return 1;
  }

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
  return 0;
}
