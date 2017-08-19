#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define bgmpath "music_001.wav"

Mix_Music *bgm=NULL;

void sound_play(){
}

void sound_startbgm(){
	Mix_PlayMusic(bgm,-1);
}

void sound_pausebgm(){
	Mix_PauseMusic();
}

void sound_init(){
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,512);
	bgm=Mix_LoadMUS(bgmpath);
}

void sound_done(){
	Mix_HaltMusic();
	Mix_FreeMusic(bgm);
	Mix_CloseAudio();
	Mix_Quit();
}

void sound_do(){
}

