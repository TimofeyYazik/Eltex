#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "parse.h"
#include "../castom_type.h"

#define SIZE_BUFF  80

void parse_flags(service_info *servise, int argc, char **argv){
  char ch = 0;
  while ((ch = getopt_long(argc, argv, "p:f:", NULL, NULL)) != -1){
    switch (ch) {
    case 'p':
      servise->lib = malloc(strlen(optarg));
      strcpy(servise->lib, optarg);
      break;
    case 'f':
      if(!servise->func_count){
        servise->func_count++;
        servise->func = malloc(sizeof(char *) * servise->func_count);
        servise->func[servise->func_count - 1] = calloc(SIZE_BUFF, sizeof(char));
        strcpy(servise->func[servise->func_count - 1], optarg);
      } else {
        servise->func_count++;
        printf("%d %s \n", servise->func_count, servise->func[0]);
        servise->func = realloc(servise->func, servise->func_count);
        printf("%d %s \n", servise->func_count, servise->func[0]);
        servise->func[servise->func_count - 1] = calloc(SIZE_BUFF, sizeof(char));
        printf("%d %s \n", servise->func_count, servise->func[0]);
        strcpy(servise->func[servise->func_count - 1], optarg);
        printf("%d %s \n", servise->func_count, servise->func[0]);
      }
      break;
    default:
      break;
    }
  }

}