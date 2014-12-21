#include "assert.h"
#include "display.h"
#include "memory.h"
#include "multiboot.h"
#include "util.h"

namespace memory {

// Defined by the linker (see linker.ld). The types and values of these
// constants are arbitrary, but their addresses are important.
extern "C" const int linker_kernelStart;
extern "C" const int linker_kernelEnd;

// The addresses of the start and end of the kernel in memory.
const u32 KERNEL_START = reinterpret_cast<u32>(&linker_kernelStart);
const u32 KERNEL_END   = reinterpret_cast<u32>(&linker_kernelEnd);

struct FreeInfo;

// In every managed memory region, free or allocated, this structure is stored
// at both the start and end. This enables finding the next or previous region
// quickly.
//
// We will refer to the info at the start and end as the 'header' and 'footer'
// respectively.
struct Info {
  // TODO: Force `size` to be 2-byte aligned so we can use the least-significant
  // bit to store the isAllocated boolean and save space.
  u32 size;
  bool isAllocated;
};

struct Footer;

// Region info at the start of the region.
struct Header : public Info {
  // If this region is free, return the associated FreeInfo which is
  // stored directly after this header.
  FreeInfo* freeInfo() const {
    assert(!isAllocated);
    // Calculate the address after the end of this structure.
    u32 addr = reinterpret_cast<u32>(this) + sizeof(*this);
    return reinterpret_cast<FreeInfo*>(addr);
  }

  Footer* footer() const {
    u32 addr = reinterpret_cast<u32>(this) + sizeof(*this) + size;
    return reinterpret_cast<Footer*>(addr);
  }
};

// Region info at the end of the region.
struct Footer : public Info {
  Header* header() const {
    u32 addr = reinterpret_cast<u32>(this) - size - sizeof(Header);
    return reinterpret_cast<Header*>(addr);
  }

  FreeInfo* freeInfo() const {
    return header()->freeInfo();
  }
};

// In free regions, this structure is stored just after the header. It links to
// the nearest free memory regions before and after this one (specifically, to
// their headers).
struct FreeInfo {
  Header* prev;
  Header* next;
};

Header* freeList = nullptr;

Header* placeHeader(u32 addr, u32 size, bool isAllocated) {
  Header* h = reinterpret_cast<Header*>(addr);
  h->size = size;
  h->isAllocated = isAllocated;
  return h;
}

Footer* placeFooter(u32 addr, u32 size, bool isAllocated) {
  Footer* f = reinterpret_cast<Footer*>(addr);
  f->size = size;
  f->isAllocated = isAllocated;
  return f;
}

// Make a contiguous region of usable memory available to the memory manager.
// `startAddr` is inclusive and `endAddr` is exclusive.
void initContiguousRegion(u32 startAddr, u32 endAddr) {
  // TODO: Move the lastAdded variable into the memory::init() function.
  static Header* lastAdded = nullptr;

  display::println("> initContiguousRegion({x}, {x})", startAddr, endAddr - 1);

  // Don't store anything at address zero or nullptr checks will go wrong.
  if (startAddr == 0) {
    startAddr += sizeof(Header);
  }

  // When coalescing adjacent free memory regions into a single large region, we
  // always examine the regions before and after the newly freed region, which
  // won't work when the first or last region of available memory is freed.
  //
  // As a work around, we store dummy Info structures at the start and end
  // of each contiguous usable memory region so coalescing knows where to stop.
  //
  // We must adjust the start and end points for the leftover region afterwards.
  placeHeader(startAddr, 0, true);
  startAddr += sizeof(Header);
  endAddr -= sizeof(Footer);
  placeFooter(endAddr, 0, true);

  // Set up the header and footer.
  u32 size = endAddr - startAddr - sizeof(Header) - sizeof(Footer);
  Header* header = placeHeader(startAddr, size, false);
  header->footer()->size = size;
  header->footer()->isAllocated = false;

  // Set up the free-region-specific info after the header and hook up the free
  // list links.
  header->freeInfo()->prev = lastAdded;
  header->freeInfo()->next = nullptr;

  if (lastAdded) {
    lastAdded->freeInfo()->next = header;
  } else {
    freeList = header;
  }

  lastAdded = header;
}

void init(u32 mmapAddr, u32 mmapLen) {
  u32 mmapCurr = mmapAddr;
  u32 mmapEnd = mmapAddr + mmapLen;

  display::println("");
  while (mmapCurr < mmapEnd) {
    auto* mmapEntry = reinterpret_cast<multiboot::MmapEntry*>(mmapCurr);
    if (mmapEntry->type == 1) {
      u32 regionStart = mmapEntry->addr;
      u32 regionEnd = regionStart + mmapEntry->len;

      // Initialize the part of the region before the kernel, if it exists.
      if (regionStart < KERNEL_START) {
        initContiguousRegion(regionStart, min(KERNEL_START, regionEnd));
      }

      // Initialize the part of the region after the kernel, if it exists.
      if (regionEnd > KERNEL_END) {
        initContiguousRegion(max(regionStart, KERNEL_END), regionEnd);
      }
    }

    display::print(mmapEntry->type == 1 ? "unused" : "used  ");
    display::println(" 0x{x}\t- 0x{x}", mmapEntry->addr,
                     mmapEntry->addr + mmapEntry->len - 1);

    mmapCurr += mmapEntry->size + sizeof(mmapEntry->size);
  }

  display::println("kernel start: 0x{x}", KERNEL_START);
  display::println("kernel end:   0x{x}", KERNEL_END);

  display::println("");
  for (Header* h = freeList; h; h = h->freeInfo()->next) {
    assert(!h->isAllocated);
    assert(!h->footer()->isAllocated);
    assert(h->size == h->footer()->size);
    display::println("Header: 0x{x}", reinterpret_cast<u32>(h));
    display::println("Footer: 0x{x}", reinterpret_cast<u32>(h->footer()));
    display::println("Size:   {}", h->size);
    display::println("");
  }
}

} // namespace memory
