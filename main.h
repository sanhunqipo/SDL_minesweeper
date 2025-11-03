#include "defs.h"

#define LOST 0
#define WON 1

#define GRID_X 8
#define GRID_Y 8

#define NUM_OF_BOMBS 5

int check_collision(SDL_Rect *, SDL_Rect *);
void tick();
void game_over(int);
void populate();
int check_adjacent(int, int);
void reveal(int, int);
