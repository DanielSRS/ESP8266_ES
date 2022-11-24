#include <stdio.h>
#include <string.h>

void clear_display(void) {
  printf("--Clear display--");
}

void init_display(void) {
  printf("--Init display--");
}
void write_char(char c) {
  printf("--%c--", c);
}
