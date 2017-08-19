#include <stddef.h>
#include "menu.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "input.h"
#include "gfx.h"
#include "game.h"
#include "sys/stat.h"
#ifndef _WIN_
#include <linux/limits.h>
#else
#include <limits.h>
//#define PATH_MAX 259
#define NAME_MAX 255
#endif
#ifndef MYDEF
#include "mytypes.h"
#endif

int menu_position = 0;
MENU_CATEGORY menu_state = MAIN_MENU;
char* main_menu[]={"3","start","file browser","quit"};

int numfiles;
char* currentpath;
char** file_browser;
//typedef enum {EMPTY,LOADED} FBSTATE;
//int file_browser_state=EMPTY;
int menuline_offset=0;

void menu_initbrowser(){
	currentpath=malloc(PATH_MAX*sizeof(char));
//	printf("got PATH_MAX: %d\n",PATH_MAX);
}
void menu_donebrowser(){
	free(currentpath);
	int i;
	for(i=0;i<numfiles;i++)free(file_browser[i]);//invalid free
}

void menu_pointer(int value){
	int max;
	switch(menu_state){
		case(MAIN_MENU):max=atoi(main_menu[0])-1;break;
		case(DIR_MENU):max=min(numfiles-menuline_offset*maxlines()-1,maxlines()-1);break;	
		default:max=0;break;
	}
	int old_position=menu_position;

	if(menu_state==DIR_MENU){
		if(menu_position+value>max&&(menu_position+value+maxlines()*menuline_offset+1)<numfiles-1){
			del_menu();
			menu_position=0;
			menuline_offset++;
			init_menu();
			return;
			}else if(menu_position+value<0&&menu_position+value+maxlines()*menuline_offset>0){
			del_menu();
			menu_position=maxlines()-1;
			menuline_offset--;
			init_menu();
			return;
		}
	}

	menu_position+=value;
	if(menu_position>max)menu_position=max;
	else if(menu_position<0)menu_position=0;
	updatemenu(menu_position,menu_state);
	updatemenu(old_position,menu_state);
}

char* menu_getline(int line,MENU_CATEGORY category){
	char *str;
	switch(category){
		case MAIN_MENU:str=main_menu[line+1]; break;
		case DIR_MENU:str=file_browser[line]; break;
		default:str=NULL; break;
	}
	//printf("got line: %s\n",str);
	return str;
}
int menu_getsize(MENU_CATEGORY category){
	int size;
	switch(category){
		case MAIN_MENU:size=atoi(main_menu[0]);break;
		case DIR_MENU:size=numfiles;break;
		default:size=0;break;
	}
	return size;
}
/* qsort C-string comparison function */ 
int cstring_cmp(const void *a, const void *b) 
{ 
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
	/* strcmp functions works exactly as expected from
	comparison function */ 
} 
void menu_loadbrowser(char* path){
#ifndef _WIN_
/*	if(file_browser_state==EMPTY){
		//currentpath=malloc(PATH_MAX*sizeof(char));
		printf("got PATH_MAX: %d\n",PATH_MAX);
		file_browser_state=LOADED;
	}*/
	//printf("trying path: %s\n",path);
	struct stat pathstat;
	stat(path,&pathstat);
	if(S_ISDIR(pathstat.st_mode)||S_ISLNK(pathstat.st_mode)){
		realpath(path,currentpath);
		//printf("got directory: %s\n",currentpath);
	}else{
		//printf("path was not a directory\n");
	}
	//strcpy(currentpath,path);

	DIR *dp;
	struct dirent *ep;

	dp = opendir (currentpath);
	if (dp != NULL)
	{
		int count=0;
		while((ep=readdir(dp))){
			count++;
			//if(count==1)file_browser=malloc(sizeof(char*));
			//else file_browser=realloc(file_browser,(count+1)*sizeof(char*));
			char** result=realloc(file_browser,(count+1)*sizeof(char*));
			if(result==NULL) free(result);
			else file_browser=result;
			char* str=ep->d_name;
			//printf("got NAME_MAX: %d\n",NAME_MAX);
			file_browser[count-1]=malloc(NAME_MAX*sizeof(char));//
			strncpy(file_browser[count-1],str,NAME_MAX);//
		}
		(void)closedir(dp);
		numfiles=count;
		qsort(file_browser,numfiles,sizeof(*file_browser),cstring_cmp);
	}
#endif
}

void menu_action(MENU_ACTION action){
	char *str;
	switch(menu_state){
		case MAIN_MENU:
			switch(action){
				case MENU_ACTION_ENTER:
					switch(menu_position){
						case 1:
							del_menu();
							menu_state=DIR_MENU;
							menu_position=0;
							menuline_offset=0;
							//menu_loadbrowser(getenv("HOME"));
							menu_initbrowser();
							menu_loadbrowser("/home/daniel/");
							init_menu();
							break;
						case 0:
							menu_state=MENU_NONE;
							game_state=GAME_START;
							del_menu();
							game_init();
							init_game();
							break;
						case 2:
							QUIT=1;
							//del_menu();
							break;
					}
					break;
				case MENU_ACTION_RETURN:
					QUIT=1;
					break;
				default:break;
			}
			break;
		case DIR_MENU:
			switch(action){
				case MENU_ACTION_ENTER:
					del_menu();
					str=malloc(PATH_MAX*sizeof(char));
					str=strcpy(str,currentpath);
					str=strcat(str,"/");
					str=strcat(str,file_browser[menu_position]);
					menu_position=0;
					menuline_offset=0;
					menu_loadbrowser(str);
					free(str);
					init_menu();
					break;
				case MENU_ACTION_RETURN:
					del_menu();
					str=malloc(PATH_MAX*sizeof(char));
					str=strcpy(str,currentpath);
					str=strcat(str,"/..");
					menu_position=0;
					menuline_offset=0;
					menu_loadbrowser(str);
					free(str);
					//free(currentpath);
					//free(file_browser);
					//file_browser_state=EMPTY;
					init_menu();
					break;
				case MENU_ACTION_ESCAPE:
					del_menu();
					menu_state=MAIN_MENU;
					menu_position=0;
					menuline_offset=0;
					menu_donebrowser();
					//file_browser_state=EMPTY;
					init_menu();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}
