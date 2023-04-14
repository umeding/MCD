/*
 * Copyright (c) 2023 Uwe Meding -- All Rights Reserved.
 *
 * 2023-03-27 uwe <uwe@uwemeding.com> -- created
 *   - wrapper around the LCD display
 */

#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27
#define LCD_ROWS 4
#define LCD_COLS 20
#define LCD_MAX_BUF (LCD_COLS * LCD_ROWS)

#define SERIAL_OUTPUT 1

static LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

void lcdInit() {
  lcd.init();
  lcd.backlight();
}

void lcdStatusMessage(char *message) {
#if 0
  char buffer[LCD_COLS + 1];
  sprintf(buffer, "%-20.20s", message);

  lcd.setCursor(0, 0);  // first line
  lcd.print(buffer);
#if SERIAL_OUTPUT
  Serial.println(buffer);
#endif
#endif
}

// -------------------------------------------------------------
char lcdBuffer[LCD_MAX_BUF];
int lcdBufferPos;
int lcdBufferDirty = 0;

void lcdClear() {
  // fill buffer with spaces
  memset(lcdBuffer, 0x20, sizeof(lcdBuffer));
  lcdBufferPos = 0;
  lcdBufferDirty = 1;
}

void lcdPrint(char c) {
  if (lcdBufferPos < LCD_MAX_BUF) {
    lcdBuffer[lcdBufferPos++] = c;
  } else {
    // shift the lcdBuffer to left
    memmove(lcdBuffer, &lcdBuffer[1], LCD_MAX_BUF - 1);
    lcdBuffer[LCD_MAX_BUF - 1] = c;
  }

  lcdBufferDirty = 1;
  // lcdEmit();
}

void lcdPrintStr(char *string) {
  char c;
  while (c = *string++) {
    lcdPrint(c);
  }
}

// char local[LCD_MAX_BUF + 1];  // +1 for the EOL
void lcdEmit() {
  if (lcdBufferDirty) {
    // do the internal buffer -> display mapping
    // memcpy(&local[0 * LCD_COLS], &lcdBuffer[0 * LCD_COLS], LCD_COLS);
    // memcpy(&local[2 * LCD_COLS], &lcdBuffer[1 * LCD_COLS], LCD_COLS);
    // memcpy(&local[1 * LCD_COLS], &lcdBuffer[2 * LCD_COLS], LCD_COLS);
    // memcpy(&local[3 * LCD_COLS], &lcdBuffer[3 * LCD_COLS], LCD_COLS);

    // local[sizeof(local) - 1] = '\0';
    // Serial.println(local);

    // lcd.print(local);
    doLcdEmit(0, 0);
    doLcdEmit(2, 1);
    doLcdEmit(1, 2);
    doLcdEmit(3, 3);

    lcdBufferDirty = 0;
  }
}
char local[LCD_COLS + 1];  // +1 for the EOL
inline void doLcdEmit(int src, int trg) {
  memcpy(&local[0], &lcdBuffer[trg * LCD_COLS], LCD_COLS);
  local[sizeof(local) - 1] = '\0';

  lcd.setCursor(0, src);
  lcd.print(local);
}
