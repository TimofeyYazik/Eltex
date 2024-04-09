#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>          /* Definition of AT_* constants */
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_LEN_NAME_FILE 255

int main(){
  int counter = 0;
  DIR *use_dir = opendir("../");
  struct dirent *namelist[100] = {NULL};
  struct dirent *buff;
  while((buff = readdir(use_dir)) != NULL){
    namelist[counter] = buff;
    counter++;
  }
  
  for(int i = 0; i < count_line; i++){
    printf("%s\n", namelist[i]->d_name);
  }
  
  closedir(use_dir);
  exit(EXIT_SUCCESS);
}
