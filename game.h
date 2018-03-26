#ifndef SDL_h_
#include <SDL2/SDL.h>
#define SDL_h_
#endif

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
    SDL_Rect *rect;
    SDL_Texture *tex;
} gameblock;

extern void game_init(void);
extern void game_done(void);
void game_reset(void);
extern void game_do(void);
extern SDL_Texture *block_gettex(int);
extern SDL_Rect *block_getrect(int);
extern unsigned int block_getcolour(int,int);
extern int game_getnumblocks(void);
extern GAME_STATE game_state;
extern int gameblock_mask[];
extern void spawnblock(void);
extern void game_moveblock(int,int,int);
extern void block_getxy(int,float*,float*);
extern void game_updateblockposition(void);
extern void block_swapcolours(int,int,int);
extern void block_delete(int);
extern void releaseblock(void);
extern int score;
extern int bonus;
extern void getfinexy(int,int,int*,int*);
extern int frand(void);
void seed_frant(uint32_t);
void seed_frand_time(void);
int int_cmp(const void*,const void*);
SDL_Texture *block_gettex(int);
int block_checkcolours(int,int,int);
int game_checkspot(int,int,int);
void game_moveblockv(int);
void swapblock(gameblock**,gameblock**);
int array_checkcontains(int*,int,int);
void clearblocks_rdo(int*,int*,int,int*,int);
int clearblocks_rcheck(int*,int*,int,int*,int,int,int,int);
void clearblocks_rsetup(int*,int*,int*,int*,int,int,int*,int);

