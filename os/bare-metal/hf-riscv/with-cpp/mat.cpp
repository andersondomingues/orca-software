//source https://github.com/daedaleanai/libsupcxx/blob/master/tests/test-01-initializers-operators.cc


#include <stdio.h>
#include <stdint.h>
#include <initializer_list>


#ifdef __cplusplus
extern "C" {
#endif

class Matrix {
private:
  __uint32_t matrix_[16];
public:

  ~Matrix() {
    printf("mat dtor1\n");
  }

  Matrix() {
    for(unsigned i = 0; i < 16; ++i) {
      matrix_[i] = 0;
    }
    printf("mat ctor1\n");
  }

  Matrix(std::initializer_list<__uint32_t> init) {
    int i = 0;
    for(auto &el : init) {
      matrix_[i++] = el;
    }
    printf("mat ctor2\n");
  }

  Matrix(const Matrix &rhs) {
    for(unsigned i = 0; i < 15; ++i) {
      matrix_[i] = rhs.matrix_[i];
    }
    printf("mat ctor3\n");
  }
  __uint32_t operator() (unsigned int i, unsigned int j) const {
    return matrix_[i*4+j];
  }

  __uint32_t &operator() (unsigned int i, unsigned int j) {
    return matrix_[i*4+j];
  }

  Matrix &operator=(const Matrix &rhs) {
    for(unsigned i = 0; i < 16; ++i) {
      matrix_[i] = rhs.matrix_[i];
    }
    return *this;
  }
};

Matrix operator * (const Matrix &lhs, const Matrix &rhs) {
  Matrix m;
  for(unsigned i = 0; i < 4; ++i) {
    for(unsigned j = 0; j < 4; ++j) {
      m(i, j) = 0;
      for(unsigned k = 0; k < 4; ++k) {
        m(i, j) += lhs(i, k) * rhs(k, j);
      }
    }
  }
  return m;
}

Matrix operator + (const Matrix &lhs, const Matrix &rhs) {
  Matrix m;
  for(unsigned i = 0; i < 4; ++i) {
    for(unsigned j = 0; j < 4; ++j) {
        m(i, j) = lhs(i, j) + rhs(i, j);
    }
  }
  return m;
}


void printMatrix(const Matrix &a) {
  for (size_t i = 0; i < 4; ++i) {
    for(size_t j = 0; j < 4; ++j) {
      printf("%d ", a(i, j));
    }
    printf("\n");
  }
}

extern unsigned	_text_size;
extern unsigned	_bss_size;
extern unsigned	_heap_size;
extern unsigned	_stack_size;
extern unsigned	_data_size;
extern unsigned	_init_array_size;
extern unsigned	_preinit_array_size;
extern unsigned	_fini_array_size;

   Matrix d = {
      1,  2,  3,  4,
      5,  6,  7,  8,
      9, 10, 11, 12,
     13, 14, 15, 16
   };

int main() {

  uintptr_t	text_size = (uintptr_t)&_text_size;
  uintptr_t	bss_size = (uintptr_t)&_bss_size;
  uintptr_t	heap_size = (uintptr_t)&_heap_size;
  uintptr_t	stack_size = (uintptr_t)&_stack_size;
  uintptr_t	data_size = (uintptr_t)&_data_size;
  uintptr_t	init_array_size = (uintptr_t)&_init_array_size;
  uintptr_t	preinit_array_size = (uintptr_t)&_preinit_array_size;
  uintptr_t	fini_array_size = (uintptr_t)&_fini_array_size;

  printf("Memory sizes (bytes):\n-text: %d\n-data: %d\n-bss: %d\n-heap: %d\n-stack: %d\n-preinit: %d\n-init: %d\n-fini: %d\n",
    text_size,data_size,bss_size,heap_size,stack_size,preinit_array_size,init_array_size,fini_array_size);

  // it should be initialized before the main
  printf("d \n");
  printMatrix(d);

  Matrix a = {
     1,  2,  3,  4,
     5,  6,  7,  8,
     9, 10, 11, 12,
    13, 14, 15, 16
  };
  printf("a =\n");
  printMatrix(a);

  Matrix b = {
     2,  3,  4,  5,
     6,  7,  8,  9,
    10, 11, 12, 13,
    14, 15, 16, 17
  };
  printf("b =\n");
  printMatrix(b);

  Matrix c = a * b;
  printf("a * b =\n");
  printMatrix(c);

  c = a + b;
  printf("a + b =\n");
  printMatrix(c);

return 0;

}

#ifdef __cplusplus
}
#endif
