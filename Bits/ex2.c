#include <stdio.h>

#define MASK 0x80000000

int main(){
    int res = 0;
    scanf("%d", &res);
    if(res > 0){
        printf("error");
    }
    else{
        for(int i = 1; i < 33; i++)
        {
            if(res & MASK){
                printf("1");
            } else {
                printf("0");
            }
            res <<= 1;
            if((i % 4) == 0){
                printf(" ");
            }
        }
    printf("\n");
    }
    return 0;
}