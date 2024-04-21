#ifndef ADD_FUNC_H
#define ADD_FUNC_H

#include "../custom_type.h"

void safe_input(input_user *data);
char **tokenize_string(const char *str, const char *delimiters,
                       int *num_tokens);

#endif // ADD_FUNC_H