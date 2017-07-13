#ifndef MYDEF
#include "mytypes.h"
#endif
extern void menu_pointer(int);
extern void menu_action(MENU_ACTION);
extern char* menu_getline(int,MENU_CATEGORY);
extern int menu_getsize(MENU_CATEGORY);
extern void menu_loadbrowser(char*);
extern int menu_position;
extern int menuline_offset;
extern MENU_CATEGORY menu_state;
