/*
 * Copyritght (c) 2023 Uwe B. Meding -- All Rights Reserved.
 *
 * 2023-02-24 uwe <uwe@uwemeding.com> -- created
 */

#include "goertzel.h"

// Gortzel algorithm: https://en.wikipedia.org/wiki/Goertzel_algorithm

float coeff;
float S1 = 0;
float S2 = 0;

#ifndef PI
#define PI 3.141592653589793
#endif

/**
 * Initialize the Goertzel algorithm.
 *
 * @param nsamples number of samples
 * @param fsample sample frequency
 * @param ftarget target frequency
 */
void gInit(int nsamples, float fsample, float ftarget)
{
  int k = (int)(0.5 + ((nsamples * ftarget) / fsample));
  float omega = (2.0 * PI * k) / nsamples;
  coeff = 2.0 * cos(omega);
  // Serial.println("fsample=%f nsamples=%d ftarget=%f\n", fsample, nsamples, ftarget);
  // Serial.println("k=%d coeff=%f\n", k, coeff);
}

/**
 * Reset the Goertzel algorithm for the next set of samples.
 */
void gReset()
{
  S1 = 0.;
  S2 = 0.;
}

/**
 * Run the Goertzel algorithm.
 *
 * @param nsamples number of samples
 * @param samples array of samples
 */
void gRun(int nsamples, int samples[])
{
  for (int i = 0; i < nsamples; i++)
  {
    float s;
    s = coeff * S1 - S2 + (float)samples[i];
    S2 = S1;
    S1 = s;
  }
}

/**
 * Get the squared magnitude.
 *g
 * @return float the magnitude
 */
float getMagSquared()
{
  float mags = (S1 * S1) + (S2 * S2) - coeff * S1 * S2;
  return mags;
  // magnitude = sqrt(magnitudeSquared);
}

/**
 * Get the magnitude.
 *
 * @return float  the magnitude
 */
float getMag()
{
  return sqrt(getMagSquared());
}
