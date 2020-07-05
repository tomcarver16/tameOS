#include "../drivers/ports.h"

void main() {
  int pos;
  int vga_offset;
  port_byte_out(0x3d4, 14);
  pos = port_byte_in(0x3d5);
  pos = pos << 8;

  port_byte_out(0x3d4, 15);
  pos += port_byte_in(0x3d5);
  vga_offset = pos * 2;

  char* video_memory = 0xb8000;
  video_memory[vga_offset] = 'X';
  video_memory[vga_offset+1] = 0x0f;
}
