#ifndef SDL_h_
#include <SDL2/SDL.h>
#define SDL_h_
#endif
#include "mytypes.h"
#include "game.h"
#include <time.h>
#include "gfx.h"
#include <stdio.h>
#include "input.h"
#include "sound.h"
#include <stdlib.h>

int score;
int speed;
int gameover;
int bonus;
unsigned int gametimer,movementtime,bonustime;

gameblock **allblocks;
int numblocks=0;
int maxblocks=0;
GAME_STATE game_state=GAME_NONE;
struct timespec tspec;


static uint32_t frand_seedv;
void seed_frand(uint32_t seed){
    frand_seedv=seed;
}
void seed_frand_time(void){
    clock_gettime(CLOCK_MONOTONIC,&tspec);
    seed_frand(tspec.tv_nsec);
}
int frand(void){
    frand_seedv = (214013 * frand_seedv + 2531011);
    return (int)((frand_seedv>>16)&0x7777);
}

unsigned int block_getcolour(int block, int c){
	return (allblocks[block]->colours>>(c*4))&0xf;
}

int int_cmp(const void* p1, const void* p2)
{ 
    int i1 = *(int*) p1;
    int i2 = *(int*) p2;
    return (i1==i2)?0:((i1<i2)?1:-1);
}

void block_delete(int i){
    if(i>=numblocks||numblocks==0)return;
    free(allblocks[i]);
    allblocks[i]=allblocks[numblocks-1];
    numblocks--;
    gfx_update();
}

#define swapc(w,x,y,z) (allblocks[i]->colours=(a<<4*w)^(b<<4*x)^(c<<4*y)^(d<<4*z))
#define refl(x)(x>0)?swapc(2,3,1,0):swapc(3,2,0,1)
#define rota(x)(x>0)?swapc(1,0,2,3):swapc(0,1,3,2)
void block_swapcolours(int i,int rot,int ref){
    unsigned int a=block_getcolour(i,3);
    unsigned int b=block_getcolour(i,2);
    unsigned int c=block_getcolour(i,1);
    unsigned int d=block_getcolour(i,0);
    int j;
    for(j=0;j<abs(rot);j++)rota(rot);
    if(ref!=0)refl(ref);
    draw_block_(&allblocks[i]->rect,&allblocks[i]->tex);
    gfx_update();
    sound_play(1);
}

#define checkc(x,y)block_getcolour(i,x)==block_getcolour(j,y)
int block_checkcolours(int i,int j,int dir){
    return (dir<2)?((dir==0)?checkc(0,1):checkc(1,0)):((dir==2)?checkc(3,2):checkc(2,3));
}

SDL_Texture *block_gettex(int block){
    return allblocks[block]->tex;
}
SDL_Rect *block_getrect(int block){
    return allblocks[block]->rect;
}
int game_getnumblocks(void){
    return numblocks;
}

void block_getxy(int i,float* x,float* y){
    *x=allblocks[i]->finex;
    *y=allblocks[i]->finey;
}

#define spotdiff(d,doff)abs(allblocks[i]->d+doff-allblocks[j]->d)<1
int game_checkspot(int i,int xoff, int yoff){
    int j;
    for(j=0;j<numblocks;j++){
        if(j==i)continue;
        if(spotdiff(x,xoff)&&spotdiff(y,yoff)){
            return j;
        }
    }
    return -1;
}

void getfinexy(int x,int y,int* finex, int* finey){
    int dimw,dimh;
    getdim(&dimw,&dimh);
    *finex=dimw/2-blockspacing*x_max/2+x*blockspacing;
    *finey=blockspacing*1.5f+y*blockspacing;
}
void getxy(int finex,int finey,int *x,int *y){
    int dimw,dimh;
    getdim(&dimw,&dimh);
    *x=(finex-dimw/2+blockspacing*x_max/2)/blockspacing;
    *y=(finey-blockspacing*1.5f)/blockspacing;
}

void spawnblock();
void game_moveblockv(int i){
    if(i==0&&allblocks[i]->nframesstationary>4){spawnblock();sound_play(0);return;}
//    else if(i==0&&allblocks[i]->nframesstationary==2)sound_play(0);
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
    allblocks[i]->finex+=allblocks[i]->xv;
    allblocks[i]->finey+=allblocks[i]->yv;

    int x1,y1;
    getxy(allblocks[i]->finex,allblocks[i]->finey,&x1,&y1);

    allblocks[i]->x=x1;
    allblocks[i]->y=y1;

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
        allblocks[i]->finey=y;
        tmp++;
    }else if(allblocks[i]->finey<y0){
        allblocks[i]->finey=y0;
        tmp++;
    }
    if(tmp==0)allblocks[i]->nframesstationary=0;
    else allblocks[i]->nframesstationary++;
    gfx_update();
}

void game_moveblock(int i,int x,int y){
    //todo: hardcode min/max
    if(game_checkspot(i,x,y)>=0){
        allblocks[i]->nframesstationary++;
        //sound_play(0);
        return;
    }
    int oldx=allblocks[i]->x;
    int oldy=allblocks[i]->y;
    allblocks[i]->x+=x;
    allblocks[i]->y+=y;
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
void spawnblock(void){
    numblocks++;
    gameblock **result=realloc(allblocks,numblocks*sizeof(gameblock*));
	allblocks=result;
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
    maxblocks++;
    allblocks[0]->colours=frand();
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
    allblocks[0]->rect=malloc(sizeof(SDL_Rect));
    draw_block_(&allblocks[0]->rect,&allblocks[0]->tex);
    gfx_update();
    if(game_checkspot(0,0,0)>=0)gameover=1;
}
void game_updateblockposition(void){
    int i;
    for(i=0;i<numblocks;i++){
        if((fabs(allblocks[i]->xv)>0.001f)||(fabs(allblocks[i]->yv)>0.001f))
            game_moveblockv(i);
    }
}

int array_checkcontains(int* arr,int max,int val){
    int i;
    for(i=0;i<max;i++){
        if(arr[i]==val)return 1;
    }
    return 0;
}


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
        return 1;
    }
    return 0;
}

#define clearblocks(x,y,z)clearblocks_rcheck(list,count,start,stationary,nstationary,x,y,z)
void clearblocks_rdo(int *list,int *count,int start,int *stationary,
    int nstationary){
    clearblocks(1,0,2);
    clearblocks(-1,0,3);
    clearblocks(0,1,1);
    clearblocks(0,-1,0);
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
        updatescore();
        sound_play(2);
    }else{
        for(i=0;i<rcount;i++,(*numchecked)++)checked[*numchecked]=rlist[i];
    }
}

void game_clearblocks(void){
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
void releaseblock(void){
    int i;
    int top_x=y_max+1;
    for(i=1;i<numblocks;i++){
        if(allblocks[i]->x==allblocks[0]->x&&allblocks[i]->y<top_x)top_x=allblocks[i]->y;
    }
    game_moveblock(0,0,top_x-allblocks[0]->y-1);
}
void game_init(void){
    sound_init();
    sound_startbgm();
    score=0;
    speed=0;
    gameover=0;
    bonus=0;
    gametimer=SDL_GetTicks();
    movementtime=gametimer;
	seed_frand_time();
    spawnblock();
}
void game_done(void){
    sound_done();
    int i;
    for(i=0;i<numblocks;i++)free(allblocks[i]);
	numblocks=0;
}
void game_reset(void){
    int i;
    for(i=0;i<numblocks;i++)free(allblocks[i]);
    score=0;gameover=0;speed=0;numblocks=0;bonus=0;
    updatescore();
	seed_frand_time();
    spawnblock();
    sound_play(1);
}
void game_do(void){
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
