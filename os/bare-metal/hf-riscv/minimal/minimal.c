volatile int my_init_global = 1;
volatile int my_uninit_global;

int main() {
  volatile int my_init_local = 2;
  volatile int my_uninit_local;
  volatile const int my_const = 3;
  volatile const char my_str[] = "I love embedded sw !!!!";
}


