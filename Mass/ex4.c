#include <stdio.h>

#define N 3

int main(){
    int mass[N][N] = {0};
    int i = 0;
    int j = 0;
    for(int counter = 1; counter <= N * N;){
        for(; j < N && mass[i][j] == 0; j++){
            mass[i][j] = counter;
            counter++;
        }
        j--, i++;
        for(; i < N && mass[i][j] == 0; i++){
            mass[i][j] = counter;
            counter++;
        }
        i--, j--;
        for( ; j >= 0 && mass[i][j] == 0; j--){
            mass[i][j] = counter;
            counter++;
        }
        j++, i--;
        for(; i >= 0 && mass[i][j] == 0; i--){
            mass[i][j] = counter;
            counter++;
        }
        i++, j++;
    }
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
        printf("%4d", mass[i][j]);
        }
        printf("\n");
    }
    return 0;
}