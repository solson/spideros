#include "types.h"
#include "display.h"
#include "multiboot.h"
#include "gdt.h"
#include "idt.h"
#include "interrupts.h"

extern "C" void kmain(const multiboot::Info* mbinfo, u32 magic) {
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
    display::println("Welcome to spideros");
    display::println("===================");
    display::println("This is", " a", ' ', 't', "est", '.');
    const int a = 12, b = 3;
    display::println("Hey look, numbers: ", a, " * ", b, " = ", a * b);
    display::println();

    if(mbinfo->hasFlag(multiboot::BOOTLOADER_NAME))
        display::println("Bootloader:\t", (const char*) mbinfo->bootloaderName);

    if(mbinfo->hasFlag(multiboot::COMMAND_LINE))
        display::println("Command line:\t", (const char*) mbinfo->commandLine);

    display::println("Integer printing tests:");
    display::println("Hex: 0x", display::hex(42));
    display::println("Oct: 0",  display::oct(42));
    display::println("Bin: 0b", display::bin(42));
    display::println("Dec: ", 42);
    display::println();

    display::print("Initializing GDT... ");
    gdt::init();
    display::println("done.");

    display::print("Initializing IDT... ");
    idt::init();
    display::println("done.");

    idt::setGate(0, (void*) interrupts::isr0, 0x8, 0, 0, idt::INTR32);

    // Do division by zero to trigger interrupt 0
    int y = 0;
    y = 1 / y;
}
