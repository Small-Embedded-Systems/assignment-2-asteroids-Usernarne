/* 
 * Assignment 2 - Small Embedded Systems.
 * Constantin Strobel, 15004712 and Leon Riseborough, 15014507
 */

// C libraries 
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

// Hardware platform libraries
#include <display.h>
#include <mbed.h>
#include "asteroids.h"
#include "model.h"
#include "utils.h"
#include "view.h"

Display *graphics = Display::theDisplay();
extern "C" uint8_t flip(uint8_t);
static void drawTopBar(int score, float elapsed_time, int lives);
static void drawShip(struct ship);
static void transformShape();
static void drawMissiles(struct missile *lst);
static void drawAsteroid(struct rock *lst);

uint8_t flip(uint8_t); // This is needed for the assembly function to flip the bit pattern.
static void rock_flip();
static unsigned char flippedRock[200];

float currentHeadingForAllMissiles;

static const colour_t shipColour = rgb(255, 255, 255); // Colours the spaceship white.
static const colour_t missileColour = rgb(102, 137, 250); // Colours the rockets in a Light blue.
static const colour_t asteroidColour = rgb(130, 130, 130); // Colours the asteroids in a Medium grey.
static const colour_t background = rgb(79, 79, 79); // Sets background colour to dark grey.

static const coordinate_t shape[] = {
  {0.0f,-10.0f}, 
	{5.0f,5.0f},
	{-5.0f,5.0f}
};

static coordinate_t transformedShip[3];

/* 
 * The double buffer is needed to avoid flickering on the screen.
 * It does so by initialising the driver of the screen's second buffer frame.
 */
void init_DBuffer(void)
{   
  uint16_t *bufferbase = graphics->getFb();
  uint16_t *nextbuffer = bufferbase+ (480*272);
  LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

/* 
 * The swap buffer swaps frames of the graphical objects which 
 * have been used by the screen's driver.
 */
void swap_DBuffer(void)
{   
  uint16_t *buffer = graphics->getFb();
  graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
  LPC_LCD->UPBASE = (uint32_t)buffer;
}


/* 
 * This method draws the screen. It uses the previously defined setting of 
 * the background, missiles, ship and rockets.
 * Additionally, it informs the player to press the User button
 * in order to start the game.
 */
void draw(void)
{
	graphics->fillScreen(background);
	
	drawTopBar(score, elapsed_time, lives);
	drawMissiles(shots);
	drawAsteroid(asteroids);
	transformShape();
	drawShip(player);
	
	if(paused)
	{
		graphics->setCursor(160,80);
		graphics->printf("Press User Button to start");
		
		graphics->setCursor(220,100);
		graphics->printf("the Game"); 
	}  
  swap_DBuffer();
}

/* 
 * This method places the side bard on the right of the screen.
 * It informs the player of the name of the game, game score, lives left
 * and the player's score.
 */
void drawTopBar(int score, float elapsed_time, int lives)
{
	graphics->fillRect(0, 0, 480, 15, BLUE);
	
	graphics->setCursor(5, 5);
	graphics->printf("Lives : %d", lives);
	
	graphics->setCursor(210, 5);
	graphics->printf("Score: %d", score);
	
	graphics->setCursor(400, 5);
	graphics->printf("Time : %.2f", elapsed_time);
}

/* 
 * This method draws the spaceship which consists of three lines arranged
 * to a triangle.
 */
void drawShip(struct ship)
{
	graphics->drawLine(player.p.x+transformedShip[0].x, player.p.y+transformedShip[0].y, player.p.x+transformedShip[1].x, player.p.y+transformedShip[1].y, shipColour);
	graphics->drawLine(player.p.x+transformedShip[1].x, player.p.y+transformedShip[1].y, player.p.x+transformedShip[2].x, player.p.y+transformedShip[2].y, shipColour);
	graphics->drawLine(player.p.x+transformedShip[2].x, player.p.y+transformedShip[2].y, player.p.x+transformedShip[0].x, player.p.y+transformedShip[0].y, shipColour);
}

/* 
 * This method checks the radians of the ship and ensures that the
 * three lines used to create the shape of the ship remain in the 
 * triangle shape.
 */
void transformShape()
{
	int i;
	
	for(i = 0; i < 3 ; i++)
	{
		transformedShip[i].x = (shape[i].x*cos(radians(player.heading))) - (shape[i].y*sin(radians(player.heading)));	
		transformedShip[i].y = (shape[i].x*sin(radians(player.heading))) + (shape[i].y*cos(radians(player.heading)));	
	}
}

/* 
 * This method is needed to flip the bit of the asteroid which results
 * in the asteroid being displayed without any glitches.
 */
void rock_flip()
{
	int i;
	
	for (i = 0; i < 200; i++)
	{
		flippedRock[i] = flip(asteroid_bits[i]);
	}
}

/* 
 * This method draws the missiles. It uses a pre-defined colour which is 
 * specified at the top of this file. 
 */
void drawMissiles(struct missile *lst)
{
	while(lst)
	{
		graphics->fillCircle(lst->p.x, lst->p.y, 1, missileColour);
		lst = lst->next;
	}
}

/* 
 * It uses the bitmap to draw the asteroids by using the pre-defined
 * width, height and colour.
 */
void drawAsteroid(struct rock *lst)
{
	rock_flip();

	while(lst)
	{
		graphics->drawBitmap(lst->p.x-20, lst->p.y-20, flippedRock, asteroid_width, asteroid_height, asteroidColour);
		lst = lst->next;
	}	
}

/* 
 * This method results in informing the player of the end of the game e.g. no lives left.
 */
void gameover()
{
	graphics->setCursor(180,150);
	graphics->printf("Game Over"); 
	
	graphics->setCursor(182,160);
	graphics->printf("Your score: %d", score); 
}
