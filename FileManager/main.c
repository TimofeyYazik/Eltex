#include <dirent.h>
#include <fcntl.h> /* Definition of AT_* constants */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
// #include <stdint.h>
#include <errno.h>

// #define MAX_LEN_NAME_FILE 256

int main() {
  int counter = 0;
  // DIR *use_dir = opendir("../");
  struct dirent *namelist[100];

  DIR *use_dir = opendir(".");
  int driver = 0;
  while (1) {
    counter = 0;
    struct dirent *buff;
    while ((buff = readdir(use_dir)) != NULL) {
      namelist[counter] = buff;
      counter++;
    }
    for (int i = 0; i < counter; i++) {
      printf("%d) %s\n", i, namelist[i]->d_name);
    }
    scanf("%d", &driver);
    if (driver >= counter || driver < 0) break;
    closedir(use_dir);
    use_dir = opendir(namelist[driver]->d_name);
    long ds = telldir(use_dir);
    seekdir(use_dir, ds);
  }
  if (use_dir != NULL) closedir(use_dir);
  free(namelist);
  exit(EXIT_SUCCESS);
}

void QuickSort(struct dirent *namelist[], int start, int finish) {
  int left = start;
  int right = finish;
  int middle = (start + finish) / 2;
  char pivot = namelist[middle]->d_name[0];  // Используем первый символ имени в
                                             // качестве опорного элемента

  // Разделение массива на две части
  while (left <= right) {
    while (namelist[left]->d_name[0] < pivot) left++;
    while (namelist[right]->d_name[0] > pivot) right--;

    if (left <= right) {
      struct dirent *buff = namelist[left];
      namelist[left] = namelist[right];
      namelist[right] = buff;
      left++;
      right--;
    }
  }

  // Рекурсивный вызов для двух подмассивов
  if (start < right) QuickSort(namelist, start, right);
  if (left < finish) QuickSort(namelist, left, finish);
}