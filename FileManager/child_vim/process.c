#include <sys/wait.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>


void VimCall(char *name) {
    int status = 0;
    pid_t pid_vim = fork();
    if (!pid_vim) {
        execl("/usr/bin/vim", "vim", name, NULL);
        perror("execl failed");
        exit(EXIT_FAILURE);
    } else if (pid_vim < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    waitpid(pid_vim, &status, 0);
}
