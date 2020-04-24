
ENTRY(_entry)

/* specify HF-RISC memory areas */

MEMORY
{
rom		: ORIGIN = 0x00000000, LENGTH = 1M	  /* ROM area*/
ram		: ORIGIN = 0x40000000, LENGTH = 4M	  /* RAM area. */
hws		: ORIGIN = 0x40410000, LENGTH = 256B  /* mmio */ 
}

/* the top of the stack is localted in the last position of the RAM, then it grows to the lower addrs*/
__stacktop = ORIGIN(ram) + LENGTH(ram);
/*8K bytes of heap by default.*/
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : 0x2000;
/*4K bytes of heap by default. if using newlib, it is recommended to use at least 2Kbytes*/
HEAP_SIZE  = DEFINED(HEAP_SIZE)  ? HEAP_SIZE  : 0x1000;

SECTIONS
{
	. = 0;

	.text :
	{
		_text = .;
		*(.text)
		*(.text.*)
		_rodata = .;
		*(.rdata)
		*(.rodata)
		*(.rodata.*)
		_erodata = .;
    	_etext = .;
	} > ram

#ifdef CONFIG_CPLUSPLUS
    /*c++ init*/
    .preinit_array ALIGN(4): {
        __preinit_array_start = .;
        KEEP(*(.preinit_array))
        __preinit_array_end = .;
    } >ram

    .init_array ALIGN(4): {
        __init_array_start = .;
        KEEP(*(.init_array))
        __init_array_end = .;
    } >ram

    .fini_array ALIGN(4): {
        __fini_array_start = .;
        KEEP(*(.fini_array))
        __fini_array_end = .;
#endif

    } >ram

	.data :
	{ 
		_data = .;
		*(.data)
		*(.data.*)
		_gp = . + 0x800;
		*(.sdata)
		*(.sdata.*)
		. = ALIGN(16);
	} > ram
	_edata = .;

	.bss :
	{
		_bss_start = .;
        _sbss = . ;
		*(.sbss)
		*(.sbss.*)
		*(.bss)
		*(.bss.*)
		. = ALIGN(4);
		_bss_end = . ;
        _ebss = . ;
	} > ram

	_end = .;

    /*heap section*/
    .heap (NOLOAD):
    {	
		. = ALIGN(8);
		__heap_start = .;
		. = . + HEAP_SIZE;
		__heap_end = .;
	} > ram

    /* stack section */
	. = __stacktop - STACK_SIZE;
	_sstack = .;
	. = . + STACK_SIZE;
	_estack = .;
	_stack = .;

/* include sizes
https://github.com/PinusThunbergii/stm32/blob/master/linker-script/stm32f103c8t6.ld
*/

}

