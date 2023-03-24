/*
 * Copyright (c) 2023 Uwe Meding -- All Rights Reserved.
 *
 * 2023-02-21  uwe <uwe@uwemeding.com> -- created
 */
#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED 1


#include <math.h>
#include "adc.h"

#ifndef PI
#define PI 3.141592653589793
#endif

// The  window values
float window[ADC_BUFSIZ];

// Different window types
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


extern int* windowApply(int*);

#endif
