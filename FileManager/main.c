#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>          /* Definition of AT_* constants */
#include <dirent.h>
#include <sys/types.h>
// #include <stdint.h>
#include <errno.h>

#define MAX_LEN_NAME_FILE 255

int main(){
  int counter = 0;
  // DIR *use_dir = opendir("../");
  struct dirent **namelist;
  // struct dirent *buff;
  // while((buff = readdir(use_dir)) != NULL){
  //   namelist[counter] = buff;
  //   counter++;
  // }
  int driver = 0;
  int len_namelist = scandir(".", &namelist, NULL, alphasort);
  while(driver != -1){
    for(int i = 0; i < len_namelist; i++){
      printf("%d) %s\n", i ,namelist[i]->d_name);
    }
    scanf("%d", driver);
    switch_dir(namelist[i]->d_name);
  }
  // closedir(use_dir);
  exit(EXIT_SUCCESS);
}


void switch_dir(char name[256]){
  DIR* use_dir = opendir(name);
  long ds = telldir(use_dir);
  seekdir(use_dir, ds);
  closedir(use_dir);
}