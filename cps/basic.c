#include <stdio.h>

typedef char *(*stringer_f)(int, char **);

struct my_test {
  int a;
  float b;
  double c;
  char x[24];
  stringer_f s;
};

char *
the_stringer(int argc, char **argv)
{
  return argv[argc / 2];
}

void
make_stringer(stringer_f *stringer)
{
  *stringer = the_stringer;
}

int main(int argc, char **argv)
{
  struct my_test test;
  stringer_f stringer;
  make_stringer(&stringer);
  puts(stringer(argc, argv));
  return 0;
}
