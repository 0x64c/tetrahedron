#include <SDL2/SDL.h>
#include "menu.h"
#include "gfx.h"
#include "game.h"
#ifndef MYDEF
#include "mytypes.h"
#endif
#include <stdlib.h>
#include "sound.h"
SDL_Event e;
SDL_Joystick *gamepad=NULL;
int joydeadzone=1<<10;
int QUIT=0;

#define GCWJOYSTICK "linkdev device (Analog 2-axis 8-button 2-hat)"

void input_init(){
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	if (SDL_NumJoysticks()>0) {
#ifdef GCW
		for (i = 0; i < SDL_NumJoysticks(); i++)
			if (strcmp(SDL_JoystickName(i), GCWJOYSTICK) == 0) gamepad = SDL_JoystickOpen(i);
#else
		gamepad=SDL_JoystickOpen(0);
#endif
	}
}

#ifdef _GCW_
typedef enum{
	mykeys_up=SDLK_UP,
	mykeys_down=SDLK_DOWN,
	mykeys_left=SDLK_LEFT,
	mykeys_right=SDLK_RIGHT,
	mykeys_a=SDLK_LCTRL,
	mykeys_b=SDLK_LALT,
	mykeys_x=SDLK_LSHIFT,
	mykeys_y=SDLK_SPACE,
	mykeys_l=SDLK_TAB,
	mykeys_r=SDLK_BACKSPACE,
	mykeys_select=SDLK_ESCAPE,
	mykeys_start=SDLK_RETURN,
	mykeys_hold=SDLK_PAUSE,
	mykeys_power=SDLK_HOME
}keys;
#else
typedef enum{
	mykeys_up=SDLK_w,
	mykeys_down=SDLK_s,
	mykeys_left=SDLK_a,
	mykeys_right=SDLK_d,
	mykeys_a=SDLK_SPACE,
	mykeys_b=SDLK_LSHIFT,
	mykeys_x=SDLK_q,
	mykeys_y=SDLK_e,
	mykeys_l=SDLK_RETURN,
	mykeys_r=SDLK_BACKSPACE,
	mykeys_select=SDLK_ESCAPE,
	mykeys_start=-3,
	mykeys_hold=SDLK_PAUSE,
	mykeys_power=SDLK_HOME
}keys;
#endif

void input_done(){
	if(gamepad)SDL_JoystickClose(gamepad);
	SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

void input_do(){
	/*if(gamepad){
		int joy_x = SDL_JoystickGetAxis(gamepad,0);
		int joy_y = SDL_JoystickGetAxis(gamepad,1);
		int x,y;
		if( (joy_x>joydeadzone) || (joy_x<-joydeadzone) ){

		}if( (joy_y>joydeadzone) || (joy_y<-joydeadzone) ){

		}
	}*/
	while(SDL_PollEvent(&e)!=0){
		switch(menu_state){
			case MAIN_MENU:
				if(e.type==SDL_JOYAXISMOTION){
					if((e.jaxis.value>joydeadzone)||(e.jaxis.value<-joydeadzone)){
						switch(e.jaxis.axis){
							case 0:
								break;
							case 1:
								break;
						}
					}
				}else if(e.type==SDL_QUIT){
					QUIT=1;
				}else if(e.type==SDL_KEYDOWN){
					switch(e.key.keysym.sym){
						case mykeys_up ://up
							menu_pointer(-1);
							break;
						case mykeys_down: //down
							menu_pointer(1);
							break;
						case mykeys_left: //left
							menu_pointer(-10);
							break;
						case mykeys_right: //right
							menu_pointer(10);
							break;
						case mykeys_a: //a
							menu_action(MENU_ACTION_ENTER);
							break;
						case mykeys_b: //b
							menu_action(MENU_ACTION_RETURN);
							break;
						case mykeys_x: //x
							break;
						case mykeys_y: //y
							break;
						case mykeys_l: //l
							menu_pointer(-50);
							break;
						case mykeys_r: //r
							menu_pointer(50);
							break;
						case mykeys_select: //sel
							QUIT=1;
							break;
						case mykeys_start: //start
							break;
						case mykeys_hold: //hold
							break;
						case mykeys_power: //power
							break;
					}
				}else if(e.type==SDL_KEYUP){
					switch(e.key.keysym.sym){
						case mykeys_up://up
							break;
						case mykeys_down://down
							break;
						case mykeys_left://left
							break;
						case mykeys_right://right
							break;
						case mykeys_a://a
							break;
						case mykeys_b://b
							break;
						case mykeys_x://x
							break;
						case mykeys_y://y
							break;
						case mykeys_l://l
							break;
						case mykeys_r://r
							break;
						case mykeys_select://sel
							break;
						case mykeys_start://start
							break;
						case mykeys_hold://hold
							break;
						case mykeys_power://power
							break;
					}
				}
				break;
			case DIR_MENU:
				if(e.type==SDL_JOYAXISMOTION){
					if((e.jaxis.value>joydeadzone)||(e.jaxis.value<-joydeadzone)){
						switch(e.jaxis.axis){
							case 0:
								break;
							case 1:
								break;
						}
					}
				}else if(e.type==SDL_QUIT){
					QUIT=1;
				}else if(e.type==SDL_KEYDOWN){
					switch(e.key.keysym.sym){
						case mykeys_up://up
							menu_pointer(-1);
							break;
						case mykeys_down://down
							menu_pointer(1);
							break;
						case mykeys_left://left
							menu_pointer(-10);
							break;
						case mykeys_right://right
							menu_pointer(10);
							break;
						case mykeys_a://a
							menu_action(MENU_ACTION_ENTER);
							break;
						case mykeys_b://b
							menu_action(MENU_ACTION_RETURN);
							break;
						case mykeys_x://x
							break;
						case mykeys_y://y
							break;
						case mykeys_l://l
							menu_pointer(-maxlines());
							break;
						case mykeys_r://r
							menu_pointer(maxlines());
							break;
						case mykeys_select://sel
							menu_action(MENU_ACTION_ESCAPE);
							break;
						case mykeys_start://start
							break;
						case mykeys_hold://hold
							break;
						case mykeys_power://power
							break;
					}
				}else if(e.type==SDL_KEYUP){
					switch(e.key.keysym.sym){
						case mykeys_up://up
							break;
						case mykeys_down://down
							break;
						case mykeys_left://left
							break;
						case mykeys_right://right
							break;
						case mykeys_a://a
							break;
						case mykeys_b://b
							break;
						case mykeys_x://x
							break;
						case mykeys_y://y
							break;
						case mykeys_l://l
							break;
						case mykeys_r://r
							break;
						case mykeys_select://sel
							break;
						case mykeys_start://start
							break;
						case mykeys_hold://hold
							break;
						case mykeys_power://power
							break;
					}
				}
				break;
			case MENU_NONE:
				if(e.type==SDL_JOYAXISMOTION){
					if((e.jaxis.value>joydeadzone)||(e.jaxis.value<-joydeadzone)){
						switch(e.jaxis.axis){
							case 0:
								break;
							case 1:
								break;
						}
					}
				}else if(e.type==SDL_QUIT){
					QUIT=1;
				}else if(e.type==SDL_KEYDOWN){
					switch(e.key.keysym.sym){
						case mykeys_up://up
							//game_moveblock(0,0,-1);
							break;
						case mykeys_down://down
							//game_moveblock(0,0,1);
							break;
						case mykeys_left://left
							game_moveblock(0,-1,0);
							break;
						case mykeys_right://right
							game_moveblock(0,1,0);
							break;
						case mykeys_a://a
							block_swapcolours(0,1,0);
							break;
						case mykeys_b://b
							block_swapcolours(0,-1,0);
							break;
						case mykeys_x://x
							//block_swapcolours(0,0,-1);
							break;
						case mykeys_y://y
							//block_swapcolours(0,0,1);
							//game_updateblockposition();
							break;
						case mykeys_l://l
							releaseblock();
							//spawnblock();
							break;
						case mykeys_r://r
							//block_delete(game_getnumblocks()-1);
							break;
						case mykeys_select://sel
							QUIT=1;
							break;
						case mykeys_start://start
							break;
						case mykeys_hold://hold
							break;
						case mykeys_power://power
							break;
					}
				}else if(e.type==SDL_KEYUP){
					switch(e.key.keysym.sym){
						case mykeys_up://up
							break;
						case mykeys_down://down
							break;
						case mykeys_left://left
							break;
						case mykeys_right://right
							break;
						case mykeys_a://a
							break;
						case mykeys_b://b
							break;
						case mykeys_x://x
							break;
						case mykeys_y://y
							break;
						case mykeys_l://l
							break;
						case mykeys_r://r
							break;
						case mykeys_select://sel
							break;
						case mykeys_start://start
							break;
						case mykeys_hold://hold
							break;
						case mykeys_power://power
							break;
					}
				}
			break;
			default:
				if(e.type==SDL_JOYAXISMOTION){
					if((e.jaxis.value>joydeadzone)||(e.jaxis.value<-joydeadzone)){
						switch(e.jaxis.axis){
							case 0:
								break;
							case 1:
								break;
						}
					}
				}else if(e.type==SDL_QUIT){
					QUIT=1;
				}else if(e.type==SDL_KEYDOWN){
					switch(e.key.keysym.sym){
						case mykeys_up://up
							break;
						case mykeys_down://down
							break;
						case mykeys_left://left
							break;
						case mykeys_right://right
							break;
						case mykeys_a://a
							break;
						case mykeys_b://b
							break;
						case mykeys_x://x
							break;
						case mykeys_y://y
							break;
						case mykeys_l://l
							break;
						case mykeys_r://r
							break;
						case mykeys_select://sel
							break;
						case mykeys_start://start
							break;
						case mykeys_hold://hold
							break;
						case mykeys_power://power
							break;
					}
				}else if(e.type==SDL_KEYUP){
					switch(e.key.keysym.sym){
						case mykeys_up://up
							break;
						case mykeys_down://down
							break;
						case mykeys_left://left
							break;
						case mykeys_right://right
							break;
						case mykeys_a://a
							break;
						case mykeys_b://b
							break;
						case mykeys_x://x
							break;
						case mykeys_y://y
							break;
						case mykeys_l://l
							break;
						case mykeys_r://r
							break;
						case mykeys_select://sel
							break;
						case mykeys_start://start
							break;
						case mykeys_hold://hold
							break;
						case mykeys_power://power
							break;
					}
				}
			break;
		}
	}
}
