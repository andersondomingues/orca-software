
// https://raw.githubusercontent.com/daedaleanai/libsupcxx/master/tests/test-03-heap.cc

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

class IInterface {
public:
  IInterface() {
    printf("IInterface constructor called\n");
  }
  virtual ~IInterface() {
    printf("IInterface destructor called\n");
  }
  virtual void doStuff() = 0;
};

class Impl1: public IInterface {
private:
  int a_;
public:
  Impl1(int a = 12) : IInterface() {
    printf("Impl1 constructor called\n");
    a_ = a;
  }
  virtual ~Impl1() {
    printf("Impl1 destructor called\n");
  }
  virtual void doStuff() override {
    printf("Do stuff #1\n");
  }
};

class Impl2: public IInterface {
public:
  Impl2() : IInterface(){
    printf("Impl2 constructor called\n");
  }
  virtual ~Impl2() {
    printf("Impl2 destructor called\n");
  }
  virtual void doStuff() override {
    printf("Do stuff #2\n");
  }
};

void main(const char *cmdline) {
  char *tmpc = (char*)malloc(12);
  free(tmpc);

  double *tmpd = new double[12];
  delete [] tmpd;

  printf("--- Object construct->destruct-parent ---\n");
  IInterface *a = new Impl1(42);
  IInterface *b = new Impl2;

  delete a;
  delete b;

  printf("--- Object array construct->destruct-parent ---\n");
  IInterface *interfaces = new Impl2[2];
  delete [] interfaces;

  printf("--- Object construct->destruct-parent (nothrow) ---\n");
  a = new Impl1(42);
  b = new Impl2;

  delete a;
  delete b;

  printf("--- Object array construct->destruct-parent (nothrow) ---\n");
  interfaces = new Impl2[2];
  delete [] interfaces;

  printf("--- Large array of basic types->destruct ---\n");
  uint64_t *large = new uint64_t[10];
  printf("Address: %p\n", large);

  for(size_t i = 0; i < 10; ++i) {
    large[i] = 0;
  }
  printf("Cleared up the memory\n");
  delete [] large;
}

#ifdef __cplusplus
}
#endif
