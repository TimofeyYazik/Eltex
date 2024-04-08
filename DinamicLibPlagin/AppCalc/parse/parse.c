#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "parse.h"
#include "../castom_type.h"

#define SIZE_BUFF  80

static void _CheckErrorMemoryLib(service_info *service){
  if(service->lib == NULL){
    ClearService(service);
    perror("error");
    exit(EXIT_FAILURE);
  }
}

static void _CheckErrorMemoryFunc(service_info *service){
  if(service->func == NULL){
    ClearService(service);
    perror("error");
    exit(EXIT_FAILURE);
  }
}

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
        _CheckErrorMemoryLib(service);
        service->lib[service->lib_count - 1] = calloc(SIZE_BUFF, sizeof(char));
        if(service->lib[service->lib_count - 1] == NULL){
          ClearService(service);
          perror("error");
          exit(EXIT_FAILURE);
        }
        strcpy(service->lib[service->lib_count - 1], optarg);
      } else {
        prev_flag = flag;
        service->lib_count++;
        service->lib = realloc(service->lib, sizeof(char *) * service->lib_count);
        _CheckErrorMemoryLib(service);
        service->lib[service->lib_count - 1] = calloc(SIZE_BUFF, sizeof(char));
        if(service->lib[service->lib_count - 1] == NULL){
          ClearService(service);
          perror("error");
          exit(EXIT_FAILURE);
        }
        strcpy(service->lib[service->lib_count - 1], optarg);
        service->func_count++;
        service->func = realloc(service->func, sizeof(char *) * service->func_count);
        _CheckErrorMemoryFunc(service);
        service->func[service->func_count - 1] = calloc(SIZE_BUFF, sizeof(char));
        if(service->func[service->func_count - 1] == NULL){
          ClearService(service);
          perror("error");
          exit(EXIT_FAILURE);
        }
        strcpy(service->func[service->func_count - 1], "new lib");
      }
      break;
    case 'f':
      service->func_count++;
      service->func = realloc(service->func, sizeof(char *) * service->func_count);
      _CheckErrorMemoryFunc(service);
      service->func[service->func_count - 1] = calloc(SIZE_BUFF, sizeof(char));
      if(service->func[service->func_count - 1] == NULL){
          ClearService(service);
          perror("error");
          exit(EXIT_FAILURE);
        }
      strcpy(service->func[service->func_count - 1], optarg);
      break;
    default:
      break;
    }
  }

}

void ClearService(service_info *service){
  if(service->func != NULL){
    for(int i = 0; i < service->func_count; i++){
      if(service->func[i] != NULL)
        free(service->func[i]);
    }
    free(service->func);
  }
  if(service->lib != NULL){
    for(int i = 0; i < service->lib_count; i++){
      if(service->lib[i] != NULL)
        free(service->lib[i]);
    }
    free(service->lib);
  }
}
