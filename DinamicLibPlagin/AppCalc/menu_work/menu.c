#include <stdio.h>
#include <string.h>

#include "../castom_type.h"


int Menu(const service_info service) {
    int menu = 0;
    int num = 1;
    printf("0) Exit\n");
    for (int i = 0; i < service.func_count; i++) {
        // Пропускаем вывод определенной функции (например, "new lib")
        if (!strcmp(service.func[i], "new lib")) {
            continue;
        }
        printf("%d) %s\n", num, service.func[i]);
        num++;
    }
    scanf("%d", &menu);
    return menu;
}

