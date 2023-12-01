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
	#pragma HLS INTERFACE s_axilite port=return bundle=fft
	#pragma HLS INTERFACE m_axi depth=1024 port=OUT_R offset=slave bundle=output
	#pragma HLS INTERFACE m_axi depth=1024 port=OUT_I offset=slave bundle=output1
	#pragma HLS INTERFACE m_axi depth=1024 port=X_R offset=slave bundle=input
	#pragma HLS INTERFACE m_axi depth=1024 port=X_I offset=slave bundle=input1
	#pragma HLS INTERFACE s_axilite port=OUT_R
	#pragma HLS INTERFACE s_axilite port=OUT_I
	#pragma HLS INTERFACE s_axilite port=X_R
	#pragma HLS INTERFACE s_axilite port=X_I

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
	int i, rev = 0;
	for (i = 0; i < 10; i++) {
//	#pragma HLS UNROLL factor=M
	rev = (rev << 1) | (input & 1);
	input = input >> 1;
	}

//	rev = (input & 1) << 9 +
//		  ((input>>1) & 1) << 8	+
//		  ((input>>2) & 1) << 7 +
//		  ((input>>3) & 1) << 6	+
//		  ((input>>4) & 1) << 5 +
//		  ((input>>5) & 1) << 4	+
//		  ((input>>6) & 1) << 3 +
//		  ((input>>7) & 1) << 2	+
//		  ((input>>8) & 1) << 1 +
//		  ((input>>9) & 1) << 0;
	return rev;
}

void bit_reverse(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE Y_R[SIZE], DTYPE Y_I[SIZE]){
//#pragma HLS DATAFLOW
	unsigned int reversed;
	unsigned int i;
//	DTYPE temp;
	traverse_all_bits:
	for (i = 0; i < 1024; i++) {
		#pragma HLS pipeline II=1
		Find_bit_reversed_index:
		reversed = reverse_bits(i); // Find the bit reversed index
//		if (i <= reversed) {
		swap:
		// Swap the real values
//			Y_R[i] = X_R[reversed];
		Y_R[reversed] = X_R[i];
		// Swap the imaginary values
//			Y_I[i] = X_I[reversed];
		Y_I[reversed] = X_I[i];
//			}
	}
}
/*=======================BEGIN: FFT=========================*/
//stage 1
void fft_stage_first(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {

	//Insert your code here
	DTYPE temp_R;		/*temporary storage complex variable*/
	DTYPE temp_I;		/*temporary storage complex variable*/


	int i,j;			/* loop indexes */
	int i_lower;		/* Index of lower point in butterfly */
	int idx = 0; 			/* index for the LUT*/

	int DFTpts;
	int numBF;			/*Butterfly Width*/

	DTYPE  c, s;
	DFTpts = 2;		// DFT = 2^stage = points in sub DFT, 1st stage is bit reverse
	numBF = 1; 			// Butterfly WIDTHS in sub-DFT

	//		a = 0.0;
	// Perform butterflies for j-th stage
	butterfly:


	c = W_real[idx];
	s = W_imag[idx];
	idx = idx + (SIZE/DFTpts);


		// Compute butterflies that use same W**k
		DFTpts:
		for(i=0; i<SIZE; i += DFTpts)
		{
			#pragma HLS pipeline II=2
//			#pragma HLS UNROLL factor=1024
//			#pragma HLS loop_tripcount min=0 max=1024
			i_lower = i + numBF;			//index of lower point in butterfly
			temp_R = X_R[i_lower]*c- X_I[i_lower]*s;
			temp_I = X_I[i_lower]*c+ X_R[i_lower]*s;

			OUT_R[i_lower] = X_R[i] - temp_R;
			OUT_I[i_lower] = X_I[i] - temp_I;
			OUT_R[i] = X_R[i] + temp_R;
			OUT_I[i] = X_I[i] + temp_I;
		}
//		k+=step;
//	}
//	step=step/2;

}

//stages
void fft_stages(DTYPE X_R[SIZE], DTYPE X_I[SIZE], int stage, DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {
//#pragma HLS DATAFLOW
	//Insert your code here
	DTYPE temp_R;		/*temporary storage complex variable*/
	DTYPE temp_I;		/*temporary storage complex variable*/


	int i_lower;		/* Index of lower point in butterfly */
	int idx = 0; 			/* index for the LUT*/

	DTYPE c, s;
	int DFTpts = 1 << stage;		// DFT = 2^stage = points in sub DFT, 1st stage is bit reverse
	int numBF = DFTpts/2; 			// Butterfly WIDTHS in sub-DFT

		c = W_real[idx];
		s = W_imag[idx];
		idx = idx + (SIZE/DFTpts);

		// Compute butterflies that use same W**k
		DFTpts:
		for(int i=0; i<1024; i += DFTpts)
		{
//			#pragma HLS UNROLL factor=2
//			#pragma HLS loop_tripcount min=0 max=256
			idx = 0;

			for(int j=0; j<numBF; j++)
				{
				#pragma HLS pipeline II=1
				c = W_real[idx];
				s = W_imag[idx];
				idx = idx + (SIZE/DFTpts);
				i_lower = i+j + numBF;			//index of lower point in butterfly
				temp_R = X_R[i_lower]*c- X_I[i_lower]*s;
				temp_I = X_I[i_lower]*c+ X_R[i_lower]*s;

				OUT_R[i_lower] = X_R[i+j] - temp_R;
				OUT_I[i_lower] = X_I[i+j] - temp_I;
				OUT_R[i+j] = X_R[i+j] + temp_R;
				OUT_I[i+j] = X_I[i+j] + temp_I;
				}

		}

}




//last stage
void fft_stage_last(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {
//	#pragma HLS DATAFLOW
	//Insert your code here
	DTYPE temp_R;		/*temporary storage complex variable*/
	DTYPE temp_I;		/*temporary storage complex variable*/


	int i,j;			/* loop indexes */
	int i_lower;		/* Index of lower point in butterfly */
	int idx = 0; 			/* index for the LUT*/

	int DFTpts;
	int numBF;			/*Butterfly Width*/

	DTYPE c, s;
	DFTpts = 1024;		// DFT = 2^stage = points in sub DFT, 1st stage is bit reverse
	numBF = 512; 			// Butterfly WIDTHS in sub-DFT

	butterfly:
	for(j=0; j<512; j++)
	{

		c = W_real[idx];
		s = W_imag[idx];
		idx = idx + (SIZE/DFTpts);
		#pragma HLS pipeline II=2

		// Compute butterflies that use same W**k
		DFTpts:
		for(i=j; i<1024; i += DFTpts)
		{

//			#pragma HLS UNROLL factor=1024
//			#pragma HLS loop_tripcount min=0 max=1024
			i_lower = i + numBF;			//index of lower point in butterfly
			temp_R = X_R[i_lower]*c- X_I[i_lower]*s;
			temp_I = X_I[i_lower]*c+ X_R[i_lower]*s;

			OUT_R[i_lower] = X_R[i] - temp_R;
			OUT_I[i_lower] = X_I[i] - temp_I;
			OUT_R[i] = X_R[i] + temp_R;
			OUT_I[i] = X_I[i] + temp_I;
		}

	}


}
/*=======================END: FFT=========================*/




