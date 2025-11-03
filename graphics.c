#include "main.h"
#include "graphics.h"

SDL_Window * window = NULL;
SDL_Renderer * rend = NULL;

/* lol */
SDL_Texture * block_tex = NULL;
SDL_Texture * flag_tex = NULL;
SDL_Texture * explode_tex = NULL;
SDL_Texture * click_tex = NULL;
SDL_Texture * num1_tex = NULL;
SDL_Texture * num2_tex = NULL;
SDL_Texture * num3_tex = NULL;
SDL_Texture * num4_tex = NULL;
SDL_Texture * num5_tex = NULL;
SDL_Texture * num6_tex = NULL;
SDL_Texture * num7_tex = NULL;
SDL_Texture * num8_tex = NULL;
SDL_Texture * smiley_idle_tex = NULL;
SDL_Texture * smiley_sweat_tex = NULL;
SDL_Texture * smiley_anger_tex = NULL;
SDL_Texture * smiley_happy_tex = NULL;

smile joe;
blks grid;

SDL_Texture * create_tex(SDL_Window * window, SDL_Renderer * rend, const char * file)
{
	/* Load image in memory */
	SDL_Surface * surface = IMG_Load(file);

	if (!surface)
	{
		fprintf(stderr,
                "Couldn't create surface: %s\n", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(window);
		SDL_Quit();
        exit(1);
	}

	/* Load image data in vram */
	SDL_Texture * tex = SDL_CreateTextureFromSurface(rend, surface);
	SDL_FreeSurface(surface);

	if (!tex)
	{
		fprintf(stderr,
                "Couldn't create texture: %s\n", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(window);
		SDL_Quit();
        exit(1);

	}
	return tex;
}


void init_video()
{
	window = SDL_CreateWindow(
			"test",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			SDL_WINDOW_MAXIMIZED
			);

	if (!window)
	{
		fprintf(stderr,
                "Couldn't create window: %s\n", SDL_GetError());
        exit(1);
	}

	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

    	rend = SDL_CreateRenderer(window, -1, render_flags);

	/* * *
	   VERSION THAT ACCOUNTS FOR GAPS, IT DOES NOT WORK PROPERLY AT AROUND 16x16 AND UP
	 * * *
	for (int i = 0; i < GRID_Y; i++) {
		for (int j = 0; j < GRID_X; j++) {
			grid[i][j].w = (WINDOW_WIDTH * 0.9) / (GRID_X + 1);
			grid[i][j].h = (WINDOW_HEIGHT * 0.9) / (GRID_Y + 1);
			grid[i][j].x = ((WINDOW_WIDTH - (WINDOW_WIDTH * 0.9)) / 2) + (j * (grid[i][j].w + (((WINDOW_WIDTH * 0.9) / (GRID_X + 1)) / (GRID_X - 1))));
			grid[i][j].y = ((WINDOW_HEIGHT - (WINDOW_HEIGHT * 0.9)) / 2) + (i * (grid[i][j].h + (((WINDOW_HEIGHT * 0.9) / (GRID_Y + 1)) / (GRID_Y - 1))));
			// drinking game: count the parentheses
		}
	}
	* * */

	for (int i = 0; i < GRID_Y; i++) {
		for (int j = 0; j < GRID_X; j++) {
			grid.block[i][j].w = (WINDOW_WIDTH * 0.75) / GRID_Y;
			grid.block[i][j].h = (WINDOW_HEIGHT * 0.75) / GRID_Y;
			grid.block[i][j].x = ((WINDOW_WIDTH - (WINDOW_WIDTH * 0.75)) / 2) + (j * (grid.block[i][j].w));
			grid.block[i][j].y = ((((WINDOW_HEIGHT + (WINDOW_HEIGHT * 0.3)) - (WINDOW_HEIGHT * 0.9)) / 2) + (i * (grid.block[i][j].h)));
		}
	}

	joe.rect.w = (WINDOW_HEIGHT * 0.2);
	joe.rect.h = (WINDOW_HEIGHT * 0.2);
	joe.rect.x = (WINDOW_WIDTH - joe.rect.w) / 2;
	joe.rect.y = 0;

	joe.state = IDLE;

    	block_tex = create_tex(window, rend, "resources/ground.png");
    	flag_tex = create_tex(window, rend, "resources/flag.png");
    	explode_tex = create_tex(window, rend, "resources/bomb.png");
    	click_tex = create_tex(window, rend, "resources/click.png");
    	num1_tex = create_tex(window, rend, "resources/num1.png");
    	num2_tex = create_tex(window, rend, "resources/num2.png");
    	num3_tex = create_tex(window, rend, "resources/num3.png");
    	num4_tex = create_tex(window, rend, "resources/num4.png");
    	num5_tex = create_tex(window, rend, "resources/num5.png");
    	num6_tex = create_tex(window, rend, "resources/num6.png");
    	num7_tex = create_tex(window, rend, "resources/num7.png");
    	num8_tex = create_tex(window, rend, "resources/num8.png");
    	smiley_idle_tex = create_tex(window, rend, "resources/smiley_idle.png");
    	smiley_sweat_tex = create_tex(window, rend, "resources/smiley_sweat.png");
    	smiley_anger_tex = create_tex(window, rend, "resources/smiley_anger.png");
    	smiley_happy_tex = create_tex(window, rend, "resources/smiley_happy.png");

}

void video_loop()
{
    	/* Clear the window */
	SDL_SetRenderDrawColor(rend, 211, 211, 211, 255);
	//SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    	SDL_RenderClear(rend);

	//SDL_SetRenderDrawColor(rend, 100, 100, 100, 255);
	//SDL_RenderFillRects(rend, grid, GRID_X * GRID_Y);

	/* Draw image */
	for (int i = 0; i < GRID_Y; i++) for (int j = 0; j < GRID_X; j++)
	{
		switch (grid.state[i][j]) {

			case EXPLOSION:
				if (grid.show[i][j] == SHOW_TRUE)
				{
					SDL_RenderCopy(rend, explode_tex, NULL, &grid.block[i][j]);
					break;
				}

			case NUM1:
				if (grid.show[i][j] == SHOW_TRUE)
				{
					SDL_RenderCopy(rend, num1_tex, NULL, &grid.block[i][j]);
					break;
				}

			case NUM2:
				if (grid.show[i][j] == SHOW_TRUE)
				{
					SDL_RenderCopy(rend, num2_tex, NULL, &grid.block[i][j]);
					break;
				}

			case NUM3:
				if (grid.show[i][j] == SHOW_TRUE)
				{
					SDL_RenderCopy(rend, num3_tex, NULL, &grid.block[i][j]);
					break;
				}

			case NUM4:
				if (grid.show[i][j] == SHOW_TRUE)
				{
					SDL_RenderCopy(rend, num4_tex, NULL, &grid.block[i][j]);
					break;
				}

			case NUM5:
				if (grid.show[i][j] == SHOW_TRUE)
				{
					SDL_RenderCopy(rend, num5_tex, NULL, &grid.block[i][j]);
					break;
				}

			case NUM6:
				if (grid.show[i][j] == SHOW_TRUE)
				{
					SDL_RenderCopy(rend, num6_tex, NULL, &grid.block[i][j]);
					break;
				}

			case NUM7:
				if (grid.show[i][j] == SHOW_TRUE)
				{
					SDL_RenderCopy(rend, num7_tex, NULL, &grid.block[i][j]);
					break;
				}

			case NUM8:
				if (grid.show[i][j] == SHOW_TRUE)
				{
					SDL_RenderCopy(rend, num8_tex, NULL, &grid.block[i][j]);
					break;
				}

			case EMPTY:
				if (grid.show[i][j] == SHOW_TRUE)
				{
					SDL_RenderCopy(rend, click_tex, NULL, &grid.block[i][j]);
					break;
				}

			case UNCLICKED:
				if (grid.show[i][j] == SHOW_FLAG) {
					SDL_RenderCopy(rend, flag_tex, NULL, &grid.block[i][j]);
					break;
				}
				SDL_RenderCopy(rend, block_tex, NULL, &grid.block[i][j]);
				break;

		}

		switch (joe.state) {
			case HAPPY:
				SDL_RenderCopy(rend, smiley_happy_tex, NULL, &joe.rect);
				break;
			case ANGER:
				SDL_RenderCopy(rend, smiley_anger_tex, NULL, &joe.rect);
				break;
			case SWEAT:
				SDL_RenderCopy(rend, smiley_sweat_tex, NULL, &joe.rect);
				break;
			case IDLE:
				SDL_RenderCopy(rend, smiley_idle_tex, NULL, &joe.rect);
				break;
		}
	}

	SDL_RenderPresent(rend);

}

void video_destroy()
{
   	SDL_DestroyTexture(block_tex);
   	SDL_DestroyTexture(explode_tex);
   	SDL_DestroyTexture(flag_tex);
   	SDL_DestroyTexture(click_tex);
   	SDL_DestroyTexture(num1_tex);
   	SDL_DestroyTexture(num2_tex);
   	SDL_DestroyTexture(num3_tex);
   	SDL_DestroyTexture(num4_tex);
   	SDL_DestroyTexture(num5_tex);
   	SDL_DestroyTexture(num6_tex);
   	SDL_DestroyTexture(num7_tex);
   	SDL_DestroyTexture(num8_tex);
    	SDL_DestroyTexture(smiley_idle_tex);
    	SDL_DestroyTexture(smiley_sweat_tex);
    	SDL_DestroyTexture(smiley_anger_tex);
    	SDL_DestroyTexture(smiley_happy_tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	IMG_Quit();
}
