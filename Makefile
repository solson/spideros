CXX := clang++
AS  := nasm
LD  := ld
QEMU := qemu-system-i386

CXXFLAGS := -std=c++11 -m32 -Wall -Wextra -Werror -nostdlib -fno-builtin \
	-fno-exceptions -fno-rtti -fno-stack-protector
ASFLAGS := -felf32 -g
LDFLAGS := -melf_i386 -nostdlib -g

# Needs to be overridden in some operating systems
GRUB_MKRESCUE := grub-mkrescue

CXXFILES := $(shell find "src" -name "*.cpp")
HDRFILES := $(shell find "src" -name "*.h")
ASMFILES := $(shell find "src" -name "*.asm")
OBJFILES := $(patsubst %.asm,%.o,$(ASMFILES)) $(patsubst %.cpp,%.o,$(CXXFILES))

.PHONY: all clean bochs qemu todolist

all: spideros.iso

bochs: spideros.iso
	@bochs -qf .bochsrc

qemu: spideros.iso
	@$(QEMU) -cdrom $< -net none

spideros.iso: spideros.exe isofs/boot/grub/grub.cfg
	@echo "ISO spideros.iso"
	@mkdir -p isofs/system
	@cp $< isofs/system
	@grub-mkrescue -o $@ isofs 2> /dev/null

spideros.exe: $(OBJFILES)
	@echo "LD  $@"
	@$(LD) $(LDFLAGS) -T linker.ld -o $@ $^

%.o: %.cpp
	@echo "CXX $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.asm
	@echo "ASM $<"
	@$(AS) $(ASFLAGS) -o $@ $<

todolist:
	@grep --color=auto --exclude=Makefile -r -F -n -I -e TODO -e FIXME src

clean:
	$(RM) $(wildcard $(OBJFILES) spideros.exe spideros.iso isofs/system/spideros.exe)
