MEMORY
{
    ram(rwx)    : o = 0x1c000000, l = 0x1c0fdfff /* RAM 1MB */
    stack(rw)   : o = 0x1c0fe000, l = 0x00002000 /* end of RAM */
    exram(xrw)  : o = 0xfff80000, l = 0x00010000
}

ENTRY("_startup")
SECTIONS
{
    .text : ALIGN(0x10) {
        _stext = . ;
        KEEP(*(.isr_vector))
        *(.text)
        _etext = . ;
    } > ram

    .rodata : ALIGN(0x10) {
        _srodata = . ;
        *(.strings)
        *(.rodata)
        *(.rodata.*)
        _erodata = . ;
    } > ram

     . = ALIGN(4);
     __end_of_text__ = .;
     _sidata = __end_of_text__;

    _sdataonrom = .; 
    .data : AT(_sdataonrom) {
        _sdata = . ;
        *(.data)
        _edata = . ;
    } > ram
    _edataonrom = _sdataonrom + SIZEOF(.data);

    .bss : {
        _sbss = . ;
        *(.bss)
        *(COMMON)
        _ebss = . ;
        _end = .;
    } > ram

    .stack 0x1c0ffffc :
    {
        . = ALIGN(4);
        _stack = .;
    } >stack
}
