// Refer to http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh

#ifndef MULTIBOOT_H
#define MULTIBOOT_H

namespace multiboot {

// The magic number passed by the bootloader to the operating system.
const u32 bootloader_magic = 0x2BADB002;

struct aout_symbol_table_t {
    u32 tabsize, strsize, addr, reserved;
};

struct elf_section_header_table_t {
    u32 num, size, addr, shndx;
};

// The different possible flags in the flags member
enum class flag {
    memory                   = 1<<0,
    boot_device              = 1<<1,
    cmdline                  = 1<<2,
    modules                  = 1<<3,
    aout_symbol_table        = 1<<4, // These two are
    elf_section_header_table = 1<<5, // mutually exclusive.
    memory_map               = 1<<6,
    drive_info               = 1<<7,
    config_table             = 1<<8,
    bootloader_name          = 1<<9,
    apm_table                = 1<<10,
    video_info               = 1<<11
};

// Structure of the information recieved from the multiboot-compliant
// bootloader (e.g. GRUB)
// TODO Add accessor functions returning the correct type for each
// members.
struct info {
    bool hasflag(flag f) {
        return flags & int(f);
    }

    // Determines which fields below are present.
    u32 flags;

    // Available memory from BIOS (in kilobytes)
    u32 memory_lower, memory_higher;

    // Boot device which was used to boot the kernel.
    u32 boot_device;

    // The command line passed to the kernel by the bootloader.
    // Example: /system/spideros.exe
    u32 cmdline;

    // Boot module list
    u32 modules_count, modules_address;

    union {
        aout_symbol_table_t aout_symbol_table;
        elf_section_header_table_t elf_section_header_table;
    };

    // Memory mapping buffer
    u32 mmap_length, mmap_address;

    // Drive info buffer
    u32 drives_length, drives_address;

    // ROM configuration table
    u32 config_table;

    // Bootloader name (e.g. GNU GRUB 0.97)
    u32 bootloader_name;

    // APM (Advanced Power Management) table
    u32 apm_table;

    // Video
    u32 vbe_control_info, vbe_mode_info;
    u16 vbe_mode, vbe_interface_seg, vbe_interface_off,
        vbe_interface_len;
};

// Structure of an entry in the multiboot memory mapping buffer
struct mmap_entry {
    // Size of the structure (not counting the size member itself)
    u32 size;

    // Memory region starting address (address_high only for 64-bit)
    u32 address_low, address_high;

    // Memory region length (length_high only for 64-bit)
    u32 length_low, length_high;

    // Type of memory (1 means available, 2 means reserved)
    u32 type;
};

// Structure of a module in the multiboot boot module list
struct module_list {
    // The memory goes from module_start to module_end-1 inclusive.
    u32 module_start, module_end;

    // Command line given to the module
    u32 cmdline;

    // Padding to take it to 16 bytes (must be zero)
    u32 pad;
};

} // namespace multiboot

#endif // MULTIBOOT_H
