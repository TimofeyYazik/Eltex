#include <stdio.h>
#include <string.h>

#define MAX_SIZE_MASS 100
#define SIZE_ELEMENTS 10

typedef struct {
    char name[SIZE_ELEMENTS];
    char second_name[SIZE_ELEMENTS];
    char tell[SIZE_ELEMENTS];
}abonent;

int menu_bar();
void new_abonent(abonent mass[MAX_SIZE_MASS]);
void search_abonent(abonent mass[MAX_SIZE_MASS]);
void print_abonent(abonent mass[MAX_SIZE_MASS]);
void del_abonent(abonent mass[MAX_SIZE_MASS]);

int main(){
    abonent mass[MAX_SIZE_MASS] = {0};
    int argument = 0;
    while(argument != 5){
        argument = menu_bar();
        if(argument == 1){
            new_abonent(mass);
        } else if(argument == 2){
            del_abonent(mass); 
        } else if(argument == 3){
            search_abonent(mass);
        } else if(argument == 4){
            print_abonent(mass);
        }
    }
}

void print_abonent(abonent mass[MAX_SIZE_MASS]){
    for(short i = 0; i < MAX_SIZE_MASS; i++){
        if(mass[i].name[0]){
            printf("%s %s %s\n", mass[i].name, mass[i].second_name, mass[i].tell);
    }
}}

void del_abonent(abonent mass[MAX_SIZE_MASS]){
    char del_name[SIZE_ELEMENTS] = {0};
    char n = 0;
    scanf("%s", del_name);
    for(short i = 0; i < SIZE_ELEMENTS; i++){
        if(strcmp(mass[i].name, del_name) == 0){
            memset(mass[i].name, 0, SIZE_ELEMENTS);
            memset(mass[i].second_name, 0, SIZE_ELEMENTS);
            memset(mass[i].tell, 0, SIZE_ELEMENTS);
        }
    }
}

void search_abonent(abonent mass[MAX_SIZE_MASS]){
    char search_name[SIZE_ELEMENTS] = {0};
    scanf("%s", search_name);
    for(short i = 0; i < SIZE_ELEMENTS; i++){
        if(strcmp(mass[i].name, search_name) == 0){
            printf("%s %s %s\n", mass[i].name, mass[i].second_name, mass[i].tell);
        }
    }
}

int menu_bar(){
    int menu = 0;
    printf("1. new abonent\n");
    printf("2. del abonent\n");
    printf("3. search abonent\n");
    printf("4. print abonent\n");
    printf("5. exit\n");  
    scanf("%d", &menu);
    return menu;
}

void new_abonent(abonent mass[MAX_SIZE_MASS]){
    short i = 0;
    for(; i < MAX_SIZE_MASS; i++){
        if(mass[i].name[0] == 0) break;
    }
    if(i == 99 && mass[i].name[0]){
        printf("abonents is full");
    }else{
        scanf("%s", mass[i].name);
        scanf("%s", mass[i].second_name);
        scanf("%s", mass[i].tell); // Подразумеваеться что все данные верны и не больше 10 символов
    }
}