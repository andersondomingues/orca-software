
Compiling 
--- 

This will compile a simple test app using hf-printf.f using ORCA with single-core plataform and bare-metal OS.

```bash
make -f Makefile-Orca clean all V=1
```

Running
--- 

Then, run the following commands to test it.

```bash
mkdir logs
single-core.exe image.bin
cat logs/core\!.cpu_debug.log
```

The expected output is :

```bash
Hello!!!!


TESTING int:  1 + 2 = 3
TESTING str: C eh taaaaao legal !!! !!!!
TESTING float str: 3.2999 !!!!
TESTING float:  !!!!
TESTING float (int): 0 !!!!
TESTING double: 1234.123413085 !!!!
CPU: arith=922, logical=47
CPU: shift=65, branches=1098
CPU: jumps=1195, loadstore=1045
CPU: cycles=5404, stalls=0
CPU: hosttime=3939146311
---
```

Remembed that it does not support printing %f. It requires to use ftoa to perform the flot->char* conversion.
