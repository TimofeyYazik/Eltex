#include <sys/wait.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>


void VimCall(char *name) {
    int status = 0;
    pid_t pid_vim = fork();
    if (!pid_vim) {
        execl("/usr/bin/vim", "vim", name, NULL);
    } 
    waitpid(pid_vim, &status, 0);
}
