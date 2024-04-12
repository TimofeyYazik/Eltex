#ifndef APPCALC_CONNECT_H_
#define APPCALC_CONNECT_H_

#include "../castom_type.h"

int ConnectLib(int (***func_calc_int)(int, int), Handler *handler, int *count, const ServiceInfo service);

#endif  // APPCALC_CONNECT_H_