#include <stdio.h>

int main(){
    int res = 0;
    char *point_res = &res;
    point_res += 2;
    if(scanf("%d", point_res) != 1){
        printf("error");
    } else{
        printf("%d", res);   
    }
    return 0;
}