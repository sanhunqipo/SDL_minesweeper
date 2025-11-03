#include "defs.h"

#define UNCLICKED 0
#define EMPTY 1
//#define UNUSED 2
//#define UNUSED 3
//#define UNUSED 4
#define EXPLOSION 5
#define NUM1 6
#define NUM2 7
#define NUM3 8
#define NUM4 9
#define NUM5 10
#define NUM6 11
#define NUM7 12
#define NUM8 13

#define DONT_SHOW 0
#define SHOW_TRUE 1
#define SHOW_FLAG 2

#define HAPPY 1
#define ANGER 2
#define SWEAT 3
#define IDLE 4

typedef struct blocks {
	SDL_Rect block[GRID_Y][GRID_X];
	int state[GRID_Y][GRID_X];
	int show[GRID_Y][GRID_X];
	int bomb[GRID_Y][GRID_X];
} blks;

typedef struct smiley {
	SDL_Rect rect;
	int state;
} smile;

extern SDL_Window * window;
extern SDL_Renderer * rend;

extern blks grid;
extern smile joe;

SDL_Texture * create_tex(SDL_Window *, SDL_Renderer *, const char *);
void init_video();
void video_loop();
void video_destroy();
