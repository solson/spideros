CXX := g++
AS  := nasm
LD  := ld

CXXFLAGS := -include src/prelude.h -std=c++0x -m32 -Wall -Wextra -Werror \
	-nostdlib -fno-builtin -fno-exceptions -fno-rtti -fno-stack-protector
ASFLAGS := -felf32 -g
LDFLAGS := -melf_i386 -nostdlib -g

STAGE2 := /boot/grub/stage2_eltorito

CXXFILES := $(shell find "src" -name "*.cpp")
HDRFILES := $(shell find "src" -name "*.h")
ASMFILES := $(shell find "src" -name "*.asm")
OBJFILES := $(patsubst %.asm,%.o,$(ASMFILES)) $(patsubst %.cpp,%.o,$(CXXFILES))

.PHONY: all clean bochs qemu todolist

all: spideros.iso

bochs: spideros.iso
	@bochs -qf .bochsrc

qemu: spideros.iso
	@qemu -cdrom spideros.iso -net none

spideros.iso: spideros.exe isofs/boot/grub/stage2_eltorito isofs/boot/grub/menu.lst
	@mkdir -p isofs/system
	cp $< isofs/system
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -input-charset utf-8 -o $@ isofs

spideros.exe: ${OBJFILES}
	${LD} ${LDFLAGS} -T linker.ld -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) ${ASFLAGS} -o $@ $<

isofs/boot/grub/stage2_eltorito:
	@mkdir -p isofs/boot/grub
	cp ${STAGE2} $@

todolist:
	@grep --color=auto --exclude=Makefile -r -F -n -e TODO -e FIXME .

clean:
	 $(RM) $(wildcard $(OBJFILES) spideros.exe spideros.iso)
