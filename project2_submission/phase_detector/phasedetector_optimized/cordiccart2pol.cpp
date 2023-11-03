#include "phasedetector.h"



data_t Kvalues[NO_ITER] = {1,	0.500000000000000,	0.250000000000000,	0.125000000000000,	0.0625000000000000,	0.0312500000000000,	0.0156250000000000,	0.00781250000000000,	0.00390625000000000,	0.00195312500000000,	0.000976562500000000,	0.000488281250000000,	0.000244140625000000,	0.000122070312500000,	6.10351562500000e-05,	3.05175781250000e-05};

data_t angles[NO_ITER] = {0.785398163397448,	0.463647609000806,	0.244978663126864,	0.124354994546761,	0.0624188099959574,	0.0312398334302683,	0.0156237286204768,	0.00781234106010111,	0.00390623013196697,	0.00195312251647882,	0.000976562189559320,	0.000488281211194898,	0.000244140620149362,	0.000122070311893670,	6.10351561742088e-05,	3.05175781155261e-05};


void cordiccart2pol(data_t x, data_t y, data_t * r,  data_t * theta)
{
	//	*r = sqrt(x * x + y * y);
	data_t current_cos = 0.0;
	data_t current_sin = 0.0;
	data_t current_theta = 0.0;
	data_t cos_shift = 0.0;
	data_t sin_shift = 0.0;
	data_t new_cos = 0.0;
	data_t new_sin = 0.0;
	if (x < 0 && y<0) // 3rd quadrant
	{
		current_cos = -y;
		current_sin = x;
		current_theta = -1.57079632679; //-pi/2
//		printf("3rd quadrant");
	}
	else if(x<0) // 2nd quadrant
	{
		current_cos = y;
		current_sin = -x;
		current_theta = 1.57079632679; // pi/2
//		printf("2nd quadrant");
	}
	else // 1 or 4 quadrant
	{
		current_cos = x;
		current_sin = y;
//		printf("1 or 4 quadrant");
	}

//    *theta = atan2(y, x);


		for (int j = 0; j < 13; j++) {

			if (current_sin > 0)
			{

				cos_shift = -1*current_cos * Kvalues[j];
				sin_shift = -1*current_sin * Kvalues[j];
				current_theta = current_theta + angles[j];
			}
			else
			{
				cos_shift = current_cos * Kvalues[j];
				sin_shift = current_sin * Kvalues[j];
				current_theta = current_theta - angles[j];
			}

			data_t new_cos = current_cos - sin_shift;
			data_t new_sin = current_sin + cos_shift;

			current_cos = new_cos;
			current_sin = new_sin;
		}

		data_t CG = 1.64669;
		*r = current_cos/CG;
		*theta = current_theta;
}
