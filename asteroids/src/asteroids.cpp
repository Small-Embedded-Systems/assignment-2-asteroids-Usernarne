/*
 * Assignment 2 - Small Embedded Systems.
 * Constantin Strobel, 15004712 and Leon Riseborough, 15014507
 * 
 * This file is responsible for calling the methods needed in order to play the game.
 */

// These are the C libraries.
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

// Hardware files
#include <display.h>
#include <mbed.h>

// Main elements needed for the game.
#include "model.h"
#include "view.h"
#include "controller.h"

// States of the game.
float elapsed_time = 0.0f;
float Dt = 0.01f;
void startButton(); 
void startGame(); 
int score = 0;
int lives = 5;
bool paused = true;
bool inPlay;
struct ship player;
struct missile *shots = NULL;
struct rock *asteroids = NULL;

Ticker model, view, controller;

DigitalIn userbutton(P2_10,PullUp); // Pull up resister must be enabled, otherwise the user button would not work.

/* 
 * The main method intialises the buffer, drawings, physics, and the game.
 */
int main()
{
  init_DBuffer();
  view.attach( draw, 0.055 );
  model.attach( physics, Dt );
  controller.attach( controls, 0.1 );

	while(true) 
	{			
		startGame();
		while (lives > 0)
		{
			startButton();
			inPlay = true;
			
			while(inPlay)
			{
				// Do nothing.
			}
		}			
		gameover();
  }
}

/* 
 * This allows the player to start the game by pressing the User button.
 */
void startButton()
{
	while(userbutton.read())
	{
		paused=true;
    wait_ms(200);
  }
	paused = false;	
}

/* 
 * This method initialising the ship and its position, missiles and asteroids.
 */
void startGame()
{
  player.p.x = 240; // This places the ship in the middle of the screen.
	player.p.y = 145; // This places the ship in the middle of the screen.
	player.v.x = 0; // This ensures that the ship is motionless when a new game is started.
	player.v.y = 0; // This ensures that the ship is motionless when a new game is started.
	player.heading = 0.0; 
	initMissiles(); // Intialises missiles.
	initAsteroids(); // Intialises asteroids.
}


