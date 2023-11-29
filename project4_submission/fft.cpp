/*
This is traditional 2-radix DIT FFT algorithm implementation.
INPUT:
	In_R, In_I[]: Real and Imag parts of Complex signal

OUTPUT:
	Out_R, Out_I[]: Real and Imag parts of Complex signal
*/

#include "fft.h"

//void bit_reverse(DTYPE X_R[SIZE], DTYPE X_I[SIZE]);
void bit_reverse(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE Y_R[SIZE], DTYPE Y_I[SIZE]);
void fft_stage_first(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]);
void fft_stages(DTYPE X_R[SIZE], DTYPE X_I[SIZE], int STAGES, DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]);
void fft_stage_last(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]);


void fft(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE])
{
	#pragma HLS DATAFLOW
	DTYPE Y_R[SIZE], Y_I[SIZE];
	bit_reverse(X_R, X_I, Y_R, Y_I);

	//Call fft
	DTYPE Stage1_R[SIZE], Stage1_I[SIZE];
	DTYPE Stage2_R[SIZE], Stage2_I[SIZE];
	DTYPE Stage3_R[SIZE], Stage3_I[SIZE];
	DTYPE Stage4_R[SIZE], Stage4_I[SIZE];
	DTYPE Stage5_R[SIZE], Stage5_I[SIZE];
	DTYPE Stage6_R[SIZE], Stage6_I[SIZE];
	DTYPE Stage7_R[SIZE], Stage7_I[SIZE];
	DTYPE Stage8_R[SIZE], Stage8_I[SIZE];
	DTYPE Stage9_R[SIZE], Stage9_I[SIZE];

	fft_stage_first(Y_R, Y_I, Stage1_R, Stage1_I);
	fft_stages(Stage1_R, Stage1_I, 2, Stage2_R, Stage2_I);
	fft_stages(Stage2_R, Stage2_I, 3, Stage3_R, Stage3_I);
	fft_stages(Stage3_R, Stage3_I, 4, Stage4_R, Stage4_I);
	fft_stages(Stage4_R, Stage4_I, 5, Stage5_R, Stage5_I);
	fft_stages(Stage5_R, Stage5_I, 6, Stage6_R, Stage6_I);
	fft_stages(Stage6_R, Stage6_I, 7, Stage7_R, Stage7_I);
	fft_stages(Stage7_R, Stage7_I, 8, Stage8_R, Stage8_I);
	fft_stages(Stage8_R, Stage8_I, 9, Stage9_R, Stage9_I);
	fft_stage_last(Stage9_R, Stage9_I, OUT_R, OUT_I);

}

unsigned int reverse_bits(unsigned int input) {
//#pragma HLS DATAFLOW
	int i, rev = 0;
	for (i = 0; i < M; i++) {
	#pragma HLS pipeline II=2
	rev = (rev << 1) | (input & 1);
	input = input >> 1;
	}
	return rev;
}
/*
void bit_reverse(DTYPE X_R[SIZE], DTYPE X_I[SIZE]){
	unsigned int reversed;
	unsigned int i;
	DTYPE temp;
	for (i = 0; i < SIZE; i++) {
		reversed = reverse_bits(i); // Find the bit reversed index
		if (i <= reversed) {
			// Swap the real values
			temp = X_R[i];
			X_R[i] = X_R[reversed];
			X_R[reversed] = temp;
			// Swap the imaginary values
			temp = X_I[i];
			X_I[i] = X_I[reversed];
			X_I[reversed] = temp;
			}
	}

}
*/
void bit_reverse(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE Y_R[SIZE], DTYPE Y_I[SIZE]){
//#pragma HLS DATAFLOW
	unsigned int reversed;
	unsigned int i;
	DTYPE temp;
	traverse_all_bits:
	for (i = 0; i < SIZE; i++) {
		#pragma HLS pipeline II=2
		Find_bit_reversed_index:
		reversed = reverse_bits(i); // Find the bit reversed index
		if (i <= reversed) {
			swap:
			// Swap the real values
			Y_R[i] = X_R[reversed];
			Y_R[reversed] = X_R[i];
			// Swap the imaginary values
			Y_I[i] = X_I[reversed];
			Y_I[reversed] = X_I[i];
			}
	}
}
/*=======================BEGIN: FFT=========================*/
//stage 1
void fft_stage_first(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {
//#pragma HLS DATAFLOW
	//Insert your code here
	DTYPE temp_R;		/*temporary storage complex variable*/
	DTYPE temp_I;		/*temporary storage complex variable*/


	int i,j;			/* loop indexes */
	int i_lower;		/* Index of lower point in butterfly */
	int step;
	int idx = 0; 			/* index for the LUT*/

	int DFTpts;
	int numBF;			/*Butterfly Width*/

	int N2 = SIZE2;	/* N2=N>>1 */
	step=N2;
	DTYPE a, e, c, s;
	DFTpts = 1 << 1;		// DFT = 2^stage = points in sub DFT, 1st stage is bit reverse
	numBF = DFTpts/2; 			// Butterfly WIDTHS in sub-DFT
	e = -6.283185307178/DFTpts;

	//		a = 0.0;
	// Perform butterflies for j-th stage
	butterfly:
	for(j=0; j<1; j++)
	{
		#pragma HLS pipeline II=1
		c = W_real[idx];
		s = W_imag[idx];
		idx = idx + (SIZE/DFTpts);
	//			c = cos(a);
	//			s = sin(a);
	//			a = a + e;

		// Compute butterflies that use same W**k
		DFTpts:
		for(i=j; i<SIZE; i += DFTpts)
		{
			#pragma HLS pipeline II=2
			i_lower = i + numBF;			//index of lower point in butterfly
			temp_R = X_R[i_lower]*c- X_I[i_lower]*s;
			temp_I = X_I[i_lower]*c+ X_R[i_lower]*s;

			OUT_R[i_lower] = X_R[i] - temp_R;
			OUT_I[i_lower] = X_I[i] - temp_I;
			OUT_R[i] = X_R[i] + temp_R;
			OUT_I[i] = X_I[i] + temp_I;
		}
//		k+=step;
	}
//	step=step/2;

}

//stages
void fft_stages(DTYPE X_R[SIZE], DTYPE X_I[SIZE], int stage, DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {
//#pragma HLS DATAFLOW
	//Insert your code here
	DTYPE temp_R;		/*temporary storage complex variable*/
	DTYPE temp_I;		/*temporary storage complex variable*/


//	int i = 0;
//	int j = 0;
//	int k = 0;			/* loop indexes */
	int i_lower;		/* Index of lower point in butterfly */
	int step;
	int idx = 0; 			/* index for the LUT*/

//	int DFTpts;
//	int numBF;			/*Butterfly Width*/

	int N2 = SIZE2;	/* N2=N>>1 */
	step=N2;
	DTYPE a, e, c, s;
	int DFTpts = 1 << stage;		// DFT = 2^stage = points in sub DFT, 1st stage is bit reverse
	int numBF = DFTpts/2; 			// Butterfly WIDTHS in sub-DFT
//	k=0;

	e = -6.283185307178/DFTpts;

	//		a = 0.0;
	// Perform butterflies for j-th stage
	butterfly:
	for(int j=0; j<numBF; j++)
	{
		#pragma HLS pipeline II=1
		#pragma HLS loop_tripcount min= (int)(1<<2)/2 max=(int)(1 << 9)/2

		c = W_real[idx];
		s = W_imag[idx];
		idx = idx + (SIZE/DFTpts);
	//			c = cos(a);
	//			s = sin(a);
	//			a = a + e;

		// Compute butterflies that use same W**k
		DFTpts:
		for(int i=j; i<1024; i += DFTpts)
		{
			#pragma HLS loop_tripcount min= (int)(0) max=(int)(1024)
			#pragma HLS pipeline II=2
			i_lower = i + numBF;			//index of lower point in butterfly
			temp_R = X_R[i_lower]*c- X_I[i_lower]*s;
			temp_I = X_I[i_lower]*c+ X_R[i_lower]*s;

			OUT_R[i_lower] = X_R[i] - temp_R;
			OUT_I[i_lower] = X_I[i] - temp_I;
			OUT_R[i] = X_R[i] + temp_R;
			OUT_I[i] = X_I[i] + temp_I;
		}
//		k+=step;
	}
//	step=step/2;

}


//last stage
void fft_stage_last(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {
//	#pragma HLS DATAFLOW
	//Insert your code here
	DTYPE temp_R;		/*temporary storage complex variable*/
	DTYPE temp_I;		/*temporary storage complex variable*/


	int i,j;			/* loop indexes */
	int i_lower;		/* Index of lower point in butterfly */
	int step;
	int idx = 0; 			/* index for the LUT*/

	int DFTpts;
	int numBF;			/*Butterfly Width*/

	int N2 = SIZE2;	/* N2=N>>1 */
	step=N2;
	DTYPE a, e, c, s;
	DFTpts = 1 << 10;		// DFT = 2^stage = points in sub DFT, 1st stage is bit reverse
	numBF = DFTpts/2; 			// Butterfly WIDTHS in sub-DFT

	e = -6.283185307178/DFTpts;

	//		a = 0.0;
	// Perform butterflies for j-th stage
	butterfly:
	for(j=0; j<512; j++)
	{
		#pragma HLS pipeline II=1
		c = W_real[idx];
		s = W_imag[idx];
		idx = idx + (SIZE/DFTpts);
	//			c = cos(a);
	//			s = sin(a);
	//			a = a + e;

		// Compute butterflies that use same W**k
		DFTpts:
		for(i=j; i<SIZE; i += DFTpts)
		{
			#pragma HLS pipeline II=2
			i_lower = i + numBF;			//index of lower point in butterfly
			temp_R = X_R[i_lower]*c- X_I[i_lower]*s;
			temp_I = X_I[i_lower]*c+ X_R[i_lower]*s;

			OUT_R[i_lower] = X_R[i] - temp_R;
			OUT_I[i_lower] = X_I[i] - temp_I;
			OUT_R[i] = X_R[i] + temp_R;
			OUT_I[i] = X_I[i] + temp_I;
		}
//		k+=step;
	}
//	step=step/2;

}
/*=======================END: FFT=========================*/




