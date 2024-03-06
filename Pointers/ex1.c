#include <stdio.h>

int main(){
    int res = 0;
    char *point_res = &res;
    point_res += 2;
    scanf("%d", point_res);
    printf("%d", res);
}