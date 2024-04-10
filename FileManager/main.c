
#include <dirent.h>
#include <fcntl.h> /* Definition of AT_* constants */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_ENTRIES 100

void QuickSort(struct dirent *namelist[], int start, int finish);

typedef struct 
{
  DIR **use_dir;
  int len;
}DirUse;


int main() {
  int counter = 0;
  struct dirent *namelist[MAX_ENTRIES];
  DirUse dr_us = {0};
  dr_us.use_dir = malloc(MAX_ENTRIES * sizeof(DIR*));
  dr_us.use_dir[dr_us.len] = opendir(".");
  if (dr_us.use_dir[dr_us.len] == NULL) {
    perror("Error opening current directory");
    exit(EXIT_FAILURE);
  }
  while (1) {
    counter = 0;
    struct dirent *buff;
    while ((buff = readdir(dr_us.use_dir[dr_us.len])) != NULL) {
      namelist[counter] = buff;
      counter++;
      if (counter >= MAX_ENTRIES) break; // Prevent buffer overflow
    }
    QuickSort(namelist, 0, counter - 1);
    for (int i = 0; i < counter; i++) {
      printf("%d) %s\n", i, namelist[i]->d_name);
    }

    printf("Enter the index of the directory you want to enter, or -1 to exit: ");
    int driver;
    scanf("%d", &driver);
    printf("\n\n\n");
    if (driver < 0 || driver >= counter) break;
    // closedir(use_dir);
    dr_us.len++;
    dr_us.use_dir[dr_us.len] = opendir(namelist[driver]->d_name);
    if (dr_us.use_dir[dr_us.len] == NULL) {
      perror("Error opening selected directory");
      exit(EXIT_FAILURE);
    }
    rewinddir(dr_us.use_dir[dr_us.len]);
  }
  for(int i = 0; i < dr_us.len; i++){
    if (dr_us.use_dir[i] != NULL) 
      closedir(dr_us.use_dir[i]);
  }
  free(dr_us.use_dir);
  exit(EXIT_SUCCESS);
}

void QuickSort(struct dirent *namelist[], int start, int finish) {
  int left = start;
  int right = finish;
  int middle = (start + finish) / 2;
  char pivot = namelist[middle]->d_name[0];  

  while (left <= right) {
    while (namelist[left]->d_name[0] < pivot) left++;
    while (namelist[right]->d_name[0] > pivot)

 right--;

    if (left <= right) {
      struct dirent *buff = namelist[left];
      namelist[left] = namelist[right];
      namelist[right] = buff;
      left++;
      right--;
    }
  }

  if (start < right) QuickSort(namelist, start, right);
  if (left < finish) QuickSort(namelist, left, finish);
}
