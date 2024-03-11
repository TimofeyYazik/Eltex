#include <stdio.h>

#define MASK 0x80000000

int main(){
    int num = 0, res = 0;
    scanf("%d", &num);
    while (num != 0)
    {
        if(num & MASK){
            res++;
        }
        num <<= 1;
    }
    printf("%d\n", res);
}