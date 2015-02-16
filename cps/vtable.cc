struct A {
  virtual int fct() { return 0; }
};

struct B : public A {
  virtual int fct() { return 42; }
};


int main()
{
  B b;
  A *a = &b;

  return a->fct();
}
