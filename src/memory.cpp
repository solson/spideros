#include "memory.h"
#include "multiboot.h"

namespace memory {

// Defined by the linker (see linker.ld).
extern "C" u32 kernel_start;
extern "C" u32 kernel_end;

void init(u32 mmapAddr, u32 mmapLen) {
  u32 mmapCurr = mmapAddr;
  u32 mmapEnd = mmapAddr + mmapLen;

  while (mmapCurr < mmapEnd) {
    auto* mmapEntry = reinterpret_cast<multiboot::MmapEntry*>(mmapCurr);
    if (mmapEntry->type == 1) {
      u32 mmapEntryEnd = mmapEntry->addr + mmapEntry->len;
      // TODO: Initialize memory header. Check for overlap with kernel.
    }
    mmapCurr += mmapEntry->size + sizeof(mmapEntry->size);
  }
}

} // namespace memory
