#ifndef MULTIBOOT_H
#define MULTIBOOT_H
// Refer to http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh

#include "types.h"

namespace multiboot {

// The magic number passed by the bootloader to the operating system.
const u32 BOOTLOADER_MAGIC = 0x2BADB002;

struct AoutSymbolTable {
  u32 tabsize, strsize, addr, reserved;
};

struct ElfSectionHeaderTable {
  u32 num, size, addr, shndx;
};

// The different possible flags in the flags member
enum Flag {
  MEMORY                   = 1 << 0,
  BOOT_DEVICE              = 1 << 1,
  COMMAND_LINE             = 1 << 2,
  MODULES                  = 1 << 3,
  AOUT_SYMBOL_TABLE        = 1 << 4, // These two are
  ELF_SECTION_HEADER_TABLE = 1 << 5, // mutually exclusive.
  MEMORY_MAP               = 1 << 6,
  DRIVE_INFO               = 1 << 7,
  CONFIG_TABLE             = 1 << 8,
  BOOTLOADER_NAME          = 1 << 9,
  APM_TABLE                = 1 << 10,
  VIDEO_INFO               = 1 << 11
};

// Structure of the information recieved from the multiboot-compliant bootloader
// (e.g. GRUB)
// TODO: Add accessor functions returning the correct type for each member.
struct Info {
  bool hasFlag(Flag f) const {
    return flags & f;
  }

  // Determines which fields below are present.
  u32 flags;

  // Available memory from BIOS (in kilobytes)
  u32 memoryLower, memoryHigher;

  // Boot device which was used to boot the kernel.
  u32 bootDevice;

  // The command line passed to the kernel by the bootloader.
  // Example: /system/spideros.exe
  u32 commandLine;

  // Boot module list
  u32 modulesCount, modulesAddr;

  union {
    AoutSymbolTable aoutSymbolTable;
    ElfSectionHeaderTable elfSectionHeaderTable;
  };

  // Memory mapping buffer
  u32 mmapLen, mmapAddr;

  // Drive info buffer
  u32 drivesLen, drivesAddr;

  // ROM configuration table
  u32 configTable;

  // Bootloader name (e.g. GNU GRUB 0.97)
  u32 bootloaderName;

  // APM (Advanced Power Management) table
  u32 apmTable;

  // Video
  u32 vbeControlInfo, vbeModeInfo;
  u16 vbeMode, vbeInterfaceSeg, vbeInterfaceOff, vbeInterfaceLen;
};

// Structure of an entry in the multiboot memory mapping buffer
struct MmapEntry {
  // Size of the structure (not counting the size member itself)
  u32 size;

  // Memory region starting address (addrHigh only for 64-bit)
  u32 addr, addrHigh;

  // Memory region length (lenHigh only for 64-bit)
  u32 len, lenHigh;

  // Type of memory (1 means available, 2 means reserved)
  u32 type;
};

// Structure of a module in the multiboot boot module list
struct ModuleList {
  // The memory goes from moduleStart to moduleEnd-1 inclusive.
  u32 moduleStart, moduleEnd;

  // Command line given to the module
  u32 commandLine;

  // Padding to take it to 16 bytes (must be zero)
  u32 padding;
};

} // namespace multiboot

#endif // MULTIBOOT_H
