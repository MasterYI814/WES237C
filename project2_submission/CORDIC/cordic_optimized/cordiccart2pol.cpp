#include "cordiccart2pol.h"
#include "math.h"
#include "stdio.h"
#include "ap_int.h"
#include "ap_fixed.h"

typedef ap_fixed<16, 3> mytype;

mytype Kvalues[NO_ITER] = {1,	0.500000000000000,	0.250000000000000,	0.125000000000000,	0.0625000000000000,	0.0312500000000000,	0.0156250000000000,	0.00781250000000000,	0.00390625000000000,	0.00195312500000000,	0.000976562500000000,	0.000488281250000000,	0.000244140625000000,	0.000122070312500000,	6.10351562500000e-05,	3.05175781250000e-05};
// 1 = 2^(-0)   0.5 = 2^(-1)
mytype angles[NO_ITER] = {0.785398163397448,	0.463647609000806,	0.244978663126864,	0.124354994546761,	0.0624188099959574,	0.0312398334302683,	0.0156237286204768,	0.00781234106010111,	0.00390623013196697,	0.00195312251647882,	0.000976562189559320,	0.000488281211194898,	0.000244140620149362,	0.000122070311893670,	6.10351561742088e-05,	3.05175781155261e-05};
// 0.785398163397448 = atan(Kvalues[0]) (Rotating angle column in CORDIC table)


void cordiccart2pol(data_t x, data_t y, data_t *r, data_t *theta)
{

    // Initialize variables
//	*r = sqrt(x * x + y * y);
	mytype current_cos = 0.0;
	mytype current_sin = 0.0;
	mytype current_theta = 0.0;
	mytype cos_shift = 0.0;
	mytype sin_shift = 0.0;
	mytype new_cos = 0.0;
	mytype new_sin = 0.0;
	if (x < 0 && y<0) // 3rd quadrant
	{
		current_cos = -y;
		current_sin = x;
		current_theta = -1.57079632679; //-pi/2
	}
	else if(x<0) // 2nd quadrant
	{
		current_cos = y;
		current_sin = -x;
		current_theta = 1.57079632679; // pi/2
	}
	else // 1 or 4 quadrant
	{
	    current_cos = x;
	    current_sin = y;
	}

//    *theta = atan2(y, x);


        for (int j = 0; j < 11; j++) {
        	/* This is tricky as our current_theta starts at 0/-90/+90. The coordinate rotates toward zero,
        	 * but our theta angle starts at 0. So we need to reverse sigma.
        	 */
//        	ap_int<2> sigma = (current_sin > 0) ? -1 : 1;
//            printf("number of iter is %d \n", j);
//            printf("\tcurrent_cos is %f \n", current_cos);
//            printf("\tcurrent_sin is %f \n", current_sin);
//            printf("\tcurrent_theta is %f \n", current_theta);
        	if (current_sin > 0)
        	{

        		cos_shift = ~current_cos >> j;
        		sin_shift = ~current_sin >> j;
        		current_theta = current_theta + angles[j];
        	}
        	else
        	{
        		cos_shift = current_cos >> j;
				sin_shift = current_sin >> j;
				current_theta = current_theta - angles[j];
        	}
//        	mytype cos_shift = current_cos * sigma >> j;
//        	mytype sin_shift = current_sin * sigma >> j;
////        	mytype cos_shift = current_cos * sigma * Kvalues[j];
////        	mytype sin_shift = current_sin * sigma * Kvalues[j];
//
////            printf("\tcos_shift is %f \n", cos_shift);
////            printf("\tsin_shift is %f \n", sin_shift);
//
//            // Perform the rotation
        	mytype new_cos = current_cos - sin_shift;
        	mytype new_sin = current_sin + cos_shift;
//
//            // Determine the new theta
//            current_theta = current_theta - sigma * angles[j];
//
            // Update current_cos and current_sin for the next iteration
            current_cos = new_cos;
            current_sin = new_sin;
        }

        // Set the final values of r and theta
        // I choose CORDIC Gain to be 1.64669
        mytype CG = 1.64669;
        *r = current_cos/CG;
        *theta = current_theta;
}
