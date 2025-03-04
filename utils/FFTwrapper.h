/*
  ZynAddSubFX - a software synthesizer

  FFTwrapper.h  -  A wrapper for Fast Fourier Transforms
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
*/

#pragma once

#include <fftw3.h>
#include <complex>

typedef double fftw_real;
typedef std::complex<fftw_real> fft_t;


//namespace padfft {

	/** A wrapper for the FFTW library (Fast Fourier Transforms) */
class FFTwrapper
{
public:
	/**Constructor
	 * @param fftsize The size of samples to be fed to fftw*/
	FFTwrapper(int fftsize_);
	/**Destructor*/
	~FFTwrapper();
	/**Convert Samples to Frequencies using Fourier Transform
	 * @param smps Pointer to Samples to be converted; has length fftsize_
	 * @param freqs Structure FFTFREQS which stores the frequencies*/
	void smps2freqs(const float *smps, fft_t *freqs);
	void freqs2smps(const fft_t *freqs, float *smps);
private:
	int fftsize;
	fftw_real    *time;
	fftw_complex *fft;
	fftw_plan     planfftw, planfftw_inv;
}
;
	
/*
 * The "std::polar" template has no clear definition for the range of
 * the input parameters, and some C++ standard library implementations
 * don't accept negative amplitude among others. Define our own
 * FFTpolar template, which works like we expect it to.
 */
template<class _Tp>
	std::complex<_Tp>
	FFTpolar(const _Tp& __rho, const _Tp& __theta = _Tp(0))
	{
		_Tp __x = __rho * cos(__theta);
		if (std::isnan(__x))
			__x = 0;
		_Tp __y = __rho * sin(__theta);
		if (std::isnan(__y))
			__y = 0;
		return std::complex<_Tp>(__x, __y);
	}
	
void FFT_cleanup();

//}



