#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "parse.h"
#include "../castom_type.h"

#define SIZE_BUFF  80

void ParseFlags(service_info *service, int argc, char **argv){
  char ch = 0;
  char flag = 0, prev_flag = 1;
  while ((ch = getopt_long(argc, argv, "p:f:", NULL, NULL)) != -1){
    switch (ch) {
    case 'p':
      flag++;
      if (flag == prev_flag){
        service->lib_count++;
        service->lib = realloc(service->lib, sizeof(char *) * service->lib_count);
        service->lib[service->lib_count - 1] = calloc(SIZE_BUFF, sizeof(char));
        strcpy(service->lib[service->lib_count - 1], optarg);
      } else {
        prev_flag = flag;
        service->lib_count++;
        service->lib = realloc(service->lib, sizeof(char *) * service->lib_count);
        service->lib[service->lib_count - 1] = calloc(SIZE_BUFF, sizeof(char));
        strcpy(service->lib[service->lib_count - 1], optarg);
        service->func_count++;
        service->func = realloc(service->func, sizeof(char *) * service->func_count);
        service->func[service->func_count - 1] = calloc(SIZE_BUFF, sizeof(char));
        strcpy(service->func[service->func_count - 1], "new lib");
      }
      break;
    case 'f':
      service->func_count++;
      service->func = realloc(service->func, sizeof(char *) * service->func_count);
      service->func[service->func_count - 1] = calloc(SIZE_BUFF, sizeof(char));
      strcpy(service->func[service->func_count - 1], optarg);
      break;
    default:
      break;
    }
  }

}