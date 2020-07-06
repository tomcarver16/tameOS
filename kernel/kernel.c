#include "../drivers/screen.h"
#include "util.h"

void main() {
  int i;
  clear_screen();
  for (i = 0; i < 24; i++) {
    char str[255];
    itoa(i, str);
    kprint_offset(str, 0, i);
  }
  
  kprint_offset("This text forces the kernel to scroll. Row 0 will disappear", 60, 24);
  kprint("And with this text, the kernel will scroll again, and row 1 will disappear");
}
