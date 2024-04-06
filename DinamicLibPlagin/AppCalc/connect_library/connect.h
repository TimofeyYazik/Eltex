#ifndef APPCALC_CONNECT_H_
#define APPCALC_CONNECT_H_

#include "../castom_type.h"

int ConnectLib(int (***func_calc_int)(int, int), handler_t *handler, int *count, const service_info service);

#endif  // APPCALC_CONNECT_H_