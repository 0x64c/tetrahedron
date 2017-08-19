#include "gfx.h"
#include "input.h"
#include "game.h"
#include <SDL2/SDL.h>

#define FPS 60

void core_init(){
	gfx_init();
	input_init();
}
void core_done(){
	gfx_done();
	input_done();
}
void core_do(){
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
