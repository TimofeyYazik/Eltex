#ifndef DINAMIC_MAS_CASTOM_TYPE_H_
#define DINAMIC_MAS_CASTOM_TYPE_H_

#define SIZE_ELEMENTS 10

typedef struct {
    char name[SIZE_ELEMENTS];
    char second_name[SIZE_ELEMENTS];
    char tell[SIZE_ELEMENTS];
}Abonent;

typedef struct {
  Abonent *abonent_mas;
  int64_t size;
}AbonentMas;

#endif // DINAMIC_MAS_CASTOM_TYPE_H_