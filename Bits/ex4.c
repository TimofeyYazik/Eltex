#include <stdio.h>

#define MASK 0xFF00FFFF
#define MASK2 0x00FF0000

int main(){
    int res = 0;
    int byte = 0;
    scanf("%d", &res);
    scanf("%d", &byte);
    byte <<= 16;
    byte |= MASK;
    res &= byte;
    byte &= MASK2;
    res |= byte;
    printf("%d\n", res);
}