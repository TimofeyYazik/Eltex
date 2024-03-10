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

void clear_stdin();

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
        case 5:
            break;
        default:
            printf("operation not found or error type input\n");
            break;
        }
    }
}

void clear_stdin(){
    char c = 0;
    while ((c = getchar()) != '\n' && c != EOF){}
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
    scanf("%9s", del_name);
    clear_stdin();
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
    scanf("%9s", search_name);
    clear_stdin();
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
    clear_stdin();
    return menu;
}

void new_abonent(abonent mass[MAX_SIZE_MASS]){
    short i = 0;
    for(i = 0; i < MAX_SIZE_MASS; i++){
        if(mass[i].name[0] == 0) break;
    }
    if((i == MAX_SIZE_MASS - 1) && mass[i].name[0]){
        printf("abonents is full");
    }else{
        scanf("%9s", mass[i].name);
        clear_stdin();
        scanf("%9s", mass[i].second_name);
        clear_stdin();
        scanf("%9s", mass[i].tell);
        clear_stdin();
    }
}