#include "menu.h"
#include "input.h"
#include "gfx.h"
#include "game.h"
#ifndef MYDEF
#include "mytypes.h"
#endif

int menu_position = 0;
MENU_CATEGORY menu_state = MAIN_MENU;
char* main_menu[]={"start","quit"};
typedef enum{
    MENU_START,
    MENU_QUIT,
    MENU_NUM_ITEMS
}MENU_ITEMS;

int menuline_offset=0;

void menu_pointer(int value){
    int max;
    switch(menu_state){
        case(MAIN_MENU):max=MENU_NUM_ITEMS;break;
        default:max=0;break;
    }
    int old_position=menu_position;
    menu_position+=value;
    if(menu_position==max)menu_position=max-1;
    else if(menu_position<0)menu_position=0;
    updatemenu(menu_position,menu_state);
    updatemenu(old_position,menu_state);
}
char* menu_getline(int line,MENU_CATEGORY category){
    char *str;
    switch(category){
        case MAIN_MENU:str=main_menu[line]; break;
        default:str=NULL; break;
    }
    return str;
}
int menu_getsize(MENU_CATEGORY category){
    int size;
    switch(category){
        case MAIN_MENU:size=MENU_NUM_ITEMS;break;
        default:size=0;break;
    }
    return size;
}
void menu_action(MENU_ACTION action){
    switch(menu_state){
        case MAIN_MENU:
            switch(action){
                case MENU_ACTION_ENTER:
                    switch(menu_position){
                        case MENU_START:
                            menu_state=MENU_NONE;
                            game_state=GAME_START;
                            del_menu();
                            game_init();
                            init_game();
                            break;
                        case MENU_QUIT:
                            QUIT=1;
                            break;
                    }
                    break;
                case MENU_ACTION_RETURN:
                    QUIT=1;
                    break;
                default:break;
            }
            break;
        default:
            break;
    }
}
