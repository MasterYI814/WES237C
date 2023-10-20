/*
	Filename: fir.cpp
		FIR lab wirtten for WES/CSE237C class at UCSD.
		Match filter
	INPUT:
		x: signal (chirp)

	OUTPUT:
		y: filtered output

*/

#include "fir.h"


void fir128 (
		data_t *y,
		data_t x
  )
{

	coef_t c[N] = {10, 11, 11, 8, 3, -3, -8, -11, -11, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -11, -11, -8, -3, 3, 8, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 8, 3, -3, -8, -11, -11, -10, -10, -10, -10, -10, -10, -10, -10, -11, -11, -8, -3, 3, 8, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 8, 3, -3, -8, -11, -11, -10, -10, -10, -10, -10, -10, -10, -10, -11, -11, -8, -3, 3, 8, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
	
	// Write your code here
	static ap_int<8> shift_reg[N];
	ap_int<16> acc;
	int i;
	acc = 0;
	#pragma HLS array_partition variable=shift_reg type=block factor=300
	#pragma HLS array_partition variable=c type=block factor=64

	TDL:
	for (i=N-1;i >0 ; i--){
		#pragma HLS pipeline II=3
		#pragma HLS unroll factor=127

		shift_reg[i] = shift_reg[i-1];
//		acc += shift_reg[i] * c[i];

	}
	//Knowing our input size is 600, divisible by unroll factor
	shift_reg[0] = x;
//	acc += x * c[0];
	MAC:
	for (i=N-1;i >=0 ; i--){
		#pragma HLS pipeline II=4
		#pragma HLS unroll factor=128

		acc += shift_reg[i] * c[i];
	}

	*y = acc;

//	Shift_Accum_Loop:
//	for (i=N-1;i >=0 ; i--){
//	#pragma HLS pipeline II=1
//		if (i == 0) {
//				acc += x * c[0];
//				shift_reg[0] = x;
//		} else{
//			// what are the values in shift_reg every time I run the fir function? Before I shift up the x values? Is it the same?
//			// do we need to initialize it?
//			shift_reg[i] = shift_reg[i-1];
//			acc += shift_reg[i] * c[i];
//		}
//	}
//	*y = acc;
}


