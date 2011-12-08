#include "display.h"

extern "C" void kmain(void* /*mbd*/, u32 magic)
{
   if ( magic != 0x2BADB002 )
   {
      /* Something went not according to specs. Print an error */
      /* message and halt, but do *not* rely on the multiboot */
      /* data structure. */
   }

   /* You could either use multiboot.h */
   /* (http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh) */
   /* or do your offsets yourself. The following is merely an example. */ 
   //char * boot_loader_name =(char*) ((long*)mbd)[16];

   /* Print a string to screen to see everything is working: */
   display::clear_screen();
   display::print("This is", " a", ' ', 't', "est", '!');
   int a = 12, b = 3;
   display::print(" Hey, guess what? ", a, " * ", b, " = ", a * b);

   for(;;);
}

