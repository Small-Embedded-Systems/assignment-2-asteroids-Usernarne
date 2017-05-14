/*
 * Assignment 2 - Small Embedded Systems.
 * Constantin Strobel, 15004712 and Leon Riseborough, 1
 *
 * This file is responsible for performing calculations for norm, lerp, map
 * randrange and radians.
 */
 
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"


/*
 * This method calculates the norm value which is the minimum value devided by the maximum value.
 */
float norm(float value, float min, float max) 
{
    return (value-min)/(max-min); 
}

/*
 * This method calculates the difference between minimum and maximum.
 */
float lerp(float min, float max, float value) 
{
    return max*value+(1.0f-value)*min; 
}

/*
 * This method calculates the size of the map needed to implement lerp(min and max), 
 * norm(min and max), value, lower and upper.
 */
float map(float value, float lower, float upper, float min, float max) 
{
    return lerp(min,max, norm(value,lower,upper)); 
}

/*
 * This method calculates the randrange. Only numbers between 0 to 359 can be used.
 */
int randrange(int from, int to)
{
    int range = to-from;
    return from + rand()%range;
}

const float pi = 3.1415926f; // This converts degrees into radians using the pi number.

/*
 * This method returns the radiant, which is pi multiplied by degrees divided by 180.
 */
float radians(float degrees) 
{ 
	return pi*(degrees/180); 
}

