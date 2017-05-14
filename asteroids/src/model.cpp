/* 
 * Assignment 2 - Small Embedded Systems.
 * Constantin Strobel, 15004712 and Leon Riseborough, 15014507
 * 
 * This file is responsible for the settings regarding movements and actions in the game.
 */
 
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "utils.h"
#include "asteroids.h"
#include "view.h"

static const float dragFactor = 0.02;
void newMissiles(struct missile *r); 
void newAsteroid(struct rock *r); 
static missile_t *freeMissileNodes;
static rock_t *freeRockNodes;
static const float accFactor = 1.0; 
static const size_t MAXSize = 10;
static rock_t rock_data[MAXSize];
static bool damagedShip(struct rock *l); 
static void updateMissileList(struct missile *l); 
static void updateRockList(struct rock *l); 
static void missileMovement(struct missile *l); 
static void asteroidMovements(struct rock *l, float x); 
static missile_t missileData[MAXSize]; 
static void movingShip(ship, float x); 
struct rock * newAsteroids(); 
static void hitAsteroid(struct missile *l, struct rock *k);


/*
 * This method checks for time elapsed, movement of the ship, rockets and asteroids.
 * It does so by using the methods written below.
 */
void physics(void)
{
	if (!paused) // Only performs the tasks below when the game is NOT paused.
	{
		elapsed_time += Dt;
		movingShip(player, Dt);
		missileMovement(shots);
		asteroids = newAsteroids();
		asteroidMovements(asteroids, Dt);
		hitAsteroid(shots, asteroids);
			
		if(damagedShip(asteroids)) // Pauses the once the ship hit an asteroid.
		{
			inPlay = false;
			score += (int) elapsed_time;
			lives--;
		}
		updateMissileList(shots);
		updateRockList(asteroids);
	}
}


/*
 * Allows for forward movement using radians and the acceleration factor.
 */
void moveForward()
{
	float xN = sin(radians(player.heading)); // A normilised vector is created for the direction it is traveling in.
	float yN = cos(radians(player.heading)); // A normilised vector is created for the direction it is traveling in.
	
	player.v.x += xN * accFactor; // This adds velocity to the ship when moving forward.
	player.v.y -= yN * accFactor; // This adds velocity to the ship when moving forward.
}

/*
 * Allows for backwards movement using radians and the acceleration factor.
 */
void moveBackwards()
{
	float xN = sin(radians(player.heading)); // A normilised vector is created for the direction it is traveling in.
	float yN = cos(radians(player.heading)); // A normilised vector is created for the direction it is traveling in.
	
	player.v.x -= xN * accFactor; // This adds velocity to the ship when moving backwards.
	player.v.y += yN * accFactor; // This adds velocity to the ship when moving backwards.
}

/*
 * Determines how far each click of a button the ship will turn to the right.
 */
void turnRight()
{
	player.heading += 10.0f;
}

/*
 * Determines how far each click of a button the ship will turn to the left.
 */
void turnLeft()
{
	player.heading -= 10.0f;
}

/*
 * Updates the position of the ship by checking the distance which it has undertaken.
 * Additionally it applies drag the to the ship and it ensures that the ship stays
 * within the boundaries of the screen.
 */
void movingShip(ship, float x)
{
	player.p.x += player.v.x; // Updates position of the ship.
	player.p.y += player.v.y; // Updates position of the ship.
	
	player.v.x = player.v.x - (player.v.x * dragFactor); // Allows for drag to be measured.
	player.v.y = player.v.y - (player.v.y * dragFactor); // Allows for drag to be measured.
	
	if(player.p.x < 0) 	player.p.x += 460; // Ensures that the ship will stay within its boundaries of the screen.
	if(player.p.x >= 460) player.p.x -= 460; // Ensures that the ship will stay within its boundaries of the screen.
	if(player.p.y < 0) 		player.p.y += 280; // Ensures that the ship will stay within its boundaries of the screen.
	if(player.p.y >= 280) player.p.y -= 280; // Ensures that the ship will stay within its boundaries of the screen.
}


/*
 * Creates the rocket.
 * Ensures that only one rocket can be fired at the time.
 * Updates the positions of the rockets.
 */
void fireRockets()
{
	struct missile *newMissile = locateMissileNode();
	
	if(newMissile)
	{
		newMissile->next = shots;
		shots = newMissile;
		newMissiles(newMissile);
	}
	updateMissileList(shots);
}

/*
 * Constructs a new missile upon pressing the joystick's centre button.
 */
void newMissiles(struct missile *r)
{
	r->heading = player.heading;
	
	r->p.x = player.p.x;
	r->p.y = player.p.y;
	
	r->v.x = sin(radians(r->heading));
	r->v.y = -cos(radians(r->heading));
	
	r->p.x += r->v.x*20;
	r->p.y += r->v.y*20;
	
	r->v.x *= 50;
	r->v.y *= 50;
	
	r->ttl = 20;
}

/*
 * This method allows for new missiles to be initialised.
 */
void initMissiles()
{
	int n;
	for(n=0; n<(MAXSize-1) ; n++)
	{
		missileData[n].next = &missileData[n+1];
	}
	missileData[n].next = NULL;
	freeMissileNodes = &missileData[0];
}

missile_t *locateMissileNode(void)
{
	missile_t *node = NULL;
	
	if (freeMissileNodes)
	{
		node = freeMissileNodes;
		freeMissileNodes = freeMissileNodes->next;
	}
	return node;
}

void missileNode(missile_t *n)
{
	n->next = freeMissileNodes;
	freeMissileNodes = n;
}

/*
 * Updates the missile list.
 */
void updateMissileList(struct missile *l)
{
	for ( ; l ; l = l->next){		
		if (l->next->ttl <= 0){
			struct missile *expiredM = l->next;
			l->next = l->next->next;
			missileNode(expiredM);
		}		
	}
}

/*
 * This method determines the movements of each missile fired.
 */
void missileMovement(struct missile *l)
{
	for ( ; l ; l = l->next)
	{
		l->p.x += l->v.x * Dt;
		l->p.y += l->v.y * Dt;
		
		if (l->p.y >= 280) 	l->ttl = 0;
		if (l->p.y < 0) 		l->ttl = 0;
		if (l->p.x >= 480) 	l->ttl = 0;
		if (l->p.x < 0) 		l->ttl = 0;
		
		l->ttl -= Dt;	
	}	
}


/*
 * Determines the movements of the asteroids and their limitations.
 * Determines the amount of asteroids on the screen and where each asteroid starts at the 
 * beginning of the game.
 */
struct rock * newAsteroids()
{
	int i;
	for(i = 0; i < MAXSize-1; i++)
	{
		struct rock *newRock = locateRockNode();
		if(newRock){
			newRock->next = asteroids;
			asteroids = newRock;
			
			int initAstPos = randrange(1, 5); // These are the intial values of the asteroids.
			switch (initAstPos)
			{
				case 1 : 
					newRock->p.y = randrange(0, 272); 
					newRock->v.x = 1;
					if(newRock->p.y > 136) newRock->v.y = -10;
					if(newRock->p.y <= 136) newRock->v.y = 10;
				break; // Places asteroids on the left side of the screen.
				case 2 : 
					newRock->p.x = randrange(280, 460);
					newRock->p.y = randrange(0, 280);
					newRock->v.x = -1;
					if(newRock->p.y > 136) newRock->v.y = -10;
					if(newRock->p.y <= 136) newRock->v.y = 10;
				break; // Places asteroids on the right side of the screen.
				case 3 : 
					newRock->p.x = randrange(0, 460); 
					newRock->p.y = randrange(0, 100);
					newRock->v.y = 1;
					if(newRock->p.x > 200) newRock->v.x = -10;
					if(newRock->p.y <= 200) newRock->v.x = 10;
				break; // Places asteroids on the top of the screen.
				case 4 : 
					newRock->p.x = randrange(0, 460); 
					newRock->p.y = randrange(172, 272);
					newRock->v.y = 1;
					if(newRock->p.x > 200) newRock->v.x = -10;
					if(newRock->p.y <= 200) newRock->v.x = 10;
				break; // Places asteroids on the bottom of the screen.
			}
			newRock->ttl = 2000;
		}
		updateRockList(asteroids);
	}
	return asteroids;
}

/*
 * This intialies the asteroids every time the centre button
 * has been pressed while the game was paused.
 */
void initAsteroids()
{
	int n;
	
	for(n=0; n<(MAXSize-1) ; n++)
	{
		rock_data[n].next = &rock_data[n+1];
	}
	rock_data[n].next = NULL;
	freeRockNodes = &rock_data[0];	
}

rock_t *locateRockNode(void)
{
	rock_t *node = NULL;
	
	if (freeRockNodes)
	{
		node = freeRockNodes;
		freeRockNodes = freeRockNodes->next;
	}
	return node;
}

/*
 * Updates the amount of rocks on the screen.
 */
void updateRockList(struct rock *l)
{
	for ( ; l ; l = l->next)
	{
		if (l->next->ttl <= 0)
		{
			struct rock *expiredR = l->next;
			l->next = l->next->next;
			asteroidNode(expiredR);
		}		
	}
}

void asteroidNode(rock_t *n)
{
	n->next = freeRockNodes;
	freeRockNodes = n;
}

/*
 * Determines the movements of the asteroid.
 * Updates the position of each asteroid.
 * Additionally, it checks the asteroid's and replace them
 * if the get to the end of the screen.
 */
void asteroidMovements(struct rock *l, float x)
{
	for ( ; l ; l = l->next){
		l->p.x += l->v.x * x;
		l->p.y += l->v.y * x;
		
		if (l->p.y >= 280) 	l->p.y -= 280;
		if (l->p.y < 35) 		l->p.y += 280;
		if (l->p.x >= 460) 	l->p.x -= 460;
		if (l->p.x < 0) 		l->p.x += 460;
		
		l->ttl -= Dt;	
	}	
}

/*
 * This checks the asteroid for any rockets that may have impacted them.
 * Removes asteroids that have been hit by a rocket. 
 */
void hitAsteroid(struct missile *l, struct rock *k)
{
	for ( ; l ; l = l->next) // Each missile is being compared.
	{ 
		for( ; k ; k =k->next)
		{ 
			float r1 = 1; // Missile's radius.
			float r2 = 20; // Asteroid's radius.
			float dx = l->p.x - k->p.x; float dy = l->p.y - k->p.y;
			float distance = sqrt(dx*dx + dy*dy);
			
			if (distance < (r1 + r2)) // If a collision has been detected.
			{
				l->ttl = 0;
				k->ttl = 0;
				score += 5; // Adds 5 points to the player's score.
			} // Missile and asteroid have been removed after impact.
		}
	}
}

/*
 * Checks for asteroids the ship collides with.
 */
bool damagedShip(struct rock *l)
{
	for( ; l ; l =l->next)
	{
		float r1 = 7.9; // Ship's radius.
		float r2 = 20; // Asteroid's radius.
		float dx = l->p.x - player.p.x; float dy = l->p.y - player.p.y;
		float distance = sqrt(dx*dx + dy*dy);
		
		if (distance < (r1 + r2)) // If a collision has been detected.
		{
			l->ttl = 0;
			return true;
		} // Asteroid is being removed.
	}
	return false;
}
