
/**
 * @file
 * @brief Linker script to specify HF-RISC memory areas, assuming bare metal apps
 * 
 * ROM is for an external flash with the code. currently not used bu required when porting it to FPGA.
 *   .text
 *   .rodata 
 * RAM sections:
 *   .data
 *   .preinit <=== ?!?!?
 *   .init    <== global classes constructors
 *   .fini    <=== aparrently this is not been used. so the destructos of global classes are note called
 *   .bss     <=== initialized local vars 
 *   .heap    <=== for dynamic allocation
 *   .stack   <=== global stack
*/

ENTRY(_entry)

/*  
add the following line in the app make to redefine one of the following symbols
example:
LDFLAGS += --defsym=RAM_SIZE=7M
*/

/** if the flash size is not defined, the default is 1Mbyte*/
FLASH_SIZE = DEFINED(FLASH_SIZE) ? FLASH_SIZE : 1M;
/** by default, the flash starts at addr zero*/
FLASH_OFF = DEFINED(FLASH_OFF) ? FLASH_OFF : 0;
/** if the ram size is not defined, the default is 4Mbyte*/
RAM_SIZE = DEFINED(RAM_SIZE) ? RAM_SIZE : 4M;
/** by default, the flash starts at addr 0x40000000*/
RAM_OFF = DEFINED(RAM_OFF) ? RAM_OFF : 0x40000000;

MEMORY
{
rom		: ORIGIN = FLASH_OFF, LENGTH = FLASH_SIZE  /* ROM area*/
ram		: ORIGIN = RAM_OFF,   LENGTH = RAM_SIZE	   /* RAM area. */
}

/** 8K bytes of heap by default. configurable with a define*/
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : 0x2000;
/** 4K bytes of heap by default. if using newlib, it is recommended to use at least 2Kbytes. configurable with a define*/
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
	} >ram
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
	/*heap starts after the bss section and it grows to higher addresses*/
	.heap (NOLOAD):
	{	
		. = ALIGN(8);
		__heap_start = .;
		. = . + HEAP_SIZE;
		__heap_end = .;
	} > ram

    	/* stack section */
	/*
	. = __stacktop - STACK_SIZE;
	_sstack = .;
	. = . + STACK_SIZE;
	_estack = .;
	_stack = .;
	*/

	/*stack starts at the end of the memory and it grows towards the heap ... possible silient stack overflow*/
	.stack ORIGIN(ram) + LENGTH(ram) - STACK_SIZE :
	{
	PROVIDE( _sstack = . );
	. = STACK_SIZE;
	PROVIDE( _stack = . );
	} >ram


	_text_size = SIZEOF(.text);
	_bss_size = SIZEOF(.bss);
	_heap_size = SIZEOF(.heap);
	_stack_size = _stack - _sstack;
	_data_size = SIZEOF(.data);
	_init_array_size = SIZEOF(.init_array);
	_preinit_array_size = SIZEOF(.preinit_array);
	_fini_array_size = SIZEOF(.fini_array);

	/* Check if data + heap + stack exceeds RAM limit */
	/*https://github.com/zolkko/lpc2468-dh-example/blob/master/newlib-nano.ld*/
	ASSERT(_sstack > __heap_end, "region RAM overflowed with stack")

	/*all the memory mapped IO should be placed above this address.
	So, the memory region cannot reach beyond this*/
	ASSERT((ORIGIN(ram) + LENGTH(ram)) < 0xe0000000, "RAM is overlapping with MMIO area")

}
