CXX := g++

CXXFLAGS := -m32 -nostdinc -ffreestanding -fno-stack-protector
LDFLAGS := -melf_i386 -nostdlib -g
ASFLAGS := -felf32 -g

STAGE2 := /boot/grub/stage2_eltorito

CXXFILES := $(shell find "src" -name "*.cpp")
ASMFILES := $(shell find "src" -name "*.asm")
OBJFILES := $(patsubst %.cpp,%.o,$(CXXFILES))
ASMOBJFILES := $(patsubst %.asm,%.o,$(ASMFILES))

.PHONY: all clean bochs bochs-dbg

all: spideros.iso

bochs: all
	bochs -qf .bochsrc

bochs-dbg: all
	bochs -qf .bochsrc-dbg

spideros.iso: spideros.exe isofs/boot/grub/stage2_eltorito isofs/boot/grub/menu.lst
	@mkdir -p isofs/system
	cp $< isofs/system
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -input-charset utf-8 -o $@ isofs

spideros.exe: ${ASMOBJFILES} ${OBJFILES}
	${LD} ${LDFLAGS} -T src/linker.ld -o $@ ${ASMOBJFILES} ${OBJFILES}

%.o: %.asm
	nasm ${ASFLAGS} -o $@ $<

isofs/boot/grub/stage2_eltorito:
	mkdir -p isofs/boot/grub
	cp ${STAGE2} $@

clean:
	 $(RM) -r $(wildcard $(ASMOBJFILES) ${OBJFILES} spideros.exe spideros.iso)
