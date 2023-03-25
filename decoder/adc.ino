/*
 * Copyright (c) 2023 Uwe Meding -- All Rights Reserved
 *
 * 2023-02-03 uwe <uwe@uwemeding.com> -- created
 */
#include "morse.h"

// the buffers
// volatile byte adcChannel;
volatile int adcBuffer[ADC_BUFSIZ];
volatile int internal__adcBuffer[ADC_BUFSIZ];
volatile int adcBufPos;

/*
 * ADC complete ISR. Converted data samples from the ADC are
 * stored in our buffer until it is full. Then we stop the
 * conversion.
 */
ISR(ADC_vect)
{
  if (adcBufPos >= ADC_BUFSIZ)
  {
    ADCSRA = 0; // turn off ADC
  }
  else
  {
    internal__adcBuffer[adcBufPos++] = ADC;
  }
}

EMPTY_INTERRUPT(TIMER1_COMPB_vect);

/*
 * Initialize the interrupt driven ADC.
 */
inline void adcInit()
{
  // reset Timer 1
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  TCCR1B = bit(CS11) | bit(WGM12); // CTC, prescaler of 8
  TIMSK1 = bit(OCIE1B);

  // PWM duty cycle:  20 uS - sampling frequency 50 kHz
  OCR1A = 39;
  OCR1B = 39;

  // setup the ADC multiplexer
  ADMUX = bit(REFS0) | analogPinToChannel(ADC_PIN);
  ADCSRB = bit(ADTS0) | bit(ADTS2); // Timer/Counter1 Compare Match B
}

/*
 * Reset the ADC for the next sample
 */
inline void adcReset()
{
  // reset the sample counter
  adcBufPos = 0;
  // turn ADC on: we also want an interrupt on completion
  ADCSRA = bit(ADEN) | bit(ADIE) | bit(ADIF);

  // set the prescaler
  //
  // bits set: ADPS2 | ADPS1 | ADPS0 -> 2^7 -> prescale=128.
  // Arduino takes 13 ADC cycles for a single conversion
  // 16MHz (CPU) / 128 / 13 -> ~9600Hz sample freq
  ADCSRA |= bit(ADPS2) | bit(ADPS1) | bit(ADPS0);

  // turn on automatic triggering
  ADCSRA |= bit(ADATE);
}

/*
 * Wait for our ADC buffer to fill up. Once the buffer is full, the ADC is stopped
 * and must be reset for the next sample.
 *
 * @return the sample buffer
 */
int *adcWaitForCompletion()
{
  while (adcBufPos < ADC_BUFSIZ)
  {
    // stay here until the buffer is full
  }
  return (int *)memcpy(adcBuffer, internal__adcBuffer, ADC_BUFSIZ * sizeof(int));
}