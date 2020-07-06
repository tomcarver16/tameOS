#include "../drivers/screen.h"

void main() {
  clear_screen();
  kprint_offset("X", 1, 6);
  kprint_offset("Across multiple lines", 75, 10);
  kprint("There is a line\nbreak!");
  kprint_offset("What's going on if run out of space", 45, 24);
}
