
#if 0
void setup() {
  Serial.begin(115200);
  Serial.println();
}

void loop(){
  int value = analogRead(0);
  Serial.println(analogPinToChannel(0));
  delay(100);
}

#else

#include "adc.h"

void setup()
{
  Serial.begin(115200); // set baudrate
  Serial.println();

  // ---- setup the ADC interrupt driven audio read
  adcInit();
  adcReset();
}


void loop()
{
    adcWaitForCompletion();
    adcReset();

    Serial.println("======");
    for (int i = 0; i < ADC_BUFSIZ; i++)
    {
      Serial.print(adcBuffer[i]);
      // Serial.print(ADMUX);
      Serial.print(" ");
    }
    Serial.println();

    // reset the ADC for the next sample
    //adcReset();
}
#endif