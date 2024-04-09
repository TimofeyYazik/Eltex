#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>          /* Definition of AT_* constants */
#include <dirent.h>
#include <sys/types.h>
// #include <stdint.h>
#include <errno.h>

#define MAX_LEN_NAME_FILE 256

int main(){
  int counter = 0;
  // DIR *use_dir = opendir("../");
  struct dirent *namelist[100];

  DIR *use_dir = opendir(".");
  int driver = 0;
  while(1){
    struct dirent *buff;
    while((buff = readdir(use_dir)) != NULL){
    namelist[counter] = buff;
    counter++;
    }
    for(int i = 0; i < counter; i++){
      printf("%d) %s\n", i, namelist[i]->d_name);
    }
    scanf("%d", &driver);
    if(driver >= counter || driver < 0) break;
    closedir(use_dir);
    use_dir(namelist[driver]->d_name);
    long ds = telldir(use_dir);
    seekdir(use_dir, ds);
  }
  if(use_dir != NULL)
    closedir(use_dir);
  free(namelist);
  exit(EXIT_SUCCESS);
}

