#include<math.h>
#include "dft.h"
#include"coefficients1024.h"
#include "stdio.h"

/*
void get_cos_sin(int i, int j,  DTYPE *c, DTYPE *s)
{
	int k;
	k = (i*j)%SIZE;
	*c = cos_coefficients_table[k];
	*s = sin_coefficients_table[k];

//	printf("c is %f \n", *c);
}

void real_imag_calc(int i, int j, hls::stream<transPkt> *real_sample, hls::stream<transPkt> *imag_sample,
		DTYPE *tempc, DTYPE *temps, DTYPE *real_output, DTYPE *imag_output)
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
*/



void dft(hls::stream<transPkt> &real_sample, hls::stream<transPkt> &imag_sample,
		hls::stream<transPkt> &real_output, hls::stream<transPkt> &imag_output)
{
	#pragma HLS INTERFACE mode=s_axilite port=return
	#pragma HLS INTERFACE mode=axis port=real_sample, imag_sample, real_output, imag_output

	int i=0;
	int j=0;
	int k=0;
	fp_int c, s;
	fp_int temp_real, temp_imag, temp_real_out, temp_imag_out;
	transPkt real, imag;
	fp_int real_data[SIZE], imag_data[SIZE], real_out[SIZE], imag_out[SIZE];
	transPkt real_output_pkt, imag_output_pkt;


	for (int i=0; i<SIZE; i++)
	{
		real = real_sample.read();
		imag = imag_sample.read();

		real_data[i].i = real.data;
		imag_data[i].i = imag.data;

		real_out[i].fp = 0;
		imag_out[i].fp = 0;
	}


	#pragma HLS array_partition variable=real_out type=cyclic factor=4
	#pragma HLS array_partition variable=imag_out type=cyclic factor=4
	#pragma HLS array_partition variable=real_data type=cyclic factor=4
	#pragma HLS array_partition variable=imag_data type=cyclic factor=4
	#pragma HLS array_partition variable=cos_coefficients_table type=cyclic factor=4
	#pragma HLS array_partition variable=sin_coefficients_table type=cyclic factor=4

	// Calculate each frequency domain sample iteratively
	for (i = 0; i < SIZE; i += 1) {

		for (j = 0; j < SIZE; j += 1) {
			#pragma HLS pipeline II=SIZE
//			#pragma HLS unroll factor=64
			k = (i*j)%SIZE;
			c.fp = cos_coefficients_table[k];
			s.fp = sin_coefficients_table[k];
			// Use integer type to read from the AXIS packets
//			real_data[j].i = real[j].data;
//			imag_data[j].i = imag[j].data;

			temp_real.fp = real_data[j].fp * c.fp;
			temp_imag.fp = imag_data[j].fp * s.fp;

			real_out[i].fp += (temp_real.fp - temp_imag.fp);

			temp_real.fp = real_data[j].fp * s.fp;
			temp_imag.fp = imag_data[j].fp * c.fp;
			imag_out[i].fp += (temp_real.fp + temp_imag.fp);
		}


	}

	for (i = 0; i < SIZE; i += 1) {

		real.data = real_out[i].i;
		real.last = (i==SIZE-1)?1:0;
		real_output.write(real);


		imag.data = imag_out[i].i;
		imag.last = (i==SIZE-1)?1:0;
		imag_output.write(imag);

	}


//	real_output.keep = real_sample.keep;
//	real_output.strb = real_sample.strb;
//	imag_output.keep = imag_sample.keep;
//	imag_output.strb = imag_sample.strb;
}

