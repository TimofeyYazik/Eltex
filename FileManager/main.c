#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>          /* Definition of AT_* constants */
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>


int main(){
  //DIR *use_dir = opendir("../");
  struct dirent **namelist = NULL;
  
  int count_line = scandir("../", &namelist, NULL, alphasort);
  for(int i = 0; i < count_line; i++){
    printf("%s\n", namelist[i]->d_name);
  }
  free(namelist);

  exit(EXIT_SUCCESS);
}
