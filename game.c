#include "mytypes.h"
#include <SDL2/SDL.h>
#include <time.h>
#include "gfx.h"
#include <stdio.h>
int score;
int speed;

typedef struct gameblock{
	unsigned int colours;
	unsigned int x;
	unsigned int y;
	int spin;
	int moving;
	SDL_Rect *rect;
	SDL_Texture *tex;
} gameblock;

//struct gameblock *currentblock;
gameblock **allblocks;
int numblocks=0;
int maxblocks=0;
GAME_STATE game_state=GAME_NONE;

int gameblock_mask[]={0xf,0xf0,0xf00,0xf000};
unsigned int shittyrandom(){
	srand(time(0));
	return (unsigned int)(0x7777&rand());
}
unsigned int block_getcolour(int block, int c){
	return (allblocks[block]->colours&gameblock_mask[c])>>c*4;
}
SDL_Texture *block_gettex(int block){
	return allblocks[block]->tex;
}
SDL_Rect *block_getrect(int block){
	return allblocks[block]->rect;
}
int game_getnumblocks(){
	return numblocks;
}
void spawnblock(){
	numblocks++;
	gameblock *block=malloc(sizeof(gameblock));
	if(maxblocks==0){
		allblocks=malloc(2*sizeof(gameblock*));
	}
	else if(numblocks>maxblocks){
		allblocks=realloc(allblocks,(numblocks+1)*sizeof(gameblock*));
		gameblock *temp=allblocks[0];
		allblocks[numblocks]=temp;
	}
	allblocks[0]=block;
	maxblocks++;
	block->colours=shittyrandom();
	block->x=0;
	block->y=0;
	block->spin=0;
	block->moving=1;
	block->rect=malloc(sizeof(SDL_Rect));
	draw_block_(&block->rect,&block->tex);
	printf("%d %d %d \n",block->rect->x,block->rect->y,block->colours);
}
void game_init(){
	score=0;
	speed=0;
	spawnblock();
}
void game_done(){
	for(int i=0;i<numblocks;i++)free(allblocks[i]);
	free(allblocks);
}
void game_do(){
}
