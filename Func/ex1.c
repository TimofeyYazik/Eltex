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
        } else if(argument == 6){
            printf("ERROR\n");
            break;
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
    fgets(del_name, 10, stdin);
    fflush(stdin);
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
    fgets(search_name, 10, stdin);
    fflush(stdin);
    for(short i = 0; i < SIZE_ELEMENTS; i++){
        if(strcmp(mass[i].name, search_name) == 0){
            printf("%s%s%s\n", mass[i].name, mass[i].second_name, mass[i].tell);
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
    if(scanf("%d", &menu) != 1){
        menu = 6;
    }
    fflush(stdin);
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
        fgets(mass[i].name, 10, stdin);
        fflush(stdin);
        fgets(mass[i].second_name, 10, stdin);
        fflush(stdin);
        fgets(mass[i].tell, 10, stdin);
        fflush(stdin);
    }
}