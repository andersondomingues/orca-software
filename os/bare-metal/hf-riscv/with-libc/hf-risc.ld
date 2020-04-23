/*
https://en.wikipedia.org/wiki/Data_segment
https://embeddedgurus.com/state-space/2014/02/are-we-shooting-ourselves-in-the-foot-with-stack-overflow/
*/

ENTRY(_entry)

/* specify HF-RISC memory areas */

MEMORY
{
rom		: ORIGIN = 0x00000000, LENGTH = 1M	  /* ROM area*/
ram		: ORIGIN = 0x40000000, LENGTH = 4M	  /* RAM area. */
hws		: ORIGIN = 0x40410000, LENGTH = 256B  /* mmio */ 
}

_stack = 0x40400000;
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : 0x2000;

/*4K bytes of heap by default. if using newlib, it is recommended to use at least 2Kbytes*/
HEAP_SIZE = DEFINED(HEAP_SIZE) ? HEAP_SIZE : 0x1000;



SECTIONS
{
	. = 0;

	.text :
	{
		_text = .;
		*(.text)
		*(.text.*)
		_etext = .;
	} > ram

	.data :
	{ 
		_rodata = .;
		*(.rdata)
		*(.rodata)
		*(.rodata.*)
		_erodata = .;
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


    /* stack section */
/*
    .stack (NOLOAD):
    {
        . = ALIGN(8);
        _sstack = .;
        . = . + STACK_SIZE;
        . = ALIGN(8);
        _estack = .;
    } > ram
*/
    . = ALIGN(8);
    __heap_start = .;
	. = . + HEAP_SIZE;
	__heap_end = .;


}

