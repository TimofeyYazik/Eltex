#include "add_func.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **tokenize_string(const char *str, const char *delimiters, int *num_tokens) {
    char *str_copy = strdup(str);
    if (str_copy == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(str_copy, delimiters);
    if (token == NULL) { // Если нет токенов, добавляем всю строку в нулевой элемент массива
        *num_tokens = 1;
        char **tokens = malloc(sizeof(char *));
        if (tokens == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        tokens[0] = strdup(str);
        free(str_copy);
        return tokens;
    }

    *num_tokens = 0;
    while (token != NULL) {
        (*num_tokens)++;
        token = strtok(NULL, delimiters);
    }

    char **tokens = malloc((*num_tokens) * sizeof(char *));
    if (tokens == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    token = strtok(strdup(str), delimiters);
    while (token != NULL) {
        tokens[i++] = strdup(token);
        token = strtok(NULL, delimiters);
    }

    free(str_copy);
    return tokens;
}
