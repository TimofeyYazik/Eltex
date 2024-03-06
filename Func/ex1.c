#include <stdio.h>
#include <string.h>

typedef struct {
    char name[10];
    char second_name[10];
    char tell[10];
}abonent;

int menu_bar();
void new_abonent(abonent mass[100]);
void search_abonent(abonent mass[100]);
void print_abonent(abonent mass[100]);
void del_abonent(abonent mass[100]);

int main(){
    abonent mass[100] = {0};
    int menu = 0;
    while(menu != 5){
        menu = menu_bar();
        if(menu == 1){
            new_abonent(mass);
        } else if(menu == 2){
            del_abonent(mass); 
        } else if(menu == 3){
            search_abonent(mass);
        } else if(menu == 4){
            print_abonent(mass);
        }
    }
}

void print_abonent(abonent mass[100]){
    for(short i = 0; i < 100; i++){
        if(mass[i].name[0] != 0){
            printf("%s %s %s\n", mass[i].name, mass[i].second_name, mass[i].tell);
        }
    }
}

void del_abonent(abonent mass[100]){
    char del_name[10] = {0};
    scanf("%s", del_name);
    for(short i = 0; i < 100; i++){
        if(strcmp(mass[i].name, del_name) == 0){
            memset(mass[i].name, 0, 10);
            memset(mass[i].second_name, 0, 10);
            memset(mass[i].tell, 0, 10);
        }
    }
}

void search_abonent(abonent mass[100]){
    char search_name[10] = {0};
    scanf("%s", search_name);
    for(short i = 0; i < 100; i++){
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

void new_abonent(abonent mass[100]){
    short i = 0;
    for(; i < 100; i++){
        if(mass[i].name[0] == 0) break;
    }
    if(i == 99 && mass[i].name[0] == 0){
        printf("abonents is full");
    }else{
        scanf("%s", mass[i].name);
        scanf("%s", mass[i].second_name);
        scanf("%s", mass[i].tell); // Подразумеваеться что все данные верны и не больше 10 символов
    }
}