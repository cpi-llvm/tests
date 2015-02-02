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
  if (pthread_create(&thread, NULL, start, argv[0])) {
    fprintf(stderr, "pthread_create(): %m\n");
    return 1;
  }

  pthread_join(thread, NULL);
  return 0;
}
