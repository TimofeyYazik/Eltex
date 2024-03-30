#ifndef DINAMIC_MAS_ADDITIONAL_FUNCTIONS_H_
#define DINAMIC_MAS_ADDITIONAL_FUNCTIONS_H_

#include "castom_type.h"

void CopyAbonent(Abonent *receiver, const Abonent *sender);
void CheckErrorMemory(AbonentMas mas);
void CheckErrorScanf(int real_result, int expected_result);
void ClearStdin();

#endif // DINAMIC_MAS_ADDITIONAL_FUNCTIONS_H_