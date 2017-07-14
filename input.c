#include <SDL2/SDL.h>
#include "menu.h"
#include "gfx.h"
#include "game.h"
#ifndef MYDEF
#include "mytypes.h"
#endif
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
						case SDLK_UP://up
							menu_pointer(-1);
							break;
						case SDLK_DOWN://down
							menu_pointer(1);
							break;
						case SDLK_LEFT://left
							menu_pointer(-10);
							break;
						case SDLK_RIGHT://right
							menu_pointer(10);
							break;
						case SDLK_LCTRL://a
							menu_action(MENU_ACTION_ENTER);
							break;
						case SDLK_LALT://b
							menu_action(MENU_ACTION_RETURN);
							break;
						case SDLK_LSHIFT://x
							break;
						case SDLK_SPACE://y
							break;
						case SDLK_TAB://l
							menu_pointer(-50);
							break;
						case SDLK_BACKSPACE://r
							menu_pointer(50);
							break;
						case SDLK_ESCAPE://sel
							QUIT=1;
							break;
						case SDLK_RETURN://start
							break;
						case SDLK_PAUSE://hold
							break;
						case SDLK_HOME://power
							break;
					}
				}else if(e.type==SDL_KEYUP){
					switch(e.key.keysym.sym){
						case SDLK_UP://up
							break;
						case SDLK_DOWN://down
							break;
						case SDLK_LEFT://left
							break;
						case SDLK_RIGHT://right
							break;
						case SDLK_LCTRL://a
							break;
						case SDLK_LALT://b
							break;
						case SDLK_LSHIFT://x
							break;
						case SDLK_SPACE://y
							break;
						case SDLK_TAB://l
							break;
						case SDLK_BACKSPACE://r
							break;
						case SDLK_ESCAPE://sel
							break;
						case SDLK_RETURN://start
							break;
						case SDLK_PAUSE://hold
							break;
						case SDLK_HOME://power
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
						case SDLK_UP://up
							menu_pointer(-1);
							break;
						case SDLK_DOWN://down
							menu_pointer(1);
							break;
						case SDLK_LEFT://left
							menu_pointer(-10);
							break;
						case SDLK_RIGHT://right
							menu_pointer(10);
							break;
						case SDLK_LCTRL://a
							menu_action(MENU_ACTION_ENTER);
							break;
						case SDLK_LALT://b
							menu_action(MENU_ACTION_RETURN);
							break;
						case SDLK_LSHIFT://x
							break;
						case SDLK_SPACE://y
							break;
						case SDLK_TAB://l
							menu_pointer(-maxlines());
							break;
						case SDLK_BACKSPACE://r
							menu_pointer(maxlines());
							break;
						case SDLK_ESCAPE://sel
							menu_action(MENU_ACTION_ESCAPE);
							break;
						case SDLK_RETURN://start
							break;
						case SDLK_PAUSE://hold
							break;
						case SDLK_HOME://power
							break;
					}
				}else if(e.type==SDL_KEYUP){
					switch(e.key.keysym.sym){
						case SDLK_UP://up
							break;
						case SDLK_DOWN://down
							break;
						case SDLK_LEFT://left
							break;
						case SDLK_RIGHT://right
							break;
						case SDLK_LCTRL://a
							break;
						case SDLK_LALT://b
							break;
						case SDLK_LSHIFT://x
							break;
						case SDLK_SPACE://y
							break;
						case SDLK_TAB://l
							break;
						case SDLK_BACKSPACE://r
							break;
						case SDLK_ESCAPE://sel
							break;
						case SDLK_RETURN://start
							break;
						case SDLK_PAUSE://hold
							break;
						case SDLK_HOME://power
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
						case SDLK_UP://up
							break;
						case SDLK_DOWN://down
							break;
						case SDLK_LEFT://left
							break;
						case SDLK_RIGHT://right
							break;
						case SDLK_LCTRL://a
							spawnblock();
							break;
						case SDLK_LALT://b
							break;
						case SDLK_LSHIFT://x
							break;
						case SDLK_SPACE://y
							break;
						case SDLK_TAB://l
							break;
						case SDLK_BACKSPACE://r
							break;
						case SDLK_ESCAPE://sel
							QUIT=1;
							break;
						case SDLK_RETURN://start
							break;
						case SDLK_PAUSE://hold
							break;
						case SDLK_HOME://power
							break;
					}
				}else if(e.type==SDL_KEYUP){
					switch(e.key.keysym.sym){
						case SDLK_UP://up
							break;
						case SDLK_DOWN://down
							break;
						case SDLK_LEFT://left
							break;
						case SDLK_RIGHT://right
							break;
						case SDLK_LCTRL://a
							break;
						case SDLK_LALT://b
							break;
						case SDLK_LSHIFT://x
							break;
						case SDLK_SPACE://y
							break;
						case SDLK_TAB://l
							break;
						case SDLK_BACKSPACE://r
							break;
						case SDLK_ESCAPE://sel
							break;
						case SDLK_RETURN://start
							break;
						case SDLK_PAUSE://hold
							break;
						case SDLK_HOME://power
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
						case SDLK_UP://up
							break;
						case SDLK_DOWN://down
							break;
						case SDLK_LEFT://left
							break;
						case SDLK_RIGHT://right
							break;
						case SDLK_LCTRL://a
							break;
						case SDLK_LALT://b
							break;
						case SDLK_LSHIFT://x
							break;
						case SDLK_SPACE://y
							break;
						case SDLK_TAB://l
							break;
						case SDLK_BACKSPACE://r
							break;
						case SDLK_ESCAPE://sel
							QUIT=1;
							break;
						case SDLK_RETURN://start
							break;
						case SDLK_PAUSE://hold
							break;
						case SDLK_HOME://power
							break;
					}
				}else if(e.type==SDL_KEYUP){
					switch(e.key.keysym.sym){
						case SDLK_UP://up
							break;
						case SDLK_DOWN://down
							break;
						case SDLK_LEFT://left
							break;
						case SDLK_RIGHT://right
							break;
						case SDLK_LCTRL://a
							break;
						case SDLK_LALT://b
							break;
						case SDLK_LSHIFT://x
							break;
						case SDLK_SPACE://y
							break;
						case SDLK_TAB://l
							break;
						case SDLK_BACKSPACE://r
							break;
						case SDLK_ESCAPE://sel
							break;
						case SDLK_RETURN://start
							break;
						case SDLK_PAUSE://hold
							break;
						case SDLK_HOME://power
							break;
					}
				}
			break;
		}
	}
}
