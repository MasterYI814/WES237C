/*
	Filename: phasedetector.cpp
		Phase detector

	INPUT:
		I: signal for I sample
		Q: signal for Q sample
		length: array size

	OUTPUT:
		R: Radius
		Theta: Angle

*/

#include "phasedetector.h"

void phasedetector (
  data_t *I,
  data_t *Q,

  data_t *R,
  data_t *Theta,

  int length
  ){
	data_t X, Y;
	for (int i = 0; i < length;i++)
	  {
//		printf("input fir I = %f, input fir Q = %f\n", I[i], Q[i]);
		fir(I[i], Q[i], &X, &Y);
//		printf("output fir X = %f, output fir Y = %f\n", X, Y);
		cordiccart2pol(X,Y, &R[i], &Theta[i]);
//		printf("R = %f, Theta = %f\n", R[i], Theta[i]);
	  }


}




