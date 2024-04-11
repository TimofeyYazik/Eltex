#ifndef FILE_MANAGER_CASTOM_TYPE_H_
#define FILE_MANAGER_CASTOM_TYPE_H_

#include <dirent.h>

#define FILE_VIM_OPEN 7
#define LEFT_WINDOW_ACTIVITY 1
#define RIGHT_WINDOW_ACTIVITY -1
#define TRUE 1
#define FALSE 0
#define KEY_ETR 10
#define KEY_TAB 9

typedef struct{
  int win; 
  int ds_dir[2];
  DIR *dr[2]; 
} SwitchDir; // 0 left window 1 right window

typedef struct{
  int len_namelist; 
  struct dirent **namelist_dir;
} DirList;

#endif // FILE_MANAGER_CASTOM_TYPE_H_