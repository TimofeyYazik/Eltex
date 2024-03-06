#include <stdio.h>

int main(){
    char str[40] = {0};
    char substr[40] = {0};
    char *res_point = NULL;
    char check_is_substr = 0;
    short j = 0, i = 0;
    for(;(str[j] = getchar()) != '\n'; j++);
    for(;(substr[i] = getchar()) != '\n'; i++);
    for(i = 0; str[i] != '\n'; i++){
        if(str[i] == substr[0]){
            check_is_substr = 1;
            for(j = 0; substr[j] != '\n'; j++, i++){
                 if(str[i] != substr[j] || str[i] == '\n'){
                    i -= j;
                    check_is_substr = 0;
                    break;
                 }
            }
        }
        if(check_is_substr){
            res_point = &str[i - j];
            break;
        }
    }
    if(res_point)
        printf("%s", res_point);
}