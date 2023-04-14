#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define LCD_ROWS 4
#define LCD_COLS 20
#define LCD_MAX_BUF (LCD_COLS * LCD_ROWS)

char lcdBuffer[LCD_MAX_BUF];
int lcdBufferPos;
int lcdBufferDirty = 0;

void clear() {
  // fill buffer with spaces
  memset(lcdBuffer, 0x20, sizeof(lcdBuffer));
  lcdBufferPos = 0;
  lcdBufferDirty = 1;
}

void print(char c) {
  if(lcdBufferPos < LCD_MAX_BUF) {
    lcdBuffer[lcdBufferPos++] = c;
  } else {
    // shift the lcdBuffer to left
    memmove(lcdBuffer, &lcdBuffer[1], LCD_MAX_BUF -1);
    lcdBuffer[LCD_MAX_BUF-1] = c;
  }

  emit();
}

void printStr(char *string) {
  char c;
  while(c = *string++) {
    print(c);
  }
}

void emit() {
  char local[LCD_MAX_BUF+1]; // +1 for the EOL
  printf("<%80.80s>\n", lcdBuffer);

  // do the internal buffer -> display mapping
  memcpy(&local[0 * LCD_COLS], &lcdBuffer[0 * LCD_COLS], LCD_COLS);
  memcpy(&local[2 * LCD_COLS], &lcdBuffer[1 * LCD_COLS], LCD_COLS);
  memcpy(&local[1 * LCD_COLS], &lcdBuffer[2 * LCD_COLS], LCD_COLS);
  memcpy(&local[3 * LCD_COLS], &lcdBuffer[3 * LCD_COLS], LCD_COLS);

  local[sizeof(local)-1] = '\0';
  printf("[%s]\n", local);
}

int main() {
  printf("lcdBuffer size %ld\n", sizeof(lcdBuffer));

  char *someString = "laberlall schlabber scheisse mit reisse blubber glork kotz mit rotz";
  clear();
  printStr(someString);
  printStr(someString);
  emit();

  return 0;
}
