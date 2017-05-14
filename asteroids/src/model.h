/*
 * Assignment 2 - Small Embedded Systems.
 * Constantin Strobel, 15004712 and Leon Riseborough, 15014507
 */

// Asteroids Model
struct point {
    float x,y;
};
typedef struct point coordinate_t;
typedef struct point vector_t;

// Some initial struct types.
struct ship {
	  float heading;
    coordinate_t p;
    vector_t     v;
};

// initial structs for building linked lists.
typedef struct rock {
	  float heading;
    coordinate_t p;
	  vector_t     v;
	  float ttl;
    struct rock *next;
} rock_t;

// Initial struct to build the missiles.
typedef struct missile {
	  float heading;
    coordinate_t p;
	  vector_t     v;
	  float ttl;
    struct missile *next;
} missile_t;

void physics(void);
void moveForward();
void moveBackwards();
void turnRight();
void turnLeft();
void fireRockets();
void initMissiles();
void initAsteroids();
void asteroidNode(rock_t *n);
void missileNode(missile_t *n);
rock_t *locateRockNode(void);
missile_t *locateMissileNode(void);

