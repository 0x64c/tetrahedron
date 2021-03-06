#define MYDEF

#define x_max 6
#define y_max 12

/*#define max(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a > _b ? _a : _b; })*/

#define min(a,b) \
__extension__({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a < _b ? _a : _b; })

typedef enum {
	MENU_NONE=	1<<0,
	MAIN_MENU=	1<<1,
	DIR_MENU=	1<<2
} MENU_CATEGORY;
typedef enum{
	MENU_ACTION_ENTER=	1<<0,
	MENU_ACTION_RETURN=	1<<1,
	MENU_ACTION_ESCAPE=	1<<2
} MENU_ACTION;
typedef enum{
	GAME_NONE=		1<<0,
	GAME_START=		1<<1,
	GAME_END=		1<<2
} GAME_STATE;

