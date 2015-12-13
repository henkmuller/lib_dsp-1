// Copyright (c) 2015, XMOS Ltd, All rights reserved

#ifndef LIB_DSP_TRANSFORMS
#define LIB_DSP_TRANSFORMS

extern const int lib_dsp_sine_8[];
extern const int lib_dsp_sine_16[];
extern const int lib_dsp_sine_32[];
extern const int lib_dsp_sine_64[];
extern const int lib_dsp_sine_128[];
extern const int lib_dsp_sine_256[];
extern const int lib_dsp_sine_512[];
extern const int lib_dsp_sine_1024[];
extern const int lib_dsp_sine_2048[];
extern const int lib_dsp_sine_4096[];
extern const int lib_dsp_sine_8192[];

#define FFT_SINE0(N) lib_dsp_sine_ ## N
#define FFT_SINE(N) FFT_SINE0(N)

typedef struct
{
    int re;
    int im;
}
lib_dsp_fft_complex_t;

/** This function takes a the output from the function lib_dsp_fft_forward_complex and
 * reorders the data to form two frequency domain arrays.
 *
 * Note: pts[0].im will contain the frequency information for the real data in the N/2 index.
 *
 * \param pts   Array of complex input points
 *
 * \param N     number of points. Must be a power of 2, both re and im should be N long
 */
void lib_dsp_fft_reorder_two_real_inputs(lib_dsp_fft_complex_t pts[], unsigned N);

/** This function takes a frequency domain array where the data is in the form of the output
 * from  lib_dsp_fft_reorder_two_real_inputs() and rebuilds the full array ready for an inverse
 * FFT.
 *
 * \param pts   Array of complex input points
 *
 * \param N     number of points. Must be a power of 2, both re and im should be N long
 */
void lib_dsp_fft_rebuild_one_real_input(lib_dsp_fft_complex_t pts[], unsigned N);

/** This function preforms index bit reversing on the the arrays around prior to computing an FFT. A
 * calling sequence for a forward FFT involves lib_dsp_fft_bit_reverse() followed by
 * fftForward(), and for an inverse FFT it involves fftTwiddle() followed
 * by fftInverse(). In some cases twiddling can be avoided, for example
 * when computing a convolution.
 *
 * \param pts   Array of complex input points
 *
 * \param N     number of points. Must be a power of 2, both re and im should be N long
 */

void lib_dsp_fft_bit_reverse( lib_dsp_fft_complex_t pts[], int N );

/** This function computes a forward FFT. The complex input array is
 * supplied as two arrays of integers, with numbers represented as
 * fixed-point values. The number of points must be a power of 2, and the
 * array of sine values should contain a quarter sine-wave. Use one of the
 * lib_dsp_sine_N tables. The function does not perform a bit-twiddle -
 * if required then lib_dsp_fft_bit_reverse() should be called beforehand.
 *
 * \param pts   Array of complex input points
 *
 * \param N     number of points. Must be a power of 2, both re and im should be N long
 *
 * \param sine  array of N/4+1 sine values, each represented as a sign bit,
 *              and a 31 bit fraction. 1 should be represented as 0x7fffffff.
 *              Arrays are provided in lib_dsp_tables.c; for example, for a 1024 point
 *              FFT use lib_dsp_sine_1024.
 */

void lib_dsp_fft_forward_complex
(
    lib_dsp_fft_complex_t pts[],
    int                   N,
    const int             sine[] );

/** This function computes the FFT of two real sequences in one go. It uses
 * a nifty trick (http://www.katjaas.nl/realFFT/realFFT.html) that enables
 * one to use a single complex FFT to compute two real FFTs simultaneously.
 * The real inputs should be in the first two real arrays, the output is in
 * the real and imaginary arrays (the output of a real FFT is still a
 * complex number).
 *
 * \param[in,out] re1    array of first set of real numbers on which to
 *                       compute FFT, on output this array stores the real
 *                       part of the complex FFT on this set of numbers.
 *
 * \param[in,out] re2    array of second set of real numbers on which to
 *                       compute FFT, on output this array stores the real
 *                       part of the complex FFT on this set of numbers.
 *
 * \param[out]    im1    imaginary parts of complex FFT of first array
 *
 * \param[out]    im2    imaginary parts of complex FFT of second array
 *
 * \param[in]     N      number of points
 *
 * \param[in]  sine  array of N/4+1 sine values, each represented as a sign bit,
 *              and a 31 bit fraction. 1 should be represented as 0x7fffffff.
 *              Arrays are provided in lib_dsp_tables.c; for example, for a 1024 point
 *              FFT use lib_dsp_sine_1024.
 */

void lib_dsp_fft_forward_tworeals
(
    lib_dsp_fft_complex_t two_re[],
    lib_dsp_fft_complex_t two_im[],
    int       N,
    const int sine[] );

void lib_dsp_fft_forward_tworeals_optimised
(
    lib_dsp_fft_complex_t two_re[],
    lib_dsp_fft_complex_t two_im[],
    int       N,
    const int sine[] );

/** This function computes an inverse FFT. The complex input array is
 * supplied as two arrays of integers, with numbers represented as
 * fixed-point values. The number of points must be a power of 2, and the
 * array of sine values should contain a quarter sine-wave. Use one of the
 * lib_dsp_sine_N tables. The function does not perform a bit-twiddle -
 * if required then lib_dsp_fft_bit_reverse() should be called beforehand.
 *
 * \param pts   Array of complex input points
 *
 * \param N     number of points. Must be a power of 2, both re and im should be N long
 *
 * \param sine  array of N/4+1 sine values, each represented as a sign bit,
 *              and a 31 bit fraction. 1 should be represented as 0x7fffffff.
 *              Arrays are provided in lib_dsp_tables.c; for example, for a 1024 point
 *              FFT use lib_dsp_sine_1024.
 */

void lib_dsp_fft_inverse_complex
(
    lib_dsp_fft_complex_t pts[],
    int                   N,
    const int             sine[] );

/** This function computes the inverse FFT on two sets of complex data that
 * are known to result in real numbers only in one go. It uses a nifty
 * trick (http://www.katjaas.nl/realFFT/realFFT.html) that enables one to
 * use a single complex inverse FFT to compute two real inverse FFTs
 * simultaneously. The outputs are in the two real arrays, the imaginary
 * arrays are unchanged.
 *
 * \param[in,out] re1    real part of first set of complex numbers on which
 *                       to compute inverse FFT
 *
 * \param[in,out] re2    real part of second set of complex numbers on which
 *                       to compute inverse FFT
 *
 * \param[in]     im1    imaginary part of first set of complex numbers on which
 *                       to compute inverse FFT
 *
 * \param[in]     im2    imaginary part of second set of complex numbers on which
 *                       to compute inverse FFT
 *
 * \param[in]     N      number of points
 *
 * \param[in] sine  array of N/4+1 sine values, each represented as a sign bit,
 *              and a 31 bit fraction. 1 should be represented as 0x7fffffff.
 *              Arrays are provided in lib_dsp_tables.c; for example, for a 1024 point
 *              FFT use lib_dsp_sine_1024.
 */

void lib_dsp_fft_inverse_tworeals
(
    lib_dsp_fft_complex_t two_re[],
    lib_dsp_fft_complex_t two_im[],
    int       N,
    const int sine[] );

#endif

