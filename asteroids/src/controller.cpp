/*
 * Assignment 2 - Small Embedded Systems.
 * Constantin Strobel, 15004712 and Leon Riseborough, 1
 *
 * This file contains the controls method which is responsible for calling 
 * methods that allows the player to control the game.
 */

 
// C libraries 
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// Hardware platform libraries 
#include <mbed.h>
 
// Asteroids 
#include "model.h"
#include "asteroids.h"

// Joystick 5-way switch
typedef enum { 
 left,
 down,
 right,
 up,
 centre 
} joystick_t;

DigitalIn joystick[] = {
 P5_0,
 P5_1,
 P5_4,
 P5_2,
 P5_3}; // Left, down, right, up, center


static bool buttonPressedAndReleased(joystick_t button);

 /*
  * This method calls previously defined methods in order to move
  * the ship and to fire rockets.
  */
void controls(void)
{
	if(!paused) // If game is NOT paused, the player can move the ship and fire rockets.
	{
		if (buttonPressedAndReleased(up))
		{
			moveForward(); // Allows player to move forward.
		}
		else if (buttonPressedAndReleased(down))
		{
			moveBackwards(); // Allows player to move backwards.
		}
		else if (buttonPressedAndReleased(left))
		{
			turnRight(); // Allows player to turn right.
		}
		else if (buttonPressedAndReleased(right))
		{
			turnLeft(); // Allows player to turn left.
		}
		else if (buttonPressedAndReleased(centre))
		{
			fireRockets(); // Allows player to fire rockets.
		}
	}
}

/*
 * @brief buttonPressedAndReleased(button) tests to see if the button has
 *        been pressed then released.
 *        
 * @param button - the name of the button
 * @result - true if button pressed then released, otherwise false
 *
 * If the value of the button's pin is 0 then the button is being pressed,
 * just remember this in savedState.
 * If the value of the button's pin is 1 then the button is released, so
 * if the savedState of the button is 0, then the result is true, otherwise
 * the result is false.
 */
bool buttonPressedAndReleased(joystick_t button) 
{
  bool result = false;
  uint32_t state;
  static uint32_t savedState[5] = {1,1,1,1,1};
  
  state = joystick[button].read();
	
  if ((savedState[button] == 0) && (state == 1)) 
	{
    result = true;
  }
  savedState[button] = state;
  return result;
}
