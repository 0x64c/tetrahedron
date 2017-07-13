#include "gfx.h"
#include "input.h"
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
		gfx_do();
		input_do();
	}
}
