
#if 0
int selectorPin = 4;
int selectionCount = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();

  attachInterrupt(digitalPinToInterrupt(7), selectionPressed, CHANGE);
}

void loop(){
  // delay(1000);
  // Serial.print("selection: ");
  // Serial.println(selectionCount);
}

void selectionPressed() {
  selectionCount++;
  Serial.print("selection: ");
  Serial.println(selectionCount);
}

#else

#include "morse.h"

volatile unsigned long lastTime;


void setup() {
  Serial.begin(115200);  // set baudrate
  Serial.println();

  lastTime = millis();

  // ---- setup signal processing
  windowInit();

  // ---- setup the ADC interrupt driven audio read
  adcInit();
  adcReset();

  gInit(ADC_BUFSIZ, FSAMPLE, FTARGET);
  gReset();
}

/**
 * The main loop.
 */
void loop() {
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

  if (delta > 500) {
    morseFlush();
  }

  if (mag > 1000) {
    lastTime = loopStart;

    morseBeep(delta, mag);
  }

  // reset the ADC for the next sample
  adcReset();
  // reset the the Goertzel alg.
  gReset();
}
#endif