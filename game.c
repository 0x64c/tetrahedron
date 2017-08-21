#include "mytypes.h"
#include <SDL2/SDL.h>
#include <time.h>
#include "gfx.h"
#include <stdio.h>
#include "input.h"
#include <math.h>
#include "sound.h"
#include <stdlib.h>

int score;
int speed;
int gameover;
int bonus;
unsigned int gametimer,movementtime,bonustime;

typedef struct gameblock{
	unsigned int colours;
	int x;
	int y;
	float finex;
	float finey;
	float xv;
	float yv;
	int spin;
	int nframesstationary;
	//int moving;
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
	sound_play(1);
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
#ifndef _WIN_
	clock_gettime(CLOCK_REALTIME,&tspec);
	srand(tspec.tv_nsec);
#else

#endif
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

void block_getxy(int i,float* x,float* y){
	*x=allblocks[i]->finex;
	*y=allblocks[i]->finey;
}

int game_checkspot(int i,int xoff, int yoff){
	int j;
	for(j=0;j<numblocks;j++){
		if(j==i)continue;
		//printf("%d %d  %d %d\n",allblocks[i]->x,allblocks[j]->x,allblocks[i]->y,allblocks[j]->y);
		if(abs(allblocks[i]->x+xoff-allblocks[j]->x)<1&&abs(allblocks[i]->y+yoff-allblocks[j]->y)<1){
			//printf("spot check 1\n");
			return j;
		}
	}
	//printf("spot check 0\n");
	return -1;
}

void getfinexy(int x,int y,int* finex, int* finey){
	int dimw,dimh;
	getdim(&dimw,&dimh);
	*finex=dimw/2-blockspacing*x_max/2+x*blockspacing;
	*finey=blockspacing*1.5+y*blockspacing;
}
void getxy(int finex,int finey,int *x,int *y){
	int dimw,dimh;
	getdim(&dimw,&dimh);
	*x=(finex-dimw/2+blockspacing*x_max/2)/blockspacing;
	*y=(finey-blockspacing*1.5)/blockspacing;
}

void spawnblock();
void game_moveblockv(int i){
	if(i==0&&allblocks[i]->nframesstationary>4){spawnblock();return;}
	else if(i==0&&allblocks[i]->nframesstationary==2)sound_play(0);
	//printf("%d\t%d\n",i,allblocks[i]->nframesstationary);
	int tx,ty;
	if(allblocks[i]->xv>0)tx=1;
	else if(allblocks[i]->xv<0)tx=-1;
	else tx=0;
	if(allblocks[i]->yv>0)ty=1;
	else if(allblocks[i]->yv<0)ty=-1;
	else ty=0;
	if(game_checkspot(i,tx,ty)>=0){
		int x2,y2;
		getfinexy(allblocks[i]->x,allblocks[i]->y,&x2,&y2);
		allblocks[i]->finex=x2;
		allblocks[i]->finey=y2;
		allblocks[i]->nframesstationary++;
		return;
	}
	/*int dimw,dimh;
	getdim(&dimw,&dimh);
	int xmax=dimw-blockspacing;xmax-=xmax%blockspacing;xmax=xmax/blockspacing;
	int ymax=dimh-blockspacing;ymax-=ymax%blockspacing;ymax=ymax/blockspacing;*/

	//printf("fx fy x y\t%.6f %.6f %d %d\t",allblocks[i]->finex,allblocks[i]->finey,allblocks[i]->x,allblocks[i]->y);
	allblocks[i]->finex+=allblocks[i]->xv;
	allblocks[i]->finey+=allblocks[i]->yv;

	int x1,y1;
	getxy(allblocks[i]->finex,allblocks[i]->finey,&x1,&y1);

	allblocks[i]->x=x1;
	allblocks[i]->y=y1;

	//printf("%.6f %.6f %d %d\n",allblocks[i]->finex,allblocks[i]->finey,allblocks[i]->x,allblocks[i]->y);

	int tmp=0;
	int x,y,x0,y0;
	getfinexy(x_max,y_max,&x,&y);
	getfinexy(0,0,&x0,&y0);
	if(allblocks[i]->finex>x){
		allblocks[i]->finex=x;
		tmp++;
	}else if(allblocks[i]->finex<x0){
		allblocks[i]->finex=x0;
		tmp++;
	}
	if(allblocks[i]->finey>y){
		//printf("test\n");
		allblocks[i]->finey=y;
		tmp++;
	}else if(allblocks[i]->finey<y0){
		allblocks[i]->finey=y0;
		tmp++;
	}
	if(tmp==0)allblocks[i]->nframesstationary=0;
	else allblocks[i]->nframesstationary++;
	//printf("%d %d %d %d\n",x0,y0,x,y);
	gfx_update();
}

void game_moveblock(int i,int x,int y){
	//todo: hardcode min/max
	if(game_checkspot(i,x,y)>=0){
		allblocks[i]->nframesstationary++;
		//sound_play(0);
		return;
	}
	//allblocks[i]->moving=1;
	/*int dimw,dimh;
	getdim(&dimw,&dimh);
	int xmax=dimw-blockspacing;xmax-=xmax%blockspacing;xmax=xmax/blockspacing;
	int ymax=dimh-blockspacing;ymax-=ymax%blockspacing;ymax=ymax/blockspacing;*/
	int oldx=allblocks[i]->x;
	int oldy=allblocks[i]->y;
	allblocks[i]->x+=x;
	allblocks[i]->y+=y;
/*	int tmp=0;
	if(allblocks[i]->x<0)allblocks[i]->x=0;
	else if(allblocks[i]->x>xmax)allblocks[i]->x=xmax;
	else tmp++;
	if(allblocks[i]->y<0)allblocks[i]->y=0;
	else if(allblocks[i]->y>ymax)allblocks[i]->y=ymax;
	else tmp++;
	if(tmp==2)allblocks[i]->nframesstationary=0;
	else allblocks[i]->nframesstationary++;*/
	int tmp=0;
	if(allblocks[i]->x>x_max){
		allblocks[i]->x=x_max;
		tmp++;
	}else if(allblocks[i]->x<0){
		allblocks[i]->x=0;
		tmp++;
	}
	if(allblocks[i]->y>y_max){
		allblocks[i]->y=y_max;
		tmp++;
	}else if(allblocks[i]->y<0){
		allblocks[i]->y=0;
		tmp++;
	}
	if(tmp==0&&oldx!=allblocks[i]->x&&oldy!=allblocks[i]->y)allblocks[i]->nframesstationary=0;
	else allblocks[i]->nframesstationary++;

	int x_,y_;
	getfinexy(allblocks[i]->x,allblocks[i]->y,&x_,&y_);

	if(abs(x)>0)allblocks[i]->finex=x_;
	if(abs(y)>0)allblocks[i]->finey=y_;

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
		allblocks[0]->yv=blockspacing/6.0f;
		allblocks[numblocks-1]=malloc(sizeof(gameblock));
		swapblock(&allblocks[0],&allblocks[numblocks-1]);
	}else{
		allblocks[0]->yv=blockspacing/6.0f;
		allblocks[numblocks-1]=malloc(sizeof(gameblock));
		swapblock(&allblocks[0],&allblocks[numblocks-1]);
		maxblocks--;
	}
	//printf("numblock,maxblock: %d,%d\n",numblocks,maxblocks);
	maxblocks++;

/*	int dimw,dimh;
	getdim(&dimw,&dimh);
	int xmax=dimw-blockspacing;xmax-=xmax%blockspacing;xmax=xmax/blockspacing;*/
	//int ymax=dimh-blockspacing;ymax-=ymax%blockspacing;ymax=ymax/blockspacing;
	allblocks[0]->colours=shittyrandom();
	allblocks[0]->x=x_max/2;
	allblocks[0]->y=0;

	int x,y;
	getfinexy(allblocks[0]->x,allblocks[0]->y,&x,&y);

	allblocks[0]->finex=x;
	allblocks[0]->finey=y;
	allblocks[0]->xv=0;
	allblocks[0]->yv=blockspacing/12.0f;
	allblocks[0]->spin=0;
	allblocks[0]->nframesstationary=0;
	//allblocks[0]->moving=1;
	allblocks[0]->rect=malloc(sizeof(SDL_Rect));
	draw_block_(&allblocks[0]->rect,&allblocks[0]->tex);
	gfx_update();
	if(game_checkspot(0,0,0)>=0)gameover=1;
}
void game_updateblockposition(){
	int i;
	for(i=0;i<numblocks;i++){
		if((abs(allblocks[i]->xv)>0.001f)||(abs(allblocks[i]->yv)>0.001f))
			game_moveblockv(i);
			//game_moveblock(i,allblocks[i]->xv,allblocks[i]->yv);
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
	clearblocks_rcheck(list,count,start,stationary,nstationary,1,0,2);
	clearblocks_rcheck(list,count,start,stationary,nstationary,-1,0,3);
	clearblocks_rcheck(list,count,start,stationary,nstationary,0,1,1);
	clearblocks_rcheck(list,count,start,stationary,nstationary,0,-1,0);
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
		bonus++;
		bonustime=SDL_GetTicks();
		score+=round(pow((rcount-3)*5,2))*bonus;
		//printf("Score: %d\n",score);
		updatescore();
		sound_play(2);
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
void releaseblock(){
/*	int dimw,dimh;
	getdim(&dimw,&dimh);
	int xmax=dimw-blockspacing;xmax-=xmax%blockspacing;xmax=xmax/blockspacing;
	int ymax=dimh-blockspacing;ymax-=ymax%blockspacing;ymax=ymax/blockspacing;*/
	int i;
	int top_x=y_max+1;
	for(i=1;i<numblocks;i++){
		if(allblocks[i]->x==allblocks[0]->x&&allblocks[i]->y<top_x)top_x=allblocks[i]->y;
	}
	game_moveblock(0,0,top_x-allblocks[0]->y-1);
	//spawnblock();
}
void game_init(){
	sound_init();
	sound_startbgm();
	score=0;
	speed=0;
	gameover=0;
	bonus=0;
	gametimer=SDL_GetTicks();
	movementtime=gametimer;
	spawnblock();
}
void game_done(){
	sound_done();
	int i;
	for(i=0;i<numblocks;i++)free(allblocks[i]);
}
void game_reset(){
	int i;
	for(i=0;i<numblocks;i++)free(allblocks[i]);
	score=0;gameover=0;speed=0;numblocks=0;bonus=0;
	updatescore();
	spawnblock();
	sound_play(1);
}
void game_do(){
	gametimer=SDL_GetTicks();
	if(gametimer-movementtime>30){
		game_updateblockposition();
		game_clearblocks();
		movementtime=gametimer;
		if(gametimer>bonustime+1500){bonus=0;updatescore();}
		if(gameover){
			game_reset();
			//QUIT=1;
		}
	}
}
