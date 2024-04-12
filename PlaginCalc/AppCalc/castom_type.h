#ifndef APPCALC_CASTOM_TYPE_H_
#define APPCALC_CASTOM_TYPE_H_

typedef struct {
  char **lib;
  char **func;
  int func_count;
  int lib_count;
} ServiceInfo;

typedef struct{
  void **handler_arr;
  int len;
} Handler;

#endif //APPCALC_CASTOM_TYPE_H_