ENTRY(boot)

SECTIONS
{
    . = 0x00100000;
    linker_kernelStart = .;

    .text ALIGN (0x1000) :
    {
        *(.text)
        *(.gnu.linkonce.t*)
    }

    .rodata ALIGN (0x1000) :
    {
        linker_constructorsStart = .;
        *(.ctor*)
        linker_constructorsEnd = .;

        *(.rodata*)
        *(.gnu.linkonce.r*)
    }

    .data ALIGN (0x1000) :
    {
        *(.data)
        *(.gnu.linkonce.d*)
    }

    .bss :
    {
        *(COMMON)
        *(.bss)
        *(.gnu.linkonce.b*)
    }

    linker_kernelEnd = .;

    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame) /* discard this, unless you are implementing runtime support for C++ exceptions. */
    }
}
