#include "types.h"
#include "display.h"
#include "multiboot.h"
#include "gdt.h"

struct test_destructors {
    const char* str;
    test_destructors(const char* str_) : str(str_) {}
    ~test_destructors() { display::println(str, " destructed"); }
};

test_destructors bar("bar");

extern "C" void kmain(multiboot::info* mbinfo, u32 magic) {
    display::init();

    if(magic != multiboot::bootloader_magic) {
        // Something went not according to specs. Do not rely on the
        // multiboot data structure.
        display::println("error: The bootloader's magic number didn't "
            "match. Something must have gone wrong.");
        return;
    }

    // Print to screen to see everything is working.
    display::clear_screen();
    display::println("Welcome to SpiderOS!");
    display::println("====================");
    display::println("This is", " a", ' ', 't', "est", '!');
    const int a = 12, b = 3;
    display::println("Hey look, numbers: ", a, " * ", b, " = ", a * b);

    display::println(); // Just a newline.

    if(mbinfo->hasflag(multiboot::flag::bootloader_name))
        display::println("Bootloader:\t",
                (const char*) mbinfo->bootloader_name);

    if(mbinfo->hasflag(multiboot::flag::cmdline))
        display::println("Command line:\t",
                (const char*) mbinfo->cmdline);

    display::println("Hex: 0x", display::hex(120), '\n',
                     "Oct: 0",  display::oct(120), '\n',
                     "Bin: 0b", display::bin(120), '\n',
                     "Dec: ", 120);

    display::println("Initializing GDT...");
    gdt::init();

    test_destructors foo("foo");
    test_destructors baz("baz");
}

