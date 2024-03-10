#include <stdio.h>

#define N 6

int main(){
    int mass[N] = {0};
    int buf = 0;
    for(int i = 0; i < N; i++){
        scanf("%d", &mass[i]);
    }
    for (int i = 0, j = N - 1; i <= j ; i++, j--)
    {
        buf = mass[i];
        mass[i] = mass[j];
        mass[j] = buf;
    }
    for(int i = 0; i < N; i++){
        printf("%d ", mass[i]);
    }

    return 0;
}
