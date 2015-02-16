typedef int (*fct_t)(void);

int fct0(void)
{
  return 0;
}

int fct1(void)
{
  return 1;
}

int fct2(void)
{
  return 2;
}

static const fct_t fcts[] = {
  fct0,
  fct1,
  fct2,
};


int main(int argc, char **argv)
{
  fct_t fct = fcts[argc];
  return fct();
}
