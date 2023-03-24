/*
 * Copyright (c) 2023 Uwe Meding -- All Rights Reserved.
 *
 * 2023-02-24 uwe <uwe@uwemeding.com> -- created
 */

#ifndef GOERTZEL_H_INCLUDED
#define GOERTZEL_H_INCLUDED 1

#include "window.h"

extern void gInit(int nsamples, float fsample, float ftarget);
extern void gReset();
extern void gRun(int nsamples, int samples[]);
extern float getMag();

///////////////////////////////////////////////////////////
// The sampling frq will be 8928 on a 16 mhz arduino     //
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

#define FSAMPLE 8000.
#define FTARGET 700.

#endif
