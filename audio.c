#include "audio.h"

Mix_Chunk * sfx_click = NULL;
Mix_Chunk * sfx_click2 = NULL;
Mix_Chunk * sfx_explosion = NULL;
Mix_Chunk * sfx_win = NULL;
Mix_Chunk * sfx_lose = NULL;

void init_audio()
{
	const char* driver_name = SDL_GetCurrentAudioDriver();

	if (driver_name) {
    		printf("Audio subsystem initialized: driver = %s.\n", driver_name);
	} else {
    		printf("Audio subsystem not initialized.\n");
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		fprintf(stderr, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}

	sfx_click = Mix_LoadWAV("resources/click.wav");
	sfx_click2 = Mix_LoadWAV("resources/click2.wav");
	sfx_explosion = Mix_LoadWAV("resources/explosion.wav");
	sfx_win = Mix_LoadWAV("resources/win.wav");
	sfx_lose = Mix_LoadWAV("resources/lose.wav");
}

void play_audio(int sfx_index)
{
	switch (sfx_index) {
		/* Since I stopped including the files with the macros I'll just indicate what it was with a comment */
		/* case UNCLICKED:*/
		case 0:
			Mix_PlayChannel(-1, sfx_click, 0);
			break;
		/* case SHOW_FLAG: */
		case 2:
			Mix_PlayChannel(-1, sfx_click2, 0);
			break;
		/* case EXPLOSION: */
		case 5:
			Mix_PlayChannel(-1, sfx_explosion, 0);
			break;
		/* case WON: */
		case 1:
			Mix_PlayChannel(-1, sfx_win, 0);
			break;
	}

}

void audio_destroy()
{
	Mix_FreeChunk(sfx_click);
	Mix_FreeChunk(sfx_click2);
	Mix_FreeChunk(sfx_explosion);
	Mix_FreeChunk(sfx_win);
	Mix_FreeChunk(sfx_lose);
	Mix_Quit();
}
