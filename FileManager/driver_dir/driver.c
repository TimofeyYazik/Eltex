#include "driver.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int DriverDir(DirList *dir_list, char *name, SwitchDir *sw_dr) {
  int error = 0;
  struct stat statbuf;
  if (sw_dr->dr[sw_dr->win] != NULL) {
    if (fchdir(sw_dr->ds_dir[sw_dr->win]) == -1) {
      perror("Error changing directory");
      error = -1;
      goto Error;
    }
  }
  if (stat(name, &statbuf) == -1) {
    perror("Error getting file status");
    error = -1;
    goto Error;
  }
  if (S_ISDIR(statbuf.st_mode)) {
    sw_dr->dr[sw_dr->win] = opendir(name);
    if (sw_dr->dr[sw_dr->win] == NULL) {
      perror("Error opening directory");
      error = -1;
      goto Error;
    }
    sw_dr->ds_dir[sw_dr->win] = dirfd(sw_dr->dr[sw_dr->win]);
    if (sw_dr->ds_dir[sw_dr->win] == -1) {
      perror("Error getting directory descriptor");
      error = -1;
      goto Error;
    }
    if (strcmp(name, ".") != 0) {
      if (fchdir(sw_dr->ds_dir[sw_dr->win]) == -1) {
        perror("Error changing directory");
        error = -1;
        goto Error;
      }
    }
    free(dir_list->namelist_dir);
    dir_list->len_namelist =
        scandir(".", &dir_list->namelist_dir, NULL, alphasort);
    if (dir_list->len_namelist == -1) {
      perror("Error scanning directory");
      error = -1;
      goto Error;
    }
  }
  if (S_ISREG(statbuf.st_mode)) {
    error = FILE_VIM_OPEN;
  }
Error:
  return error;
}
