
#include <dirent.h>
#include <fcntl.h> /* Definition of AT_* constants */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

#define MAX_ENTRIES 100

void QuickSort(struct dirent *namelist[], int start, int finish);

int main() {
  int len_namelist = 0;
  struct dirent **namelist;
  int ch = 0;
  while (1)
  {
    len_namelist = scandir(".", &namelist, NULL, alphasort);
    for (int i = 0; i < len_namelist; i++) {
      printf("%d) %s\n", i, namelist[i]->d_name);
    }
    printf("Enter the index of the directory you want to enter, or -1 to exit: ");
    int driver;
    scanf("%d", &driver);
    if (driver < 0 || driver >= len_namelist) break;
    printf("\n\n\n");
    chdir(namelist[driver]->d_name);
    free(namelist);
  }
  free(namelist);

  exit(EXIT_SUCCESS);
}

// void QuickSort(struct dirent *namelist[], int start, int finish) {
//   int left = start;
//   int right = finish;
//   int middle = (start + finish) / 2;
//   char pivot = namelist[middle]->d_name[0];  

//   while (left <= right) {
//     while (namelist[left]->d_name[0] < pivot) left++;
//     while (namelist[right]->d_name[0] > pivot)

//  right--;

//     if (left <= right) {
//       struct dirent *buff = namelist[left];
//       namelist[left] = namelist[right];
//       namelist[right] = buff;
//       left++;
//       right--;
//     }
//   }

//   if (start < right) QuickSort(namelist, start, right);
//   if (left < finish) QuickSort(namelist, left, finish);
// }
