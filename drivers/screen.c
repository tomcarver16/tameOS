#include "ports.h"
#include "screen.h"

/*
 * Declare Private Functions
 */
int get_cursor_offset();
void set_cursor_offset();
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

/*
 * Public Functions
 */

void kprint_offset(char* msg, int col, int row) {
  int offset;
  int i = 0;
  if (col >= 0 && row >= 0) {
    offset = get_offset(col, row);
  } else {
    offset = get_cursor_offset();
    row = get_offset_row(offset);
    col = get_offset_col(offset);
  }

  while (msg[i] != 0) {
    offset = print_char(msg[i++], col, row, WHITE_ON_BLACK); 
    row = get_offset_row(offset);
    col = get_offset_col(offset);
  }
}

void kprint(char* msg) {
  kprint_offset(msg, -1, -1);
}

/*
 * Private Functions
 */
int print_char(char c, int col, int row, char attr) {
  int offset;
  unsigned char* video_memory = (unsigned char*) VIDEO_ADDRESS;
  if (!attr) attr = WHITE_ON_BLACK;

  if (col >= MAX_COLS || row >= MAX_ROWS) {
    video_memory[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
    video_memory[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;
    return get_offset(col, row);
  }

  if (col >= 0 && row >= 0) {
    offset = get_offset(col, row);
  } else {
    offset = get_cursor_offset();
  }

  if (c == '\n') {
    row = get_offset_row(offset);
    offset = get_offset(0, row+1l);
  } else {
    video_memory[offset] = c;
    video_memory[offset+1] = attr;
    offset += 2;
  }

  set_cursor_offset(offset);
  return offset;
}

int get_cursor_offset() {
  int offset;
  port_byte_out(REG_SCREEN_CTRL, 14);
  offset = port_byte_in(REG_SCREEN_DATA) << 8;
  offset += port_byte_in(REG_SCREEN_DATA);
  return offset * 2;
}

void set_cursor_offset(int offset) {
  offset /= 2;
  port_byte_out(REG_SCREEN_CTRL, 14);
  port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
  port_byte_out(REG_SCREEN_CTRL, 15);
  port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xFF));
}

void clear_screen() {
  int screen_size = MAX_COLS * MAX_ROWS;
  int i;
  char* screen = VIDEO_ADDRESS;

  for (i = 0; i < screen_size; i++) {
    screen[i*2] = ' ';
    screen[i*2+1] = WHITE_ON_BLACK;
  }

  set_cursor_offset(get_offset(0, 0));
}

int get_offset(int col, int row) {
  return 2 * (row * MAX_COLS + col);
}

int get_offset_row(int offset) {
  return offset / (2 * MAX_COLS);
}

int get_offset_col(int offset) {
  return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2;
}

