#include "mytypes.h"
#include <SDL2/SDL.h>
#include <time.h>
#include "gfx.h"
#include <stdio.h>
#include "input.h"
#include <math.h>
int score;
int speed;
int gameover;
unsigned int gametimer,movementtime;

typedef struct gameblock{
	unsigned int colours;
	int x;
	int y;
	int xv;
	int yv;
	int spin;
	int nframesstationary;
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
unsigned int block_getcolour(int block, int c){
	return (allblocks[block]->colours&gameblock_mask[c])>>c*4;
}

int int_cmp(const void* p1, const void* p2)
{ 
   int i1 = *(int*) p1;
   int i2 = *(int*) p2;
   if (i1 < i2) return 1;
   else if (i1 == i2) return 0;
   else return -1;
 }

void block_delete(int i){
	if(i>=numblocks||numblocks==0)return;
	free(allblocks[i]);
	allblocks[i]=allblocks[numblocks-1];
	numblocks--;
	gfx_update();
}

void block_swapcolours(int i,int rot,int ref){
	unsigned int a=block_getcolour(i,3);
	unsigned int b=block_getcolour(i,2);
	unsigned int c=block_getcolour(i,1);
	unsigned int d=block_getcolour(i,0);
	int j;
	if(rot>0)
		for(j=0;j<abs(rot);j++)
			allblocks[i]->colours=(a<<4)^(b)^(c<<2*4)^(d<<3*4);
	else if(rot<0)
		for(j=0;j<abs(rot);j++)
			allblocks[i]->colours=(a)^(b<<4)^(c<<3*4)^(d<<2*4);
	if(ref>0)
		allblocks[i]->colours=(a<<2*4)^(b<<3*4)^(c<<1*4)^(d);
	else if(ref<0)
		allblocks[i]->colours=(a<<3*4)^(b<<2*4)^(c)^(d<<1*4);

	draw_block_(&allblocks[i]->rect,&allblocks[i]->tex);
	gfx_update();
}

int block_checkcolours(int i,int j,int dir){
	int ret=0;
	switch(dir){
		case 0:
			if(block_getcolour(i,0)==block_getcolour(j,1))ret=1;//upper
			break;
		case 1:
			if(block_getcolour(i,1)==block_getcolour(j,0))ret=1;//lower
			break;
		case 2:
			if(block_getcolour(i,3)==block_getcolour(j,2))ret=1;//right
			break;
		case 3:
			if(block_getcolour(i,2)==block_getcolour(j,3))ret=1;//left
			break;
		default:
			break;
	}
	return ret;
}

unsigned int shittyrandom(){
//	srand(time(0));
	clock_gettime(CLOCK_REALTIME,&tspec);
	srand(tspec.tv_nsec);
	return (unsigned int)(0x7777&rand());
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

int game_checkspot(int i,int xoff, int yoff){
	int j;
	for(j=0;j<numblocks;j++){
		if(j==i)continue;
		if(abs(allblocks[i]->x+xoff-allblocks[j]->x)<32&&abs(allblocks[i]->y+yoff-allblocks[j]->y)<32){
			//printf("spot check 1\n");
			return j;
		}
	}
	//printf("spot check 0\n");
	return -1;
}

void game_moveblock(int i,int x,int y){
	//todo: hardcode min/max
	if(game_checkspot(i,x*32,y*32)>=0){
//		allblocks[i]->xv=0;
//		allblocks[i]->yv=0;
		//allblocks[i]->moving=0;
		allblocks[i]->nframesstationary++;
		return;
	}
	//allblocks[i]->moving=1;
	int xmax=800-32;xmax-=xmax%32;
	int ymax=600-32;ymax-=ymax%32;

	allblocks[i]->x+=x*32;
	allblocks[i]->y+=y*32;
	int tmp=0;
	if(allblocks[i]->x<0)allblocks[i]->x=0;
	else if(allblocks[i]->x>xmax)allblocks[i]->x=xmax;
	else tmp++;
	if(allblocks[i]->y<0)allblocks[i]->y=0;
	else if(allblocks[i]->y>ymax)allblocks[i]->y=ymax;
	else tmp++;
	if(tmp==2)allblocks[i]->nframesstationary=0;
	else allblocks[i]->nframesstationary++;
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
	}else{
		allblocks[numblocks-1]=malloc(sizeof(gameblock));
		swapblock(&allblocks[0],&allblocks[numblocks-1]);
		maxblocks--;
	}
	//printf("numblock,maxblock: %d,%d\n",numblocks,maxblocks);
	maxblocks++;
	allblocks[0]->colours=shittyrandom();
	allblocks[0]->x=0;
	allblocks[0]->y=0;
	allblocks[0]->xv=0;
	allblocks[0]->yv=1;
	allblocks[0]->spin=0;
	allblocks[0]->nframesstationary=0;
	allblocks[0]->moving=1;
	allblocks[0]->rect=malloc(sizeof(SDL_Rect));
	draw_block_(&allblocks[0]->rect,&allblocks[0]->tex);
	gfx_update();
	if(game_checkspot(0,0,0)>=0)gameover=1;
}
void game_updateblockposition(){
	int i;
	for(i=0;i<numblocks;i++){
		if((allblocks[i]->xv!=0)||(allblocks[i]->yv!=0))
			game_moveblock(i,allblocks[i]->xv,allblocks[i]->yv);
		//else allblocks[i]->nframesstationary++;
		//printf("stationary %d %d\n",i,allblocks[i]->nframesstationary);
	}
}

int array_checkcontains(int* arr,int max,int val){
	int i;
	for(i=0;i<max;i++){
		if(arr[i]==val)return 1;
	}
	return 0;
}

void clearblocks_rdo(int*,int*,int,int*,int);
int clearblocks_rcheck(int *list,int *count,int start,int *stationary,int nstationary,
		int x,int y,int dir){
	int j;
	if((j=game_checkspot(start,x,y))>=0
			&&array_checkcontains(list,*count,j)==0
			&&array_checkcontains(stationary,nstationary,j)>0
			&&block_checkcolours(start,j,dir)>0
			){
		list[*count]=j;
		(*count)++;
		clearblocks_rdo(list,count,j,stationary,nstationary);
	}else return 0;
	return 1;
}

void clearblocks_rdo(int *list,int *count,int start,int *stationary,
		int nstationary){
	clearblocks_rcheck(list,count,start,stationary,nstationary,32,0,2);
	clearblocks_rcheck(list,count,start,stationary,nstationary,-32,0,3);
	clearblocks_rcheck(list,count,start,stationary,nstationary,0,32,1);
	clearblocks_rcheck(list,count,start,stationary,nstationary,0,-32,0);
	return;
}

void clearblocks_rsetup(int *arr,int *count,int* checked,int *numchecked,int max,int start,
		int *stationary,int nstationary){
	int i;
	int rlist[max];
	int rcount=0;
	rlist[rcount]=start;
	rcount++;
	clearblocks_rdo(rlist,&rcount,start,stationary,nstationary);
	if(rcount>3){
		for(i=0;i<rcount;i++,(*count)++,(*numchecked)++){
			arr[*count]=rlist[i];
			checked[*numchecked]=rlist[i];
		}
		score+=pow((rcount-3)*5,2);
		printf("Score: %d\n",score);
	}else{
		for(i=0;i<rcount;i++,(*numchecked)++)checked[*numchecked]=rlist[i];
	}
}

void game_clearblocks(){
	int i;
	int toclear[numblocks];
	int checked[numblocks];
	int stationary[numblocks];
	int numchecked=0;
	int count=0;
	int numstationary=0;
	for(i=1;i<numblocks;i++){
		if(allblocks[i]->nframesstationary>0){
			stationary[numstationary]=i;
			numstationary++;
		}
	}
	for(i=0;i<numstationary;i++){
		if(array_checkcontains(checked,numchecked,stationary[i])==0){
			clearblocks_rsetup(toclear,&count,checked,&numchecked,numblocks,stationary[i],
				stationary,numstationary);
		}
	}
	qsort(toclear,count,sizeof(int),int_cmp);
	for(i=0;i<count;i++){
		block_delete(toclear[i]);
	}
}

void game_init(){
	score=0;
	speed=0;
	gameover=0;
	gametimer=SDL_GetTicks();
	movementtime=gametimer;
	spawnblock();
}
void game_done(){
	int i;
	for(i=0;i<numblocks;i++)free(allblocks[i]);
	free(allblocks);
}
void game_do(){
	gametimer=SDL_GetTicks();
	if(gametimer-movementtime>1000){
		game_updateblockposition();
		game_clearblocks();
		movementtime=gametimer;
	}
	if(gameover)QUIT=1;
}
