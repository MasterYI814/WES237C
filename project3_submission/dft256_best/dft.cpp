#include<math.h>
#include "dft.h"
#include"coefficients256.h"
#include "stdio.h"


void get_cos_sin(int i, int j,  DTYPE *c, DTYPE *s)
{
	int k;
	k = (i*j)%SIZE;
	*c = cos_coefficients_table[k];
	*s = sin_coefficients_table[k];

//	printf("c is %f \n", *c);
}

void real_imag_calc(int i, int j, DTYPE *real_sample, DTYPE *imag_sample,  DTYPE *tempc, DTYPE *temps, DTYPE *real_output, DTYPE *imag_output)
{
	DTYPE temp_real, temp_imag;
	temp_real = real_sample[j] * *tempc;
	temp_imag = imag_sample[j] * *temps;
	real_output[i] += (temp_real - temp_imag);
	temp_real = real_sample[j] * *temps;
	temp_imag = imag_sample[j] * *tempc;
	imag_output[i] += (temp_real + temp_imag);

}

void real_calc(int i, int j, DTYPE *real_sample, DTYPE *imag_sample,  DTYPE c, DTYPE s, DTYPE *real_output)
{
	DTYPE temp_real, temp_imag;
	temp_real = real_sample[j] * c;
	temp_imag = imag_sample[j] * s;
	real_output[i] += (temp_real - temp_imag);
	printf("c is %f \n", c);
//	printf("temp_real is %f \n", temp_real);
}

void imag_calc(int i, int j, DTYPE *real_sample, DTYPE *imag_sample,  DTYPE c, DTYPE s, DTYPE *imag_output)
{
	DTYPE temp_real, temp_imag;
	temp_real = real_sample[j] * s;
	temp_imag = imag_sample[j] * c;
	imag_output[i] += (temp_real + temp_imag);
}

void swap (DTYPE *c, DTYPE *s, DTYPE *tempc, DTYPE *temps)
{
	*tempc = *c;
	*temps = *s;
}

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE real_output[SIZE], DTYPE imag_output[SIZE])
{
#pragma HLS DATAFLOW
	int i=0;
	int j=0;
	int k=0;
	DTYPE c, s, tempc, temps;


//	#pragma HLS array_partition variable=real_output type=cyclic factor=4
//	#pragma HLS array_partition variable=imag_output type=cyclic factor=4
//	#pragma HLS array_partition variable=real_sample type=cyclic factor=4
//	#pragma HLS array_partition variable=imag_sample type=cyclic factor=4
//	#pragma HLS array_partition variable=cos_coefficients_table type=cyclic factor=4
//	#pragma HLS array_partition variable=sin_coefficients_table type=cyclic factor=4

	// Calculate each frequency domain sample iteratively
//	double_pass(real_sample, temp_rs, imag_sample, temp_is);
	for (i = 0; i < SIZE; i += 1) {


		for (j = 0; j < SIZE; j += 1) {
//			#pragma HLS unroll factor=4

			#pragma HLS pipeline II=SIZE

			get_cos_sin(i,j,&c,&s);
//			printf("c is %f \n", c);
			swap(&c, &s, &tempc, &temps );
//			printf("c is %f \n", c);
			real_imag_calc(i, j, real_sample, imag_sample, &tempc, &temps, real_output, imag_output);


		}
	}

}


/*
void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE real_output[SIZE], DTYPE imag_output[SIZE])
{

	int i=0;
	int j=0;
	int k=0;
	DTYPE c, s;
	DTYPE temp_real, temp_imag;

//	#pragma HLS array_partition variable=real_output type=cyclic factor=4
//	#pragma HLS array_partition variable=imag_output type=cyclic factor=4
//	#pragma HLS array_partition variable=real_sample type=cyclic factor=4
//	#pragma HLS array_partition variable=imag_sample type=cyclic factor=4
//	#pragma HLS array_partition variable=cos_coefficients_table type=cyclic factor=4
//	#pragma HLS array_partition variable=sin_coefficients_table type=cyclic factor=4

	// Calculate each frequency domain sample iteratively
	for (i = 0; i < SIZE; i += 1) {


		for (j = 0; j < SIZE; j += 1) {
			#pragma HLS pipeline II=SIZE
//			#pragma HLS unroll factor=64
			k = (i*j)%SIZE;
			c = cos_coefficients_table[k];
			s = sin_coefficients_table[k];
			temp_real = real_sample[j] * c;
			temp_imag = imag_sample[j] * s;
			real_output[i] += (temp_real - temp_imag);
			temp_real = real_sample[j] * s;
			temp_imag = imag_sample[j] * c;
			imag_output[i] += (temp_real + temp_imag);

		}
	}

}
*/
