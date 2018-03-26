#ifndef MYDEF
#include "mytypes.h"
#endif
#ifndef SDL_h_
#include <SDL2/SDL.h>
#define SDL_h_
#endif
extern void gfx_init(void);
extern void gfx_done(void);
extern void gfx_do(void);
extern void updatemenu(int,MENU_CATEGORY);
extern void del_menu(void);
extern void init_menu(void);
extern int maxlines(void);
extern void draw_block_(SDL_Rect**,SDL_Texture**);
extern void gfx_update(void);
extern void init_game(void);
extern void getdim(int*,int*);
extern void updatescore(void);
Uint8 randcolour(void);
int maxlines(void);
SDL_Rect draw_gamebg(void);
SDL_Rect draw_gamepointer(void);
SDL_Rect draw_menubg(void);
SDL_Rect draw_block(void);
SDL_Texture* draw_sprite(SDL_Rect*,SDL_Rect(*)());
void drawmenu(void);
void drawgame(void);
void del_game(void);

