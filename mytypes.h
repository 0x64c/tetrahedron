#define MYDEF

#define max(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a > _b ? _a : _b; })

#define min(a,b) \
({ __typeof__ (a) _a = (a); \
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

