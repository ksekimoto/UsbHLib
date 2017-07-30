MEMORY
{
    ram(rwx)    : o = 0x00000000, l = 0x0007e000 /* RAM 512KB */
    stack(rw)   : o = 0x0007e000, l = 0x00002000 /* end of RAM */
    sdram(rwx)  : o = 0x08000000, l = 0x00800000 /* SDRAM 8MB */
    rom(rx)     : o = 0xffc00000, l = 0x003fff80 /* ROM 4MB */
    exvectors(rx): o = 0xffffff80, l = 0x0000007c
    fvectors(rx): o = 0xfffffffc, l = 0x00000004
}

ENTRY("_startup")
SECTIONS
{
    .text : {
        _stext = . ;
		*(.text)
		*(.text.*)
		*(P)
        _etext = . ;
    } > rom

    .rodata : {
        _srodata = . ;
		*(.rodata)
		*(.rodata.*)
		*(C)
		*(C_1)
		*(C_2)
		*(C$VECT)
        *(.strings)
        _erodata = . ;
    } > rom

     . = ALIGN(4);
     __end_of_text__ = .;
     _sidata = __end_of_text__;

    _sdataonrom = .; 
    .data : AT(_sdataonrom) {
        _sdata = . ;
		*(.data)
		*(.data.*)
		*(D)
		*(D_1)
		*(D_2)
        _edata = . ;
    } > ram
    _edataonrom = _sdataonrom + SIZEOF(.data);

    .bss : {
        _sbss = . ;
		*(.bss)
		*(.bss.*)
		*(COMMON)
		*(B)
		*(B_1)
		*(B_2)
        _ebss = . ;
    } > ram

    . = ALIGN(4);
    _end = . ;

    .stack : {
        _stack = .;
        _user_vbr = .;
    } > stack

    .exvector_tbl  : {
        KEEP(*(.exvectors)) 
    } > exvectors AT>exvectors
    
    .fvector_tbl  : {
        KEEP(*(.fvectors)) 
    } > fvectors AT>fvectors
}
