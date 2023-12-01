#include "fft.h"
#include <stdio.h>

void qpsk_decode(DTYPE R[SIZE], DTYPE I[SIZE], int D[SIZE]) {

	//Write your code here
	for (int i = 0; i<SIZE; i++)
	{
		#pragma HLS pipeline II=2
		if ((R[i]>0) && (I[i]>0))
		{
			D[i] = 0;
		}
		else if ((R[i]<0) && (I[i]>0))
		{
			D[i] = 1;
		}
		else if((R[i]<0) && (I[i]<0))
		{
			D[i] = 3;
		}
		else if((R[i]>0) && (I[i]<0))
		{
			D[i] = 2;
		}
	}
}

