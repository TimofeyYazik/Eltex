#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int DriverDir(int *len_namelist, struct dirent ***namelist_dir, char *name_dir) {
  int error = 0;
  if(strcmp(name_dir, ".") != 0){
    error = chdir(name_dir);
    // perror("Not dir:");
  }  
  if(error != -1){
    *len_namelist = scandir(".", namelist_dir, NULL, alphasort);
  }

  return error;
}