#include<math.h>
#include "dft.h"
#include"coefficients256.h"

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE])
{
	//Write your code here
	int i, j;
	DTYPE w, c, s;

	DTYPE temp_real[SIZE];
	DTYPE temp_imag[SIZE];

	// Calculate each frequency domain sample iteratively
	for (i = 0; i < SIZE; i += 1) {
		temp_real[i] = 0;
		temp_imag[i] = 0;
		// (2 * pi * i)/SIZE
		w = (2.0 * 3.141592653589 / SIZE) * (DTYPE)i;

		// Calculate the jth frequency sample sequentially
		for (j = 0; j < SIZE; j += 1) {
			// Utilize HLS tool to calculate sine and cosine values
			c = cos(j*w);
			s = -1*sin(j*w);
//			c = cos_coefficients_table[(i*j)%SIZE];
//			s = sin_coefficients_table[(i*j)%SIZE];
			// Multiply the current phasor with the appropriate input sample and keep
			// running sum
			temp_real[i] += (real_sample[j] * c - imag_sample[j] * s);
			temp_imag[i] += (real_sample[j] * s + imag_sample[j] * c);
		}

	}

	// Perform an inplace DFT, i.e., copy result into the input arrays
	for (i = 0; i < SIZE; i += 1) {
		real_sample[i] = temp_real[i];
		imag_sample[i] = temp_imag[i];
	}
}
