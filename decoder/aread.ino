
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

#include "adc.h"
#include "window.h"
#include "goertzel.h"

volatile unsigned long lastTime;
void setup()
{
  Serial.begin(115200); // set baudrate
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

int totmin = 2000;
int totmax = 0;
int tottime = 0;
int absmin = 2000;
int absmax = 0;
int abstime = 0;

void printToneMag(unsigned long delta, float mag)
{
  // char buffer[100], mags[10];
  //   dtostrf(mag, 4, 2, mags);
  //   sprintf(buffer, "%6lu %s", delta, mags);
  //   Serial.println(buffer);
}

char morseCode[10];
void resetLetter()
{
  memset(morseCode, 0, sizeof(morseCode));
}
void printMorseCode()
{
  char letter = findLetter(morseCode);

  Serial.print(">>> ");
  Serial.print(morseCode);
  Serial.print(" ==> ");
  Serial.println(letter);

  resetLetter();
}
void printMorseSpace()
{

  Serial.println(">>>  (space)");

  resetLetter();
}
void processToneLen(int toneLen)
{
  char *text;
  // avoid buffer overflow
  if (strlen(morseCode) >= sizeof(morseCode) - 1)
  {
    return;
  }
  // 20 wpm: 4 and 3
  // 30 wpm:
  if (toneLen == 0)
  {
    return;
  }
  else if (toneLen >= 3)
  {
    text = "-";
  }
  else if (toneLen <= 2)
  {
    text = ".";
  }
  else
  {
    text = "*";
  }
  strcat(morseCode, text);

  // Serial.print(toneLen);
  // Serial.print(" ");
  // Serial.println(text);
}



volatile int toneLen = 0;
void loop()
{
  unsigned long loopStart = millis();

  int *data = adcWaitForCompletion();
  data = windowApply(data);

  gRun(ADC_BUFSIZ, data);

  // Serial.println("======");
  {

    float mag = getMag();
    unsigned long delta = loopStart - lastTime;

    if (delta > 500)
    {
      // tone length detection reset
      processToneLen(toneLen);
      if (toneLen > 0)
      {
        // Serial.println("<<<<");
        printMorseCode();
      }
      toneLen = 0;
    }

    if (mag > 1000)
    {
      lastTime = loopStart;

#if 0
      printToneMag(delta, mag);

#if 0
      if(delta > 300) {
        printMorseSpace();
      }
      if(delta < 40L || delta > 300) {
        toneLen++;
      } else if(delta < 120L) {
        processToneLen(toneLen);
        toneLen = 1;
      } else if (delta > 150L) {
        // Serial.println("<<<<");
        processToneLen(toneLen);
        printMorseCode();
        toneLen = 0;
      } else {
        processToneLen(toneLen);
        toneLen = 0;
      }
#else
      if(delta > 300) {
        printMorseSpace();
      }
      if(delta < 40L || delta > 300) {
        toneLen++;
      } else if(delta < 110L) {
        processToneLen(toneLen);
        toneLen = 1;
      } else if (delta > 130L) {
        // Serial.println("<<<<");
        processToneLen(toneLen);
        printMorseCode();
        toneLen = 0;
      } else {
        processToneLen(toneLen);
        toneLen = 0;
      }
#endif
#endif
      float deltaNormalized = (float)delta / 30.;
      char type = mapType(deltaNormalized);

      // Serial.println("======");
      Serial.print(type);
      Serial.print(" ");
      Serial.print(deltaNormalized);
      Serial.print(" ");
      Serial.print(delta);
      Serial.print(" ");
      // Serial.println(loopStart);
      // Serial.println(lastTime);
      Serial.println(mag);
    }
  }

  // Serial.println(lastTime);

#if 0
// sensitivity
#define MIN(x, y) x > y ? y : x
#define MAX(x, y) x > y ? x : y
  int min = 2000;
  int max = 0;
  for (int i = 0; i < ADC_BUFSIZ; i++)
  {
    min = MIN(min, data[i]);
    max = MAX(max, data[i]);
    absmin = MIN(absmin, data[i]);
    absmax = MAX(absmax, data[i]);
    totmin = MIN(totmin, data[i]);
    totmax = MAX(totmax, data[i]);
    // Serial.print(data[i]);
    // Serial.print(window[i]);
    // Serial.print(" ");
  }
  Serial.print(max-min);
  Serial.print(" ");
  Serial.print(absmax-absmin);
  Serial.print(" ");
  Serial.print(totmax-totmin);
  Serial.println();

  int curtime = millis();
  if(curtime - abstime > 300) {
    abstime = curtime;
    absmin = 2000;
    absmax = 0;
  }
  if(curtime - tottime > 1000) {
    tottime = curtime;
    totmin = 2000;
    totmax = 0;
  }
#endif
  // reset the ADC for the next sample
  adcReset();
  // reset the the Goertzel alg.
  gReset();
}
#endif