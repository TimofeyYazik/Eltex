#ifndef FILE_MANAGER_DRIVER_DIR_DRIVER_H_
#define FILE_MANAGER_DRIVER_DIR_DRIVER_H_

typedef struct{
  char win; // 0 left win 1 right win
  int ds_dir[2]; // 0 left win 1 right win
  DIR *dr[2]; // 0 left win 1 right win
} SwitchDir;

typedef struct{
  int len_namelist; 
  struct dirent **namelist_dir;
} DirList;

int DriverDir(DirList *dir_list, char *name_dir, SwitchDir *ds_dir);

#endif //FILE_MANAGER_DRIVER_DIR_DRIVER_H_
