ENTRY("startup")
/* Highest address of the user mode stack */
_estack = 0x20010000;    /* end of RAM */
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x200;      /* required amount of heap  */
_Min_Stack_Size = 0x400; /* required amount of stack */

MEMORY
{
    rom(rx)     : o = 0x00000000, l = 0x00100000 /* ROM 1MB */
    ram(rwx)    : o = 0x1fff0000, l = 0x00020000 /* RAM 128KB */
    stack(rw)   : o = 0x2000fffc, l = 0x00000004 /* end of RAM */
                                                 /* store virtual vector addr*/
    
    sram(rwx)   : o = 0x00400000, l = 0x00080000 /* SRAM 512KB */
}

SECTIONS
{
	/* The startup code goes first into FLASH */
  	.isr_vector :
	{
    	. = ALIGN(4);
    	KEEP(*(.isr_vector)) /* Startup code */
    	. = ALIGN(4);
  	} >rom

  	/* The program code and other data goes into FLASH */
  	.text :
  	{
    	. = ALIGN(4);
    	*(.text)           /* .text sections (code) */
    	*(.text*)          /* .text* sections (code) */
    	*(.glue_7)         /* glue arm to thumb code */
    	*(.glue_7t)        /* glue thumb to arm code */
    	*(.eh_frame)

    	KEEP (*(.init))
    	KEEP (*(.fini))

    	. = ALIGN(4);
   		_etext = .;        /* define a global symbols at end of code */
  	} >rom

	/* Constant data goes into FLASH */
  	.rodata :
  	{
    	. = ALIGN(4);
    	*(.rodata)         /* .rodata sections (constants, strings, etc.) */
    	*(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    	. = ALIGN(4);
  	} >rom

	.ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >rom
  	.ARM : {
    	__exidx_start = .;
    	*(.ARM.exidx*)
    	__exidx_end = .;
  	} >rom

  	.preinit_array     :
  	{
    	PROVIDE_HIDDEN (__preinit_array_start = .);
    	KEEP (*(.preinit_array*))
    	PROVIDE_HIDDEN (__preinit_array_end = .);
  	} >rom
  	.init_array :
  	{
    	PROVIDE_HIDDEN (__init_array_start = .);
    	KEEP (*(SORT(.init_array.*)))
    	KEEP (*(.init_array*))
    	PROVIDE_HIDDEN (__init_array_end = .);
  	} >rom
  	.fini_array :
  	{
    	PROVIDE_HIDDEN (__fini_array_start = .);
    	KEEP (*(SORT(.fini_array.*)))
    	KEEP (*(.fini_array*))
    	PROVIDE_HIDDEN (__fini_array_end = .);
  	} >rom

  	/* used by the startup to initialize data */
  	_sidata = LOADADDR(.data);

  	/* Initialized data sections goes into RAM, load LMA copy after code */
  	.data : 
  	{
    	. = ALIGN(4);
    	_sdata = .;        /* create a global symbol at data start */
    	*(.data)           /* .data sections */
    	*(.data*)          /* .data* sections */

    	. = ALIGN(4);
    	_edata = .;        /* define a global symbol at data end */
  	} >ram AT> rom

  
  	/* Uninitialized data section */
  	. = ALIGN(4);
  	.bss :
  	{
    	/* This is used by the startup in order to initialize the .bss secion */
    	_sbss = .;         /* define a global symbol at bss start */
    	__bss_start__ = _sbss;
    	*(.bss)
    	*(.bss*)
    	*(COMMON)

    	. = ALIGN(4);
    	_ebss = .;         /* define a global symbol at bss end */
    	__bss_end__ = _ebss;
  	} >ram

  	/* User_heap_stack section, used to check that there is enough RAM left */
  	._user_heap_stack :
  	{
    	. = ALIGN(8);
    	PROVIDE ( end = . );
    	PROVIDE ( _end = . );
    	. = . + _Min_Heap_Size;
    	. = . + _Min_Stack_Size;
    	. = ALIGN(8);
  	} >ram

  	/* Remove information from the standard libraries */
  	/DISCARD/ :
  	{
    	libc.a ( * )
    	libm.a ( * )
    	libgcc.a ( * )
  	}

  	.ARM.attributes 0 : { *(.ARM.attributes) }
}
