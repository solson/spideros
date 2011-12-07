const int console_width = 80;
const int console_height = 25;
const u8 color = 0x07; /* light grey (7) on black (0). */

u16 volatile* videoram = (u16 volatile*) 0xb8000;

void clear_screen() {
   for(int i = 0; i < console_height * console_width; i++)
      videoram[i] = (color << 8) | ' ';
}

void put_char_at(char c, int row, int col) {
   videoram[row * console_width + col] = (color << 8) | c;
}

int position = 0;
void put_char(char c) {
   put_char_at(c, 0, position); // TODO Handle end of line wrapping.
   position++;
}

void put_string(const char* str) {
   for(int i = 0; str[i]; i++)
      put_char(str[i]);
}


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
   clear_screen();
   put_string("This is a test!");
}

