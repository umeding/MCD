/*
 * Copyright (c) 2023 Uwe Meding -- All Rights Reserved.
 *
 * 2023-02-03 uwe <uwe@uwemeding.com> -- created
 *  * the pin/system settings are for the
 *    Arduino Micro/Leonardo board
 */

// ========================================================
// we use pin A0 to read the analog signal
#define ADC_PIN 0

// the ADC buffer
#define ADC_BUFSIZ 205
extern volatile int internal__adcBuffer[ADC_BUFSIZ];
extern volatile int adcBuffer[ADC_BUFSIZ];

extern int *adcWaitForCompletion();

// ========================================================
// the signal windowing
#include <math.h>

#ifndef PI
#define PI 3.141592653589793
#endif

// Different window types the code supports
#define RECTANGLE 0
#define BLACKMAN 1
#define KAISER 2

// Select a window type
#define WINDOWING BLACKMAN

// General window parameters
#if WINDOWING == KAISER
// A beta of 14 gives us pretty smooth magnitudes
#define BETA 14.
#endif

extern int *windowApply(int *);

// ========================================================
// we use the Goertzel algorithm for the signal analysis
// this allows us to peel out the dits and dahs.

extern void gInit(int nsamples, float fsample, float ftarget);
extern void gReset();
extern void gRun(int nsamples, int samples[]);
extern float getMag();

/*
 * The sampling frq will be 8928 on a 16 mhz arduino
 * without any prescaler etc
 * because we need the tone in the center of the bins
 * you can set the tone to 496, 558, 744 or 992
 * then n the number of samples which give the bandwidth
 * can be (8928 / tone) * 1 or 2 or 3 or 4 etc
 * init is 8928/558 = 16 *4 = 64 samples
 * try to take n = 96 or 128 ;o)
 * 48 will give you a bandwidth around 186 hz
 * 64 will give you a bandwidth around 140 hz
 * 96 will give you a bandwidth around 94 hz
 * 128 will give you a bandwidth around 70 hz
 * BUT remember that high n take a lot of time
 * so you have to find the compromise - i use 48
 */
#define FSAMPLE 8000.
#define FTARGET 700.

// ========================================================
// Parameters for the morse code extraction
//

// 40ms is slightly more than the processing takes
// measured values (16MHz Ardunio) = ~35ms
#define NORMALIZE_TIMING(x) ((float)x / 40.)

// Internal state machine
#define MORSE_SYMBOL 'S'
#define MORSE_SYMBOL_CONT '*'
#define MORSE_LETTER 'L'
#define MORSE_WORD_SPACE 'W'
#define MORSE_DONE 'F'

// Variois serial print detail
#define MORSE_PRINT_TIMING 0
#define MORSE_PRINT_CODE 0
#define MORSE_PRINT_LETTER 1