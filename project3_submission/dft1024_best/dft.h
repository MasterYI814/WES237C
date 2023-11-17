#include <ap_int.h>
#include "ap_axi_sdata.h"
#include "hls_stream.h"


union fp_int {
	int i;
	float fp;
};

typedef ap_axis<32,2,5,6> transPkt;
typedef float DTYPE;
#define SIZE 1024 		/* SIZE OF DFT */
//void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE],DTYPE real_op[SIZE], DTYPE imag_op[SIZE]);
void dft(hls::stream<transPkt> &real_sample, hls::stream<transPkt> &imag_sample,
		hls::stream<transPkt> &real_output, hls::stream<transPkt> &imag_output);
//void dft(DTYPE *, DTYPE *,DTYPE *, DTYPE *); use this for doing the demo// 
