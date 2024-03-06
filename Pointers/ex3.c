#include <stdio.h>

int main(){
    int mass[10] = {1,2,3,4,5,6,7,8,9,10};
    int *mas_point = &mass[0];
    for(short i = 0; i < 10; i++, mas_point++){
        printf("%d\n", *mas_point);
    }
}