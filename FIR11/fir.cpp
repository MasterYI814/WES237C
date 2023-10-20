/*
	Filename: fir.cpp
		FIR lab written for WES/CSE237C class at UCSD.

	INPUT:
		x: signal (chirp)

	OUTPUT:
		y: filtered output

*/

#include "fir.h"

void fir11 (
  data_t *y,
  data_t x
  )
{
	coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0,53};
	// Write your code here
	static data_t shift_reg[N];
	acc_t acc;
	int i;
	acc = 0;
	//Shift_Accum_Loop:

	for (i=N-1;i >=0 ; i--){
		if (i == 0) {
				acc += x * c[0];
				shift_reg[0] = x;
		} else{
			// what are the values in shift_reg every time I run the fir function? Before I shift up the x values? Is it the same?
			// do we need to initialize it?
			shift_reg[i] = shift_reg[i-1];
			acc += shift_reg[i] * c[i];
		}
	}
	*y = acc;
}


