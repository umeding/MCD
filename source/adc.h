/*
 * Copyright (c) 2023 Uwe Meding -- All Rights Reserved.
 *
 * 2023-02-03 uwe <uwe@uwemeding.com> -- created
 */

// we use pin A0
#define ADC_PIN 0

// the ADC buiffer
#define ADC_BUFSIZ 256
extern volatile int adcBuffer[ADC_BUFSIZ];
