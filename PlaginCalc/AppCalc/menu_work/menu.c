#include <stdio.h>
#include <string.h>

#include "../castom_type.h"


int Menu(const service_info service) {
    int menu = 0;
    int num = 1;
    printf("0) Exit\n");
    for (int i = 0; i < service.func_count; i++) {
        if (!strcmp(service.func[i], "new lib")) {
            continue;
        }
        printf("%d) %s\n", num, service.func[i]);
        num++;
    }
    if(scanf("%d", &menu) != 1){
      menu = 0;
    }
    return menu;
}

