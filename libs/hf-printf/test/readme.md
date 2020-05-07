
Compiling 
--- 

This will compile a simple test app using hf-printf.f using ORCA with single-core plataform and bare-metal OS.

```bash
make -f Makefile-Orca clean all V=1
```

The code size is:

```
riscv64-unknown-elf-size image.elf
   text	   data	    bss	    dec	    hex	filename
   2552	     68	  12292	  14912	   3a40	image.elf
```

This is the detailed code size information

```
python ../../../tools/linker-map-summary/analyze_map.py image.map
orca-software-tools/os/bare-metal/hf-riscv/startup.o 	        109  (code: 36 data: 73)
lib-orca-monitoring.a(orca-hardware-counters.o) 	           110  (code: 0 data: 110)
orca-software-tools/os/bare-metal/hf-riscv/crt0.o 	           251  (code: 192 data: 59)
app-test.a(test.o)                     	                    2217  (code: 2143 data: 74)
gcc/riscv64-unknown-elf/8.3.0/rv32im/ilp32/libgcc.a(lesf2.o)  3039  (code: 180 data: 2859)
*fill*                                   	                    12297 (code: 1 data: 12296)
TOTAL 18023  (code: 2552 data: 15471)
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
