#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include "menu.h"
#include <stdio.h>
#ifndef MYDEF
#include "mytypes.h"
#endif

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Renderer *renderer2 = NULL;
SDL_Surface *surface = NULL;
char *fontfile;
TTF_Font *font;
int fontsize=12;

struct dim{
	int width;
	int height;
}dim;

struct colours{
	SDL_Colour empty;
	SDL_Colour black;
	SDL_Colour white;
}colours;

struct colours my_colours={
	.empty={0x00,0x00,0x00,0x00},
	.black={0x00,0x00,0x00,0xff},
	.white={0xff,0xff,0xff,0xff}
};

SDL_Texture *tex_menu;
SDL_Texture **tex_menutext=NULL;
SDL_Rect rect_menu;
SDL_Rect **rect_menutext=NULL;

int maxlines(){
	return dim.height/fontsize;
}

SDL_Rect draw_menubg(){
	SDL_Rect rect={0,0,dim.width,dim.height};
	SDL_SetRenderDrawColor(renderer2,
		my_colours.empty.r,my_colours.empty.g,my_colours.empty.b,my_colours.empty.a);
	SDL_RenderClear(renderer2);
	SDL_SetRenderDrawColor(renderer2,
		my_colours.black.r,my_colours.black.g,my_colours.black.b,my_colours.black.a);
	SDL_RenderFillRect(renderer2,&rect);
	return rect;
}

SDL_Texture* draw_sprite(SDL_Rect* rect2,SDL_Rect (*draw)()){
	SDL_Rect rect = draw();
	SDL_Surface *dummy_surface = SDL_CreateRGBSurface(0,dim.width,dim.height,32,
		0xFF000000,0x00FF0000,0x0000FF00,0x000000FF);
	SDL_BlitSurface(surface,&rect,dummy_surface,NULL);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer,dummy_surface);
	SDL_FreeSurface(dummy_surface);
	rect2->x=rect.x;
	rect2->y=rect.y;
	rect2->w=rect.w;
	rect2->h=rect.h;
	return tex;
}

void drawmenu(){
	if(!menu_state)return;
	SDL_RenderCopy(renderer,tex_menu,NULL,&rect_menu);
	switch(menu_state){
		case MAIN_MENU:
		case DIR_MENU:
			for(int i=0;i<min(menu_getsize(menu_state)-maxlines()*menuline_offset,maxlines());i++){
				SDL_RenderCopy(renderer,tex_menutext[i],NULL,rect_menutext[i]);
			}
			break;
		default:break;
	}
}
void del_menu(){
	for(int i=0;i<min(menu_getsize(menu_state)-maxlines()*menuline_offset,maxlines());i++){
		SDL_DestroyTexture(tex_menutext[i]);
		free(rect_menutext[i]);
	}
}
void updatemenu(int line, MENU_CATEGORY category){
	int textw=0,texth=0;
	SDL_Surface *messagebox = NULL;
	char cursor;
	char buffer[20];
	if(line==menu_position)cursor='*';
	else cursor=' ';
	sprintf(buffer,"%c%s",cursor,menu_getline(line+maxlines()*menuline_offset,category));
	messagebox = TTF_RenderText_Solid(font,buffer,my_colours.white);

	SDL_Texture *message = SDL_CreateTextureFromSurface(renderer,messagebox);
	SDL_QueryTexture(message,NULL,NULL,&textw,&texth);

	rect_menutext[line]->x=0;
	rect_menutext[line]->y=0+line*fontsize;
	rect_menutext[line]->w=textw;
	rect_menutext[line]->h=texth;
	tex_menutext[line]=message;
	SDL_FreeSurface(messagebox);
}
void init_menu(){
	int menusize=min(menu_getsize(menu_state)-maxlines()*menuline_offset,maxlines());
	rect_menutext = malloc(menusize * sizeof(SDL_Rect*));
	tex_menutext = malloc(menusize * sizeof(SDL_Texture*));
	for(int i=0;i<menusize;i++){
		rect_menutext[i]=malloc(sizeof(SDL_Rect));
		updatemenu(i,menu_state);
	}
}

void gfx_init(){
	//fontfile=(char*)malloc(sizeof(char));
#ifdef _GCW_
	dim.width=320;
	dim.height=240;
	char default_font[]="/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf";
#else
	dim.width=800;
	dim.height=600;
	char default_font[]="/usr/share/fonts/TTF/DejaVuSansMono.ttf";
#endif
	//fontfile=realloc(fontfile,strlen(default_font)*sizeof(char));
	fontfile=malloc((strlen(default_font)+1)*sizeof(char));
	strcpy(fontfile,default_font);
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("tetrahedron",SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,dim.width,dim.height,SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|
		SDL_RENDERER_TARGETTEXTURE);

	surface = SDL_CreateRGBSurface(0,dim.width,dim.height,32,
		0xFF000000,0x00FF0000,0x0000FF00,0x000000FF);
	renderer2 = SDL_CreateSoftwareRenderer(surface);

	TTF_Init();
	font = TTF_OpenFont(fontfile,fontsize);

	tex_menu = draw_sprite(&rect_menu,(*draw_menubg));
	init_menu();
}

void gfx_done(){
	del_menu();
	SDL_DestroyTexture(tex_menu);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyRenderer(renderer2);
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	free(fontfile);
	TTF_Quit();
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void gfx_do(){
	drawmenu();
	SDL_RenderPresent(renderer);
	SDL_UpdateWindowSurface(window);
}
