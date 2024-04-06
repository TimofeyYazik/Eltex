#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(){
  int size = 100;
  int len = 0;
  char ch = 0;
  char *buffer = calloc(size, sizeof(char));
  while (ch != '\n'){
    ch = getchar();
    if(len == size){
      size = size * 3 / 2;
      buffer = realloc(buffer, size);
    }
    buffer[len] = ch;
    len++;
  }
  int ds_file = open("test.txt", O_APPEND | O_CREAT | O_WRONLY);
  write(ds_file, buffer, len);
  return 0;
}
