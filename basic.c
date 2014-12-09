#include <stdio.h>

int main(int argc, char **argv)
{
  char buffer[1024];

  snprintf(buffer, sizeof (buffer), "Hello %s\n", argv[0]);
  printf("%s", buffer);
  return 0;
}
