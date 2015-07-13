import os

env = Environment(
    CXX = 'clang++',
    CPPFLAGS = [
        '-Wall',
        '-Wextra',
        '-std=c++14',
        '-m32',
        '-march=i386',
        '-nostdlib',
        '-nostdinc',
        '-ffreestanding',
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

# Allow clang static analyzer to override the compiler.
env['CC'] = os.getenv('CC') or env['CC']
env['CXX'] = os.getenv('CXX') or env['CXX']
env['ENV'].update(x for x in os.environ.items() if x[0].startswith('CCC_'))

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
        'src/keyboard.cpp',
        'src/kmain.cpp',
        'src/memory.cpp',
        'src/misc.asm',
        'src/util.cpp',
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
