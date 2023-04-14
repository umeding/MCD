
/*
 * Copyright (c) 2023 Uwe B. Meding -- All Rights Reserved
 *
 * 2023-04-14 uwe <uwe@uwemeding.com> -- created
 *  - kill switch functionality in case we fuck up the code
 *    and create a situation where we cannot program the
 *    Arduino anymore.
 *
 * -----------------
 * Current Arduino Micro/Leonardo kill count:
 * as of 2023-04-14: 5
 *
 */
#include "morse.h"

void setup() {
  // kill switch check
  pinMode(KILL_SWITCH_PIN, INPUT);
  digitalWrite(KILL_SWITCH_PIN, HIGH);

  if (digitalRead(KILL_SWITCH_PIN) == LOW) {
    // regular start
    doSetup();
  } else {
    // kill switch engaged
    // ...
  }
}

void loop() {
  if (digitalRead(KILL_SWITCH_PIN) == LOW) {
    doLoop();
  } else {
    // kill switch engaged
    alternateBlink();
  }
}