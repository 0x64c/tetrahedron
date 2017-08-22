#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include "menu.h"
#include "game.h"
#include <stdio.h>
#ifndef MYDEF
#include "mytypes.h"
#endif
#include <dirent.h>
#include <stdlib.h>
#include "input.h"

#ifndef _WIN_
#include <linux/limits.h>
#else
#include <limits.h>
//#define PATH_MAX 259
#define NAME_MAX 255
#endif

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Renderer *renderer2 = NULL;
SDL_Surface *surface = NULL;
//char *fontfile;
TTF_Font *font = NULL;
/*#ifdef _GCW_
int fontsize=12;
#else
int fontsize=32;
#endif*/
int fontsize;
int SOMETHING_HAPPENED=1;
int menusize;

struct SDL_Point block_points[][6]={
	{{1,0},{6,0},{5,1},{2,1},{4,2},{4,3}},
	{{1,7},{6,7},{5,6},{2,6},{3,5},{4,5}},
	{{0,1},{0,6},{1,5},{1,2},{2,4},{3,4}},
	{{7,1},{7,6},{6,5},{6,2},{5,3},{5,4}},
	{{0,0},{7,7},{0,0},{0,0},{0,0},{0,0}},
	{{0,7},{7,0},{0,0},{0,0},{0,0},{0,0}}
};

struct dim{
	int width;
	int height;
}dim;

struct colours{
	SDL_Colour empty;
	SDL_Colour black;
	SDL_Colour white;
	SDL_Colour blue;
	SDL_Colour red;
	SDL_Colour green;
	SDL_Colour yellow;
	SDL_Colour magenta;
	SDL_Colour cyan;
	SDL_Colour orange;
	SDL_Colour salmon;
}colours;

struct colours my_colours={
	.empty={0x00,0x00,0x00,0x00},
	.black={0x00,0x00,0x00,0xff},
	.white={0xff,0xff,0xff,0xff},
	.red={0xff,0x00,0x00,0xff},
	.green={0x00,0xaa,0x00,0xff},
	.blue={0x00,0x00,0xff,0xff},
	.yellow={0xff,0xff,0x00,0xff},
	.magenta={0xaa,0x00,0xff,0xff},
	.cyan={0x55,0xaa,0xff,0xff},
	.orange={0xaa,0x55,0x00,0xff},
	.salmon={0xaa,0xaa,0xaa,0xff}
	/*.red={0xff,0x00,0x00,0xff},
	.green={0x00,0xff,0x00,0xff},
	.blue={0x00,0x00,0xff,0xff},
	.yellow={0xff,0xff,0x00,0xff},
	.magenta={0xff,0x00,0xff,0xff},
	.cyan={0x00,0xff,0xff,0xff},
	.orange={0xff,0x99,0x66,0xff},
	.salmon={0x99,0x00,0xcc,0xff}*/
};

SDL_Texture *tex_menu;
SDL_Texture **tex_menutext=NULL;
SDL_Rect rect_menu;
SDL_Rect **rect_menutext=NULL;

SDL_Texture *gamebg_tex;
SDL_Rect *gamebg_rect;

SDL_Texture *tex_score;
SDL_Rect *rect_score;

SDL_Texture *gamept_tex;
SDL_Rect *gamept_rect;

int maxlines(){
	return dim.height/fontsize;
}

SDL_Rect draw_gamebg(){
	SDL_Rect rect={0,0,dim.width,dim.height};
	SDL_SetRenderDrawColor(renderer2,
		my_colours.empty.r,my_colours.empty.g,my_colours.empty.b,my_colours.empty.a);
	SDL_RenderClear(renderer2);
	SDL_SetRenderDrawColor(renderer2,
		my_colours.black.r,my_colours.black.g,my_colours.black.b,my_colours.black.a);
	SDL_RenderFillRect(renderer2,&rect);
	SDL_SetRenderDrawColor(renderer2,
		my_colours.white.r,my_colours.white.g,my_colours.white.b,my_colours.white.a);
	
	int x,y,x1,y1;
	getfinexy(0,0,&x,&y);
	getfinexy(x_max+1,y_max+1,&x1,&y1);
	/*SDL_Point points[5]={{x,y},{x,y1},{x1,y1},{x1,y},{x,y}};
	SDL_RenderDrawLines(renderer2,points,5);*/
	SDL_Rect b_rect0={x-blockspacing/4,y-blockspacing/4,blockspacing/4,y1-y+blockspacing/2};
	SDL_Rect b_rect1={x1,y-blockspacing/4,blockspacing/4,y1-y+blockspacing/2};
	SDL_Rect b_rect2={x-blockspacing/4,y-blockspacing/4,x1-x+blockspacing/2,blockspacing/4};
	SDL_Rect b_rect3={x-blockspacing/4,y1,x1-x+blockspacing/2,blockspacing/4};
	SDL_RenderFillRect(renderer2,&b_rect0);
	SDL_RenderFillRect(renderer2,&b_rect1);
	SDL_RenderFillRect(renderer2,&b_rect2);
	SDL_RenderFillRect(renderer2,&b_rect3);
	
	return rect;
}

SDL_Rect draw_gamepointer(){
	SDL_Rect rect={0,0,blockspacing,blockspacing*(y_max+1)};
	SDL_SetRenderDrawColor(renderer2,
		my_colours.empty.r,my_colours.empty.g,my_colours.empty.b,my_colours.empty.a);
	SDL_RenderClear(renderer2);
	SDL_SetRenderDrawColor(renderer2,
		my_colours.white.r,my_colours.white.g,my_colours.white.b,my_colours.white.a);
	SDL_RenderDrawRect(renderer2,&rect);
	return rect;
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

SDL_Rect draw_block(){
	SDL_Rect rect={0,0,8,8};
	SDL_SetRenderDrawColor(renderer2,
		my_colours.empty.r,my_colours.empty.g,my_colours.empty.b,my_colours.empty.a);
	SDL_RenderClear(renderer2);
	int i;
	for(i=0;i<4;i++){
		SDL_Colour *colour;
		switch(block_getcolour(0,i)){
			case 0:colour=&my_colours.red;break;
			case 1:colour=&my_colours.green;break;
			case 2:colour=&my_colours.blue;break;
			case 3:colour=&my_colours.yellow;break;
			case 4:colour=&my_colours.magenta;break;
			case 5:colour=&my_colours.cyan;break;
			case 6:colour=&my_colours.orange;break;
			case 7:colour=&my_colours.salmon;break;
			default:colour=&my_colours.white;break;
		}
		//printf("got colour %d\n",block_getcolour(0,i));
		SDL_SetRenderDrawColor(renderer2,colour->r,colour->g,colour->b,colour->a);
		SDL_RenderDrawLines(renderer2,block_points[i],6);
	}
	SDL_SetRenderDrawColor(renderer2,
		my_colours.black.r,my_colours.black.g,my_colours.black.b,my_colours.black.a);
	SDL_RenderDrawLines(renderer2,block_points[4],2);
	SDL_RenderDrawLines(renderer2,block_points[5],2);
	//SDL_RenderFillRect(renderer2,&rect);
	//printf("rect points %d %d %d %d\n",rect.x,rect.y,rect.w,rect.h);
	return rect;
}

SDL_Texture* draw_sprite(SDL_Rect* rect2,SDL_Rect (*draw)()){
	SDL_Rect rect = draw();
	SDL_Surface *dummy_surface = SDL_CreateRGBSurface(0,rect.w,rect.h,32,0xFF000000,0x00FF0000,0x0000FF00,0x000000FF);
	//SDL_Surface *dummy_surface = SDL_CreateRGBSurfaceWithFormat(0,rect.w,rect.h,16,SDL_PIXELFORMAT_RGBA4444);
	SDL_BlitSurface(surface,&rect,dummy_surface,NULL);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer,dummy_surface);//
	SDL_FreeSurface(dummy_surface);
	rect2->x=rect.x;
	rect2->y=rect.y;
	rect2->w=rect.w;
	rect2->h=rect.h;
	return tex;
}

void draw_block_(SDL_Rect **rect,SDL_Texture **tex){
	(*tex)=draw_sprite((*rect),draw_block);
}

void drawmenu(){
	if(!menu_state)return;
	SDL_RenderCopy(renderer,tex_menu,NULL,&rect_menu);//
	int i;
	switch(menu_state){
		case MAIN_MENU:
		case DIR_MENU:
			for(i=0;i<menusize;i++){
				SDL_RenderCopy(renderer,tex_menutext[i],NULL,rect_menutext[i]);
			}
			break;
		default:break;
	}
}
void del_menu(){
	int i;
	for(i=0;i<menusize;i++){
		SDL_DestroyTexture(tex_menutext[i]);
		free(rect_menutext[i]);
	}
	SDL_DestroyTexture(tex_menu);
}

void updatemenu(int line, MENU_CATEGORY category){
	int textw=0,texth=0;
	SDL_Surface *messagebox = NULL;
	char cursor;
	char buffer[NAME_MAX];
	if(line==menu_position)cursor='*';
	else cursor=' ';
	sprintf(buffer,"%c%s",cursor,menu_getline(line+maxlines()*menuline_offset,category));
	messagebox = TTF_RenderText_Solid(font,buffer,my_colours.white);

	SDL_Texture *message = SDL_CreateTextureFromSurface(renderer,messagebox);//
	SDL_QueryTexture(message,NULL,NULL,&textw,&texth);

	rect_menutext[line]->x=0;
	rect_menutext[line]->y=0+line*fontsize;
	rect_menutext[line]->w=textw;
	rect_menutext[line]->h=texth;
	tex_menutext[line]=message;
	SDL_FreeSurface(messagebox);
	SOMETHING_HAPPENED=1;
}

void init_menu(){
	tex_menu = draw_sprite(&rect_menu,(*draw_menubg));
	menusize=min(menu_getsize(menu_state)-maxlines()*menuline_offset,maxlines());

	SDL_Texture** rtex=realloc(tex_menutext,menusize*sizeof(SDL_Texture*));
	SDL_Rect** rrect=realloc(rect_menutext,menusize*sizeof(SDL_Rect*));
	if(rtex==NULL)free(rtex);
	else tex_menutext=rtex;
	if(rrect==NULL)free(rrect);
	else rect_menutext=rrect;

	//rect_menutext = malloc(menusize * sizeof(SDL_Rect*));
	//tex_menutext = malloc(menusize * sizeof(SDL_Texture*));
	int i;
	for(i=0;i<menusize;i++){
		rect_menutext[i]=malloc(sizeof(SDL_Rect));
		updatemenu(i,menu_state);
	}
}
void updatescore(){
	int textw=0,texth=0;
	SDL_Surface *messagebox=NULL;
	char buffer[256];
	sprintf(buffer,"Score: %d x%d",score,bonus);
	messagebox=TTF_RenderText_Solid(font,buffer,my_colours.white);
	SDL_Texture *message=SDL_CreateTextureFromSurface(renderer,messagebox);
	SDL_QueryTexture(message,NULL,NULL,&textw,&texth);
	rect_score->x=dim.width/2-textw/2;
	rect_score->y=fontsize/2;
	rect_score->w=textw;
	rect_score->h=texth;
	tex_score=message;
	SDL_FreeSurface(messagebox);
	SOMETHING_HAPPENED=1;
}
void init_game(){
	gamebg_rect=malloc(sizeof(SDL_Rect));
	gamebg_tex=draw_sprite(gamebg_rect,(*draw_gamebg));
	gamept_rect=malloc(sizeof(SDL_Rect));
	gamept_tex=draw_sprite(gamept_rect,(*draw_gamepointer));
	rect_score=malloc(sizeof(SDL_Rect));
	updatescore();
	SOMETHING_HAPPENED=1;
}
void drawgame(){
	//SDL_Rect kek={0,0,32,32};
	int i;float x,y;int xx,yy;
	SDL_RenderCopy(renderer,gamebg_tex,gamebg_rect,NULL);
	block_getxy(0,&x,&y);
	getfinexy(0,0,&xx,&yy);
	gamept_rect->x=x;
	gamept_rect->y=yy;
	SDL_RenderCopy(renderer,gamept_tex,NULL,gamept_rect);
	for(i=game_getnumblocks()-1;i>=0;i--){
		block_getxy(i,&x,&y);
		SDL_Rect kek={(int)x,(int)y,blockspacing,blockspacing};
		SDL_RenderCopy(renderer,block_gettex(i),block_getrect(i),&kek);//block_getrect(i));
		//printf("drawblock %d\n",i);
	}
	SDL_RenderCopy(renderer,tex_score,NULL,rect_score);
	
}
void del_game(){
	SDL_DestroyTexture(gamebg_tex);
	SDL_DestroyTexture(gamept_tex);
	SDL_DestroyTexture(tex_score);
	free(gamebg_rect);
	free(gamept_rect);
	free(rect_score);
}

void gfx_update(){
	SOMETHING_HAPPENED=1;
}

/*void redraw_block(int i){
	SDL_DestroyTexture(block_gettex(i));
	free(block_getrect(i));
	draw_block_((SDL_Rect**)block_getrect(i),(SDL_Texture**)block_gettex(i));
}*/

void getdim(int* x,int *y){
	*x=dim.width;
	*y=dim.height;
}

void gfx_init(){
	char *default_font[]={
		"/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
		"/usr/share/fonts/TTF/DejaVuSansMono.ttf",
		"DejaVuSansMono.ttf",
		"C:/Windows/Fonts/DejaVuSansMono.ttf",
		NULL};
	//fontfile=(char*)malloc(sizeof(char));
#ifdef _GCW_
	dim.width=320;
	dim.height=240;
	//char default_font[]="/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf";
#elif defined(_WIN_)
	dim.width=960;
	dim.height=720;
	//char default_font[]="C:/Windows/Fonts/DejaVuSansMono.ttf";
#else
	dim.width=960;
	dim.height=720;
	//char default_font[]="/usr/share/fonts/TTF/DejaVuSansMono.ttf";
#endif
	//fontfile=realloc(fontfile,strlen(default_font)*sizeof(char));
	//fontfile=malloc((strlen(default_font)+1)*sizeof(char));
	//strcpy(fontfile,default_font);

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("tetrahedron",SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,dim.width,dim.height,SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|
		SDL_RENDERER_TARGETTEXTURE);

	surface = SDL_CreateRGBSurface(0,dim.width,dim.height,32,0xFF000000,0x00FF0000,0x0000FF00,0x000000FF);
	//surface = SDL_CreateRGBSurfaceWithFormat(0,dim.width,dim.height,16,SDL_PIXELFORMAT_RGBA4444);
	renderer2 = SDL_CreateSoftwareRenderer(surface);

	TTF_Init();
	//font = TTF_OpenFont(fontfile,fontsize);
	fontsize=dim.height/20;
	int i;
	for(i=0;default_font[i]!=NULL&&font==NULL;i++)
		font = TTF_OpenFont(default_font[i],fontsize);
	if(font==NULL){
		QUIT=1;
		printf("Error: Font file not found:\n");
		for(i=0;default_font[i]!=NULL;i++)printf("%s\n",default_font[i]);
		printf("Quitting...\n");
		return;
	}
	blockspacing=dim.height/15;
	init_menu();
}

void gfx_done(){
	if(menu_state>1)del_menu();
	if(game_state>1)del_game();
	free(rect_menutext);
	free(tex_menutext);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyRenderer(renderer2);
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	//free(fontfile);
	TTF_Quit();
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void gfx_do(){
	if(SOMETHING_HAPPENED){
		if(menu_state>1)drawmenu();
		if(game_state>1)drawgame();
		SDL_RenderPresent(renderer);//
		SDL_UpdateWindowSurface(window);
		SOMETHING_HAPPENED=0;
	}
}
