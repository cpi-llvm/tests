#include <stdio.h>

#include "lib.h"

void *hello(void *str)
{
  char buffer[1024];

  snprintf(buffer, sizeof (buffer), "Hello %s\n", str);
  printf("%s", buffer);
  return NULL;
}
