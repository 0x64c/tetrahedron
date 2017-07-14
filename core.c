#include "gfx.h"
#include "input.h"
#include "game.h"
void core_init(){
	gfx_init();
	input_init();
}
void core_done(){
	gfx_done();
	input_done();
}
void core_do(){
	while(!QUIT){
		if(game_state)game_do();
		gfx_do();
		input_do();
	}
}
