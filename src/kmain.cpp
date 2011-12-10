#include "display.h"
#include "multiboot.h"

extern "C" void kmain(multiboot::info* mbinfo, u32 magic) {
    if(magic != multiboot::bootloader_magic) {
        // Something went not according to specs. Do not rely on the
        // multiboot data structure.
        display::println("error: The bootloader's magic number didn't "
            "match. Something must have gone wrong.");
        return;
    }

    // Print to screen to see everything is working.
    display::init();
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

    // Sit around wasting CPU cycles.
    while(true){}
}

