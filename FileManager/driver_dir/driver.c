#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

#include "../vim_connect/process.h"
#include "driver.h"



int DriverDir(DirList *dir_list, char *name, SwitchDir *sw_dr) {
  int error = 0;  
  struct stat statbuf;
  if(sw_dr->dr[sw_dr->win] != NULL){
    fchdir(sw_dr->ds_dir[sw_dr->win]);
    closedir(sw_dr->dr[sw_dr->win]);
  }
  stat(name, &statbuf);
  if(S_ISDIR(statbuf.st_mode)){
  sw_dr->dr[sw_dr->win] = opendir(name);
  sw_dr->ds_dir[sw_dr->win] = dirfd(sw_dr->dr[sw_dr->win]);
  if(strcmp(name, ".") != 0){
    error = fchdir(sw_dr->ds_dir[sw_dr->win]);
  }  
  if(error != -1){
    dir_list->len_namelist = scandir(".", &dir_list->namelist_dir, NULL, alphasort);
    if(dir_list->len_namelist == -1){
      error = -1;
    }
    }
  } 
  if(S_ISREG(statbuf.st_mode)){
    VimCall(name);
  }

  return error;
}