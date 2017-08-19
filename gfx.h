#ifndef MYDEF
#include "mytypes.h"
#endif
#include <SDL2/SDL.h>
extern void gfx_init();
extern void gfx_done();
extern void gfx_do();
extern void updatemenu(int,MENU_CATEGORY);
extern void del_menu();
extern void init_menu();
extern int maxlines();
extern void draw_block_(SDL_Rect**,SDL_Texture**);
extern void gfx_update();
extern void init_game();
extern void getdim(int*,int*);
//extern void redraw_block(int);
