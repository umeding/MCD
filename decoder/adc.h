/*
 * Copyright (c) 2023 Uwe Meding -- All Rights Reserved.
 *
 * 2023-02-03 uwe <uwe@uwemeding.com> -- created
 */
#ifndef ADC_H_INCLUDED
#define ADC_H_INCLUDED 1

// we use pin A0
#define ADC_PIN 0

// the ADC buffer
#define ADC_BUFSIZ 205
extern volatile int internal__adcBuffer[ADC_BUFSIZ];
extern volatile int adcBuffer[ADC_BUFSIZ];

extern int *adcWaitForCompletion();

#endif
