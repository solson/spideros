import os

env = Environment(
    CXX = 'clang++',
    CPPFLAGS = [
        '-Wall',
        '-Wextra',
        '-std=c++11', # TODO: Switch to c++14/c++1y.
        '-m32',
        '-nostdlib',
        '-fno-builtin',
        '-fno-exceptions',
        '-fno-rtti',
        '-fno-stack-protector',
    ],

    AS = 'nasm',
    ASFLAGS = [
        '-felf32',
    ],

    ENV = {
        # Allow clang++ to use color.
        'TERM': os.environ['TERM']
    },
)

spideros_exe = env.Program(
    target = 'isofs/system/spideros.exe',
    source = [
        'src/assert.cpp',
        'src/boot.asm',
        'src/cppsupport.cpp',
        'src/display.cpp',
        'src/gdt.cpp',
        'src/idt.cpp',
        'src/interrupts.cpp',
        'src/kmain.cpp',
        'src/memory.cpp',
        'src/misc.asm',
        'src/ports.cpp',
        'src/string.cpp',
    ],
    LINK = 'ld',
    LINKFLAGS = [
        '-melf_i386',
        '-nostdlib',
        '-Tlinker.ld',
    ],
)
Depends(spideros_exe, 'linker.ld')

env.Command('spideros.iso', spideros_exe, 'grub-mkrescue -o $TARGET isofs')

if ARGUMENTS.get('debug') == '1':
    env.Append(CPPFLAGS = ['-g'], ASFLAGS = ['-g'])

# Print C++ flags for the YouCompleteMe vim plugin.
if ARGUMENTS.get('ycm'):
    print(env.subst('$CXXFLAGS $CCFLAGS $_CCCOMCOM'))
    exit()
