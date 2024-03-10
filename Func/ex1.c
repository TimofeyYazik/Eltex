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

void clear_enter(abonent *a);

int main(){
    abonent mass[MAX_SIZE_MASS] = {0};
    volatile int argument = 0;
    while(argument != 5){
        argument = menu_bar();
        switch (argument)
        {
        case 1:
            new_abonent(mass);
            break;
        case 2:
            del_abonent(mass); 
            break;
        case 3:
            search_abonent(mass);
            break;
        case 4:
            print_abonent(mass);
            break;
        default:
            printf("operation not found or error type input\n");
            break;
        }
    }
}

void clear_enter(abonent *a){
    for(int i = 0; i <  SIZE_ELEMENTS; i++){
        if(a->name[i] == '\n'){
            a->name[i] = 0;
        }
        if(a->second_name[i] == '\n'){
            a->second_name[i] = 0;
        }
        if(a->tell[i] == '\n'){
            a->tell[i] = 0;
        }
    }
}

void print_abonent(abonent mass[MAX_SIZE_MASS]){
    for(short i = 0; i < MAX_SIZE_MASS; i++){
        if(mass[i].name[0]){
            printf("name: %s\n"
                    "second name: %s\n"
                    "call: %s\n\n", mass[i].name, mass[i].second_name, mass[i].tell);
        }
    }
}

void del_abonent(abonent mass[MAX_SIZE_MASS]){
    char del_name[SIZE_ELEMENTS] = {0};
    fgets(del_name, 10, stdin);
    for(short i = 0; i < SIZE_ELEMENTS;i++){
        if(del_name[i] == '\n'){
            del_name[i] = 0;
            break;
        }
    }
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
    for(short i = 0; i < SIZE_ELEMENTS;i++){
        if(search_name[i] == '\n'){
            search_name[i] = 0;
            break;
        }
    }
    for(short i = 0; i < SIZE_ELEMENTS; i++){
        if(strcmp(mass[i].name, search_name) == 0){
            printf("name: %s\n"
                    "second name: %s\n"
                    "call: %s\n", mass[i].name, mass[i].second_name, mass[i].tell);
        }
    }
}

int menu_bar(){
    int menu = 0;
    printf("1. new abonent\n"
            "2. del abonent\n"
            "3. search abonent\n"
            "4. print abonent\n"
            "5. exit\n");  
    if(scanf("%d", &menu) != 1){
        menu = 6;
    }
    fflush(stdin);
    return menu;
}

void new_abonent(abonent mass[MAX_SIZE_MASS]){
    short i = 0;
    for(i = 0; i < MAX_SIZE_MASS; i++){
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
        clear_enter(&mass[i]);
    }
}