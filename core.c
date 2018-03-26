#ifndef SDL_h_
#include <SDL2/SDL.h>
#define SDL_h_
#endif
#include "core.h"
#include "gfx.h"
#include "input.h"
#include "game.h"
#include <stdlib.h>

#define FPS 60

void core_init(void){
	gfx_init();
	input_init();
}
void core_done(void){
	gfx_done();
	input_done();
}
void core_do(void){
	unsigned int start=SDL_GetTicks();
	unsigned int time=0;
	while(!QUIT){
		time=SDL_GetTicks();
		if(1000/FPS<time-start){
			SDL_Delay(1000/FPS);
			start=time;
		}
		if(game_state>1)game_do();
		gfx_do();
		input_do();
	}
	if(game_state>1)game_done();
}
