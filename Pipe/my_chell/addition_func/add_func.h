#ifndef ADD_FUNC_H
#define ADD_FUNC_H

#include "../custom_type.h"

void SafeInput(input_user *data);
char **TokenizeString(const char *str, const char *delimiters,
                       int *num_tokens);

#endif // ADD_FUNC_H