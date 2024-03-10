#include <stdio.h>

#define N 5

int main(){
    int mass[N][N] = {0};
    int counter = 1;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            mass[i][j] = counter;
            counter+=1;
        }
    }
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
        printf("%4d", mass[i][j]);
        }
        printf("\n");
    }
    return 0;
}