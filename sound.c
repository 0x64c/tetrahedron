#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>

#define bgmpath "music_001.ogg"

char *sfx_filenames[]={"snd000.ogg","snd001.ogg","snd002.ogg",NULL};
Mix_Chunk **sfx;

Mix_Music *bgm=NULL;

void sound_play(int i){
    Mix_PlayChannel(-1,sfx[i],0);
}

void sound_startbgm(){
    Mix_PlayMusic(bgm,-1);
}

void sound_pausebgm(){
    Mix_PauseMusic();
}

void sound_init(){
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,4096);
    bgm=Mix_LoadMUS(bgmpath);
    int i;
    for(i=0;sfx_filenames[i]!=NULL;i++)continue;
    int j;
    sfx=malloc(i*sizeof(Mix_Chunk*));
    for(j=0;j<i;j++){
        sfx[j]=Mix_LoadWAV(sfx_filenames[j]);
    }
}

void sound_done(){
    int i;
    for(i=0;sfx_filenames[i]!=NULL;i++)Mix_FreeChunk(sfx[i]);
    free(sfx);
    Mix_HaltMusic();
    Mix_FreeMusic(bgm);
    Mix_CloseAudio();
    Mix_Quit();
}

void sound_do(){
}

