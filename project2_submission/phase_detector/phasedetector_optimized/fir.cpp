/*
	Filename: fir.cpp
		Complex FIR or Match filter
		firI1 and firI2 share coef_t c[N]
		firQ1 and firQ2 share coef_t c[N]
		
	INPUT:
		I: signal for I sample
		I: signal for Q sample

	OUTPUT:
		X: filtered output
		Y: filtered output

*/

#include "phasedetector.h"

void firI1 (
  data_t *y,
  data_t x
  ) {

	coef_t c[N] = {1,    -1,    1,    -1,    -1,    -1,    1,    1,    -1,    -1,    -1,    1,    1,    -1,    1,    -1,    -1,    -1,    -1,    1,    1,    1,    1,    1,    -1,    -1,    1,    1,    1,    -1,    -1,    -1};

	// Write your code here
	static data_t shift_reg[N];
	data_t acc;
	int i;
	acc = 0;
//	#pragma HLS array_partition variable=shift_reg type=block factor=256
	#pragma HLS array_partition variable=c type=block factor=64

	TDL:
	for (i=N-1;i >0 ; i--){
//		#pragma HLS pipeline II=3
		#pragma HLS unroll factor=32

		shift_reg[i] = shift_reg[i-1];

	}

	shift_reg[0] = x;
	MAC:
	for (i=N-1;i >=0 ; i--){
//		#pragma HLS pipeline II=4
		#pragma HLS unroll factor=32

		acc += shift_reg[i] * c[i];
	}

	*y = acc;

}


void firI2 (
  data_t *y,
  data_t x
  ) {

	coef_t c[N] = {1,    -1,    1,    -1,    -1,    -1,    1,    1,    -1,    -1,    -1,    1,    1,    -1,    1,    -1,    -1,    -1,    -1,    1,    1,    1,    1,    1,    -1,    -1,    1,    1,    1,    -1,    -1,    -1};

	// Write your code here
	static data_t shift_reg[N];
	data_t acc;
	int i;
	acc = 0;
//#pragma HLS array_partition variable=shift_reg type=block factor=256
#pragma HLS array_partition variable=c type=block factor=64

	TDL:
	for (i=N-1;i >0 ; i--){
//		#pragma HLS pipeline II=3
		#pragma HLS unroll factor=32

		shift_reg[i] = shift_reg[i-1];

	}

	shift_reg[0] = x;
	MAC:
	for (i=N-1;i >=0 ; i--){
//		#pragma HLS pipeline II=4
		#pragma HLS unroll factor=32

		acc += shift_reg[i] * c[i];
	}

	*y = acc;
}




void firQ1 (
  data_t *y,
  data_t x
  ) {

	coef_t c[N] = {-1,    -1,    1,    -1,    1,    -1,    1,    -1,    -1,    -1,    -1,    1,    -1,    1,    -1,    1,    1,    -1,    1,    -1,    -1,    1,    -1,    1,    1,    1,    1,    -1,    1,    -1,    1,    1};


	// Write your code here
	static data_t shift_reg[N];
	data_t acc;
	int i;
	acc = 0;
//	#pragma HLS array_partition variable=shift_reg type=block factor=256
	#pragma HLS array_partition variable=c type=block factor=32

	TDL:
	for (i=N-1;i >0 ; i--){
		#pragma HLS pipeline II=3
		#pragma HLS unroll factor=32

		shift_reg[i] = shift_reg[i-1];

	}

	shift_reg[0] = x;
	MAC:
	for (i=N-1;i >=0 ; i--){
		#pragma HLS pipeline II=3
		#pragma HLS unroll factor=32

		acc += shift_reg[i] * c[i];
	}

	*y = acc;
}

void firQ2 (
  data_t *y,
  data_t x
  ) {

	coef_t c[N] = {-1,    -1,    1,    -1,    1,    -1,    1,    -1,    -1,    -1,    -1,    1,    -1,    1,    -1,    1,    1,    -1,    1,    -1,    -1,    1,    -1,    1,    1,    1,    1,    -1,    1,    -1,    1,    1};


	// Write your code here
	static data_t shift_reg[N];
	data_t acc;
	int i;
	acc = 0;
//	#pragma HLS array_partition variable=shift_reg type=block factor=256
	#pragma HLS array_partition variable=c type=block factor=64

	TDL:
	for (i=N-1;i >0 ; i--){
		#pragma HLS pipeline II=3
		#pragma HLS unroll factor=32

		shift_reg[i] = shift_reg[i-1];

	}

	shift_reg[0] = x;
	MAC:
	for (i=N-1;i >=0 ; i--){
		#pragma HLS pipeline II=3
		#pragma HLS unroll factor=32

		acc += shift_reg[i] * c[i];
	}

	*y = acc;
}


void fir (
  data_t I,
  data_t Q,

  data_t *X,
  data_t *Y
  ) {
	data_t Corr_II=0.0;
	data_t Corr_IQ= 0.0;
	data_t Corr_QQ = 0.0;
	data_t Corr_QI=0.0;

	// Write your code here
	firI1(&Corr_II, I);
	firQ1(&Corr_IQ, I);
	firI2(&Corr_QI, Q);
	firQ2(&Corr_QQ, Q);
	//Calculate X
	*X = Corr_II + Corr_QQ;
	//Calculate Y
	*Y = Corr_QI - Corr_IQ;


}


