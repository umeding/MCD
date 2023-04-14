/*
 * Copyright (c) 2023 Uwe B. Meding -- All Rights Reserved.
 *
 * 2023-04-14 uwe <uwe@uwemeding.com> -- created
 *
 */

#include "morse.h"

// Last time we the loop ran
volatile unsigned long lastTime;

/**
 * Morse decoder setup.
 */
inline void doSetup() {
  Serial.begin(115200);  // set baudrate
  Serial.println();

#if MORSE_PRINT_LCD
  lcdInit();
  lcdClear();
#endif

  lastTime = millis();

  // ---- setup signal processing
  windowInit();

  // ---- setup the ADC interrupt driven audio read
  adcInit();
  adcReset();

  gInit(ADC_BUFSIZ, FSAMPLE, FTARGET);
  gReset();

  // lcdStatusMessage("Morse Decoder");
}

/**
 * The main loop.
 */
inline void doLoop() {
  // get the current timestamp
  unsigned long loopStart = millis();

  // get stuff from the ADC
  int *data = adcWaitForCompletion();

  // Signal analysis: Goertzel code
  data = windowApply(data);
  gRun(ADC_BUFSIZ, data);

  // Morse code analysis
  float mag = getMag();
  unsigned long delta = loopStart - lastTime;

  // we haven't heard anything in 0.5sec -> flush whatever is pending
  if (delta > 500) {
    morseFlush();
  }

  // we have a relevant magnitude at the frequency we are tuned in at
  if (mag > 1000) {
    lastTime = loopStart;

    morseBeep(delta, mag);
  }

#if MORSE_PRINT_LCD
  lcdEmit();
#endif

  // reset the ADC for the next sample
  adcReset();
  // reset the the Goertzel alg.
  gReset();
}
