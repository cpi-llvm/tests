#include <stdio.h>
#include <pthread.h>

#include "lib.h"

static void *start(void *str)
{
  char buffer[1024];

  snprintf(buffer, sizeof (buffer), "Hello %s\n", str);
  printf("%s", buffer);
  return NULL;
}

void *hello(void *str)
{
  pthread_t thread;
  if (pthread_create(&thread, NULL, start, str)) {
    fprintf(stderr, "pthread_create(): %m\n");
    return NULL;
  }

  pthread_join(thread, NULL);
  return NULL;
}
