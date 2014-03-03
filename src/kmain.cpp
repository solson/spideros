#include "types.h"
#include "display.h"
#include "multiboot.h"
#include "gdt.h"

struct TestDestructors {
    const char* str;
    TestDestructors(const char* str_) : str(str_) {}
    ~TestDestructors() { display::println(str, " destructed"); }
};

TestDestructors bar("bar");

extern "C" void kmain(multiboot::Info* mbinfo, u32 magic) {
    display::init();

    if(magic != multiboot::BOOTLOADER_MAGIC) {
        // Something went not according to specs. Do not rely on the
        // multiboot data structure.
        display::println("error: The bootloader's magic number didn't "
            "match. Something must have gone wrong.");
        return;
    }

    // Print to screen to see everything is working.
    display::clearScreen();
    display::println("Welcome to SpiderOS!");
    display::println("====================");
    display::println("This is", " a", ' ', 't', "est", '!');
    const int a = 12, b = 3;
    display::println("Hey look, numbers: ", a, " * ", b, " = ", a * b);

    display::println(); // Just a newline.

    if(mbinfo->hasFlag(multiboot::BOOTLOADER_NAME))
        display::println("Bootloader:\t", (const char*) mbinfo->bootloaderName);

    if(mbinfo->hasFlag(multiboot::COMMAND_LINE))
        display::println("Command line:\t", (const char*) mbinfo->commandLine);

    display::println("Hex: 0x", display::hex(120), '\n',
                     "Oct: 0",  display::oct(120), '\n',
                     "Bin: 0b", display::bin(120), '\n',
                     "Dec: ", 120);

    display::println("Initializing GDT...");
    gdt::init();

    TestDestructors foo("foo");
    TestDestructors baz("baz");
}

