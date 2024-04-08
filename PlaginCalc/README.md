### Руководство пользователя
Что бы использовать калькулятор при исполнении программы через флаг -p(path) 
вы должны указать путь до динамической библиотеке и через флаг -f(function) указать
функции которые хотите достать из библиотеки.

Так же если указать второй флаг -p то с этого момента все последующие функции указаные в флаге -f будут 
использоваться конкретно к этой библиотеки

Количество библиотек и функций не ограничено

#### примеры:
./AppCalc -p../libcalc1.so -fAdd -p../libcalc2.so -fMul 
./AppCalc -p../libcalc1.so -fAdd -fSub -p../libcalc2.so -fMul 
./AppCalc -p../libcalc1.so -fAdd -p../libcalc2.so -fMul -fDiv
./AppCalc -p../libcalc1.so -fAdd -fSub -p../libcalc2.so -fMul -fDiv

### Нереализованно
Не смог решить проблему с доступом к матрице из-за чего не могу доставть функции без их названия
веденых пользователем

int g_number_functions = 2;

char g_name_func[2][5] = {
  "Div",
  "Mul"
};

// int *count_func = dlsym(handler->handler_arr[i], "g_number_functions"); //это число ддостаеться без проблем
// if(count_func == NULL){
//   fprintf(stderr, "var not found");
//   exit(EXIT_FAILURE);
// }
// char ***func_name = dlsym(handler->handler_arr[i], "g_name_func"); //также ошибки не происходит
// if(func_name == NULL){
//   fprintf(stderr, "var not found");
//   exit(EXIT_FAILURE);
// } 
// printf("%s\n", (*func_name)[0]); //но при попытке обратиться просиходит seg fault
