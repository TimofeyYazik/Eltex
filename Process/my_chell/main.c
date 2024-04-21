#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "custom_type.h"
#include "addition_func/add_func.h"
#include "driver_shell/drive.h"

int main() {
  DriverShell();
  exit(EXIT_SUCCESS);
}
