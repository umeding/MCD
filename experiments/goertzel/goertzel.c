#include <stdio.h>
#include <math.h>

///////////////////////////////////////////////////////////
// The sampling frq will be 8928 on a 16 mhz             //
// without any prescaler etc                             //
// because we need the tone in the center of the bins    //
// you can set the tone to 496, 558, 744 or 992          //
// then n the number of samples which give the bandwidth //
// can be (8928 / tone) * 1 or 2 or 3 or 4 etc           //
// init is 8928/558 = 16 *4 = 64 samples                 //
// try to take n = 96 or 128 ;o)                         //
// 48 will give you a bandwidth around 186 hz            //
// 64 will give you a bandwidth around 140 hz            //
// 96 will give you a bandwidth around 94 hz             //
// 128 will give you a bandwidth around 70 hz            //
// BUT remember that high n take a lot of time           //
// so you have to find the compromice - i use 48         //
///////////////////////////////////////////////////////////

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
  printf("fsample=%f nsamples=%d ftarget=%f\n", fsample, nsamples, ftarget);
  printf("k=%d coeff=%f\n", k, coeff);
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

// #define FSAMPLE 8928.0
// #define FTARGET 700
// #define NSAMPLES 128
#define FSAMPLE 8000.
#define FTARGET 941.
#define NSAMPLES 205
int samples[NSAMPLES];

float window[NSAMPLES];

#define RECTANGLE 0
#define BLACKMAN 1
#define KAISER 2
#define WINDOWING BLACKMAN

#if WINDOWING == RECTANGLE
#define WINDOWFUNCTION() rectWindowInit()

void rectWindowInit()
{
  printf("rectangle: nsamples=%d\n", NSAMPLES);
  for (int i = 0; i < NSAMPLES; i++)
  {
    window[i] = 1.;
  }
}

#elif WINDOWING == BLACKMAN

void blackmanWindowInit(int nsamples)
{
  printf("blackman: nsamples=%d", nsamples);
  for (int i = 0; i < NSAMPLES; i++)
  {
    window[i] = (0.426591 - 0.496561 * cos((2.0 * PI * i) / NSAMPLES) + 0.076848 * cos((4.0 * PI * i) / NSAMPLES));
    // printf("%4d: %13.6f\n", i, window[i]);
  }
}
#define WINDOWFUNCTION() blackmanWindowInit(NSAMPLES)

#elif WINDOWING == KAISER

/**
 * Zeroth order Bessel function. Code lifted and slightly modified from
 * https://www.atnf.csiro.au/computing/software/gipsy/sub/bessel.c
 *
 * @param x input value
 * @return float calculated
 */
float besselI0(float x)
{
  float ax, ans;
  float y;

  if ((ax = fabs(x)) < 3.75)
  {
    y = x / 3.75, y = y * y;
    ans = 1.0 + y * (3.5156229 + y * (3.0899424 + y * (1.2067492 + y * (0.2659732 + y * (0.360768e-1 + y * 0.45813e-2)))));
  }
  else
  {
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
 * beta Window shape
 * 0 Rectangular
 * 5 Similar to a Hamming
 * 6 Similar to a Hann
 * 8.6 Similar to a Blackman
 * 14 is a pretty good starting point
 *
 * @param nsamples
 * @param beta
 */
void kaiserWindowInit(int nsamples, float beta)
{
  printf("kaiser: nsamples=%d beta=%f\n", nsamples, beta);
  float A = (nsamples - 1) * (nsamples - 1);
  float B = besselI0(beta);
  for (int i = 0; i < NSAMPLES; i++)
  {
    float C = 2. * (float)i / (float)(nsamples - 1) - 1.;
    window[i] = besselI0(beta * sqrt(1 - C * C)) / besselI0(beta);
    // printf("%4d: %13.6f\n", i, window[i]);
  }
}
#define WINDOWFUNCTION() kaiserWindowInit(NSAMPLES, 8.6)

#else
#error Bad window selected
#endif

void applyWindow()
{
  for (int i = 0; i < NSAMPLES; i++)
  {
    samples[i] *= window[i];
  }
}

void generate(float ftarget)
{
  float step = ftarget * 2. * PI / FSAMPLE;
  for (int i = 0; i < NSAMPLES; i++)
  {
    samples[i] = (int)(100 * sin(i * step) + 100);
    // apply the window
    samples[i] *= window[i];
    // printf("%4d: %d\n", i, samples[i]);
  }
}

void runG(float fread)
{
  generate(fread);
  gReset();
  gRun(NSAMPLES, samples);
  float mag = getMag();
  printf("fread=%13.6f mag=%13.6f\n", fread, mag);
}
int main(int ac, char **av)
{
  WINDOWFUNCTION();
  gInit(NSAMPLES, FSAMPLE, FTARGET);
  for (float fread = FTARGET - 300.; fread < FTARGET + 300.; fread += 15.)
  {
    runG(fread);
    // break;
  }
}