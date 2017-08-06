#include "mytypes.h"
#include <SDL2/SDL.h>
#include <time.h>
#include "gfx.h"
#include <stdio.h>
int score;
int speed;

typedef struct gameblock{
	unsigned int colours;
	int x;
	int y;
	int spin;
	int moving;
	SDL_Rect *rect;
	SDL_Texture *tex;
} gameblock;

gameblock **allblocks;
int numblocks=0;
int maxblocks=0;
GAME_STATE game_state=GAME_NONE;
struct timespec tspec;

int gameblock_mask[]={0xf,0xf0,0xf00,0xf000};
unsigned int shittyrandom(){
//	srand(time(0));
	clock_gettime(CLOCK_REALTIME,&tspec);
	srand(tspec.tv_nsec);
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

void block_getxy(int i,int* x,int* y){
	*x=allblocks[i]->x;
	*y=allblocks[i]->y;
}

void game_moveblock(int x,int y){
	//todo: hardcode min/max
	int xmax=800-32;xmax-=xmax%32;
	int ymax=600-32;ymax-=ymax%32;
	allblocks[0]->x+=x*32;
	allblocks[0]->y+=y*32;
	if(allblocks[0]->x<0)allblocks[0]->x=0;
	else if(allblocks[0]->x>xmax)allblocks[0]->x=xmax;
	if(allblocks[0]->y<0)allblocks[0]->y=0;
	else if(allblocks[0]->y>ymax)allblocks[0]->y=ymax;
	gfx_update();
}
void swapblock(gameblock **a,gameblock **b){
	gameblock *temp=*a;
	*a=*b;
	*b=temp;
}
void spawnblock(){
	numblocks++;
        gameblock **result=realloc(allblocks,numblocks*sizeof(gameblock*));
        if(result==NULL)free(result);
        else allblocks=result;
	if(maxblocks==0){
		allblocks[0]=malloc(sizeof(gameblock));
	}
	else if(numblocks>maxblocks){
		allblocks[numblocks-1]=malloc(sizeof(gameblock));
		swapblock(&allblocks[0],&allblocks[numblocks-1]);
	}
	printf("numblock,maxblock: %d,%d\n",numblocks,maxblocks);
	maxblocks++;
	allblocks[0]->colours=shittyrandom();
	allblocks[0]->x=0;
	allblocks[0]->y=0;
	allblocks[0]->spin=0;
	allblocks[0]->moving=1;
	allblocks[0]->rect=malloc(sizeof(SDL_Rect));
	draw_block_(&allblocks[0]->rect,&allblocks[0]->tex);
	gfx_update();
}
void game_init(){
	score=0;
	speed=0;
	spawnblock();
}
void game_done(){
	int i;
	for(i=0;i<numblocks;i++)free(allblocks[i]);
	free(allblocks);
}
void game_do(){
}
