/*
 * Copyright (c) 2023 Uwe Meding -- All Rights Reserved.
 *
 * 2023-03-28 uwe <uwe@uwemeding.com> -- created
 *  - LED helpers
 */

#include "morse.h"

// --- LED helpers
void ledInit() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RXLED, OUTPUT);
  pinMode(TXLED, OUTPUT);
  ledOff();
}
void ledOff() {
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(RXLED, HIGH);
  digitalWrite(TXLED, HIGH);
}
void ledOn() {
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(RXLED, LOW);
  digitalWrite(TXLED, LOW);
}
int BLINK_STATE = 0;
/*
   Blink the RX/TX LED's alternately
*/
void alternateBlink() {
  if (BLINK_STATE) {
    digitalWrite(RXLED, LOW);
    digitalWrite(TXLED, HIGH);
  } else {
    digitalWrite(RXLED, HIGH);
    digitalWrite(TXLED, LOW);
  }
  BLINK_STATE = !BLINK_STATE;
  delay(100);
}
