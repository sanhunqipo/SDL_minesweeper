#include "main.h"
#include "graphics.h"
#include "audio.h"

#include <stdlib.h>
#include <time.h>

SDL_Rect player;
int pause = 0;

int main(void)
{
	srand(time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO) < 0)
	{
		fprintf(stderr,
                "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
		return 1;
	}

	init_video();
	init_audio();

	player.w = 1;
	player.h = 1;

	tick();

	video_destroy();
	audio_destroy();

	SDL_Quit();

	return 0;
}

void tick()
{
	int close_requested = 0;

	SDL_Event event;

	int r_mouse = 0;
	int l_mouse = 0;

	srand(time(NULL));

	int player_bombs = NUM_OF_BOMBS;
	int player_unclicked = GRID_X * GRID_Y;

	populate();

	/* Check the board and give all blocks their proper values */
	for (int i = 0; i < GRID_Y; i++) for (int j = 0; j < GRID_X; j++)
	{
		int bombs_block = check_adjacent(i, j); /* Store value to not check twice */
		if (bombs_block > 0 && bombs_block < 9) {
			grid.state[i][j] = bombs_block + 5; /* In the DEFINE macros the numerical blocks start at 6 */
		} else if (bombs_block == 9) {
			grid.state[i][j] = EXPLOSION;
		}
	}


	while(!close_requested)
	{
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
				{
					close_requested = 1;
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_RIGHT) r_mouse = 1;
					if (event.button.button == SDL_BUTTON_LEFT) l_mouse = 1;
					break;
			}
		}

		if (!pause)
		{
			if (player_bombs == 0) {
				for (int i = 0; i < GRID_Y; i++) for (int j = 0; j < GRID_X; j++)
					if (grid.show[i][j] == SHOW_FLAG && grid.state[i][j] == EXPLOSION) player_bombs++;
				if (player_bombs == NUM_OF_BOMBS)
					game_over(WON);
			}

			/* This is probably inneficient, a last minute add because I forgot this is also a win condition of minesweeper */
			player_unclicked = GRID_X * GRID_Y;
			for (int i = 0; i < GRID_Y; i++) for (int j = 0; j < GRID_X; j++)
			{
				if (grid.show[i][j] == SHOW_TRUE) player_unclicked--;
			}

			if (player_unclicked == NUM_OF_BOMBS) game_over(WON);

			if (player_bombs == 1) {
				joe.state = SWEAT;
			}
			else if (joe.state == SWEAT) {
				joe.state = IDLE;
			}

			int mouse_x, mouse_y;
			SDL_GetMouseState(&mouse_x, &mouse_y);

			/* Update position */
			player.x = mouse_x;
			player.y = mouse_y;

			for (int i = 0; i < GRID_Y; i++) for (int j = 0; j < GRID_X; j++)
			{
				if (check_collision(&player, &grid.block[i][j])) {
					if (r_mouse == 1) {
						r_mouse = 0; /* Reset the mouse the moment a single collision happened to ensure only a single block is clicked */
						if (!grid.show[i][j] && player_bombs != 0) {
							play_audio(SHOW_FLAG);
							grid.show[i][j] = SHOW_FLAG;
							player_bombs--;
							break;
						}
						if (grid.show[i][j] == SHOW_FLAG) {
							play_audio(SHOW_FLAG);
							grid.show[i][j] = DONT_SHOW;
							player_bombs++;
							break;
						}
						break; /* No need to keep checking since the mouse is reset */
					}
					if (l_mouse == 1) {
						l_mouse = 0;
						reveal(i, j);
						break;
					}
				}
			}
		}

		video_loop();
		SDL_Delay(1000/60);

        }

	return;
}

void game_over(int winstate)
{
	for (int i = 0; i < GRID_Y; i++) for (int j = 0; j < GRID_X; j++) {
		grid.show[i][j] = SHOW_TRUE;
		if (grid.state[i][j] == UNCLICKED) grid.state[i][j] = EMPTY;

	}

	if (winstate == LOST) {
		joe.state = ANGER;
		/* play_audio(LOST); the explosion sound is enough and avoids conflict with explosion sound */
	}
	if (winstate == WON) {
		joe.state = HAPPY;
		play_audio(WON);
	}

	pause = 1;
}

int check_adjacent(int i, int j) /* Got too used to dealing with i and j with the for loops I guess */
{
	int bombs = 0; /* This value stores the amount of bombs around a block not the amount of bombs on the whole grid */

	/* check middle */
	if (grid.bomb[i][j]) return 9; /* Since there can never be more than 8 bombs, this is a safe value to use */

	/* check upper left */
	if (i - 1 >= 0 && j - 1 >= 0) {
		if (grid.bomb[i - 1][j - 1]) bombs++;
	}

	/* check upper middle */
	if (i - 1 >= 0) {
		if (grid.bomb[i - 1][j]) bombs++;
	}

	/* check upper right */
	if (i - 1 >= 0 && j + 1 < GRID_X ) {
		if (grid.bomb[i - 1][j + 1]) bombs++;
	}

	/* check middle left */
	if (j - 1 >= 0) {
		if (grid.bomb[i][j - 1]) bombs++;
	}

	/* check middle right */
	if (j + 1 < GRID_X ) {
		if (grid.bomb[i][j + 1]) bombs++;
	}

	/* check lower left */
	if (i + 1 < GRID_Y && j - 1 >= 0) {
		if (grid.bomb[i + 1][j - 1]) bombs++;
	}

	/* check lower middle */
	if (i + 1 < GRID_Y) {
		if (grid.bomb[i + 1][j]) bombs++;
	}

	/* check lower right */
	if (i + 1 < GRID_Y && j + 1 < GRID_X) {
		if (grid.bomb[i + 1][j + 1]) bombs++;
	}

	return bombs;
}

void reveal(int i, int j)
{
	if (!grid.show[i][j]) {
		grid.show[i][j] = SHOW_TRUE;
		if (grid.state[i][j] == EXPLOSION) {
			play_audio(EXPLOSION);
			game_over(LOST);
		} else {
			play_audio(UNCLICKED);
		}
		if (grid.state[i][j] == UNCLICKED) {
			grid.state[i][j] = EMPTY;
		}
	}

	if (grid.state[i][j] == EMPTY) {
		/* check upper middle */
		if (i - 1 >= 0) {
			if (grid.state[i - 1][j] == UNCLICKED)
			{
				grid.state[i - 1][j] = EMPTY;
				grid.show[i - 1][j] = SHOW_TRUE;
				reveal(i - 1, j);
			} else {
				grid.show[i - 1][j] = SHOW_TRUE;
			}
		}

		/* check middle left */
		if (j - 1 >= 0) {
			if (grid.state[i][j - 1] == UNCLICKED)
			{
				grid.state[i][j - 1] = EMPTY;
				grid.show[i][j - 1] = SHOW_TRUE;
				reveal(i, j - 1);
			} else {
				grid.show[i][j - 1] = SHOW_TRUE;
			}
		}

		/* check middle right */
		if (j + 1 < GRID_X ) {
			if (grid.state[i][j + 1] == UNCLICKED)
			{
				grid.state[i][j + 1] = EMPTY;
				grid.show[i][j + 1] = SHOW_TRUE;
				reveal(i, j + 1);
			} else {
				grid.show[i][j + 1] = SHOW_TRUE;
			}
		}

		/* check lower middle */
		if (i + 1 < GRID_Y) {
			if (grid.state[i + 1][j] == UNCLICKED)
			{
				grid.state[i + 1][j] = EMPTY;
				grid.show[i + 1][j] = SHOW_TRUE;
				reveal(i + 1, j);
			} else {
				grid.show[i + 1][j] = SHOW_TRUE;
			}
		}

	}


}

/* Populate grid with bombs */
void populate()
{
	for (int i = 0; i < NUM_OF_BOMBS; i++) {
		int rand_x = rand() % GRID_X;
		int rand_y = rand() % GRID_Y;

		if (!grid.bomb[rand_y][rand_x]) {
			grid.bomb[rand_y][rand_x] = 1;
		} else {
			--i;
		}
	}
}

int check_collision(SDL_Rect * obj1, SDL_Rect * obj2)
{
	if (obj1->x < obj2->x + obj2->w && obj1->x + obj1->w > obj2->x) {
    		if ( obj1->y < obj2->y + obj2->h && obj1->y + obj1->h > obj2->y) {
			/* collision happened */
			return 1;
		}
	}
	return 0;
}
