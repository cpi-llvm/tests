#include <stdio.h>
#include <pthread.h>

static void *start(void *str)
{
  char buffer[1024];

  snprintf(buffer, sizeof (buffer), "Hello %s\n", str);
  printf("%s", buffer);
  return NULL;
}

int main(int argc, char **argv)
{
  pthread_t thread;
  pthread_attr_t attr;

  static char stack[1 << 14];

  if (pthread_attr_init(&attr)) {
    fprintf(stderr, "pthread_attr_init(): %m\n");
    return 1;
  }

  if (pthread_attr_setstack(&attr, stack, sizeof (stack))) {
    fprintf(stderr, "pthread_attr_setstack(): %m\n");
    return 1;
  }

  if (pthread_create(&thread, &attr, start, argv[0])) {
    fprintf(stderr, "pthread_create(): %m\n");
    return 1;
  }

  pthread_join(thread, NULL);
  pthread_attr_destroy(&attr);
  return 0;
}
