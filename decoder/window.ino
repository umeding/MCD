/*
 * Copyright (c) 2023 Uwe Meding -- All Rights Reservec
 *
 * 2023-02-21 uwe <uwe@uwemeding.com> -- created
 */

#include "morse.h"

float window[ADC_BUFSIZ];

#if WINDOWING == RECTANGLE
// ===============================================================
// ===== RECTANGLE WINDOW =========================================

/**
 * Rectangle window, not much to do, all window values are 1
 */
void windowInit() {
  for (int i = 0; i < ADC_BUFSIZ; i++) {
    window[i] = 1.;
  }
}

#elif WINDOWING == BLACKMAN
// ===============================================================
// ===== BLACHMAN WINDOW =========================================

/**
 * Calculate the Blackman style window values.
 *
 * @param nsamples number of samples
 */
inline void blackmanWindowInit(int nsamples) {
  for (int i = 0; i < nsamples; i++) {
    window[i] = (0.426591 - 0.496561 * cos((2.0 * PI * i) / nsamples) + 0.076848 * cos((4.0 * PI * i) / nsamples));
  }
}

// Generic window initialization
void windowInit() {
  blackmanWindowInit(ADC_BUFSIZ);
}

#elif WINDOWING == KAISER
// ===============================================================
// ===== KAISER WINDOW ===========================================

/**
 * Zeroth order Bessel function. Code lifted (and slightly modified) from
 * https://www.atnf.csiro.au/computing/software/gipsy/sub/bessel.c
 *
 * @param x input value
 * @return float calculated
 */
float besselI0(float x) {
  float ax, ans;
  float y;

  if ((ax = fabs(x)) < 3.75) {
    y = x / 3.75, y = y * y;
    ans = 1.0 + y * (3.5156229 + y * (3.0899424 + y * (1.2067492 + y * (0.2659732 + y * (0.360768e-1 + y * 0.45813e-2)))));
  } else {
    y = 3.75 / ax;
    ans = (exp(ax) / sqrt(ax)) * (0.39894228 + y * (0.1328592e-1 + y * (0.225319e-2 + y * (-0.157565e-2 + y * (0.916281e-2 + y * (-0.2057706e-1 + y * (0.2635537e-1 + y * (-0.1647633e-1 + y * 0.392377e-2))))))));
  }
  return ans;
}

/**
 * Calculate the Kaiser-Bessel window.
 * Details: https://en.wikipedia.org/wiki/Kaiser_window
 *
 * as beta goes up, the window narrows
 * beta window shapes:
 * 0   - rectangular
 * 5   - similar to a Hamming
 * 6   - similar to a Hann
 * 8.6 - similar to a Blackman
 * 14  - is a pretty good starting point
 *
 * @param nsamples number of samples
 * @param beta Kaiser beta value
 */
inline void kaiserWindowInit(int nsamples, float beta) {
  float A = (nsamples - 1) * (nsamples - 1);
  float B = besselI0(beta);
  for (int i = 0; i < nsamples; i++) {
    float C = 2. * (float)i / (float)(nsamples - 1) - 1.;
    window[i] = besselI0(beta * sqrt(1 - C * C)) / besselI0(beta);
  }
}

// Generic window initialization
void windowInit() {
  kaiserWindowInit(ADC_BUFSIZ, BETA);
}

#else
#error Bad window selected
#endif

/**
 * Apply the window to the data sample.
 */
inline void applyTheWindow(int nsamples, int *samples) {
  for (int i = 0; i < nsamples; i++) {
    *samples++ *= window[i];
  }
}
int *windowApply(int *samples) {
  int *org = samples;
  applyTheWindow(ADC_BUFSIZ, samples);
  return org;
}