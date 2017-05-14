/*
 * Assignment 2 - Small Embedded Systems.
 * Constantin Strobel, 15004712 and Leon Riseborough, 1
 *
 * This files assigns norm, lerp and map to float.
 * Sets the colour range to 16 bit.
 */

// some numerical helper functions 
float norm(float value, float min, float max);
float lerp(float min, float max, float value);
float map(float value, float lower, float upper, float min, float max);


typedef uint16_t colour_t; // This allows for 16 bit colours to be used.

/* macro to make colour value from r g b values in the range 0...255 
 *  see https://en.wikipedia.org/wiki/List_of_colors_(compact)
 *  https://en.wikipedia.org/wiki/List_of_Crayola_colored_pencil_colors
 *  https://en.wikipedia.org/wiki/List_of_Crayola_crayon_colors
 *  For lists of R G B values you might want to use
 *	I'm using a macro here as the compiler might well convert it into a constant at compile time
*/
#define rgb(r,g,b) ((uint16_t)(31 * r /255)<<11 | (uint16_t)(63 * g /255)<<5 | (uint16_t)(31 * b /255))

int randrange(int from, int to);

float dist(coordinate_t p1, coordinate_t p2);

extern const float pi;
float radians(float degrees);
