#ifndef STATIC_LIB_CALCLIB_INT_H_
#define STATIC_LIB_CALCLIB_INT_H_

int g_number_functions = 4;

//-a realiaze

char g_name_func[4][30] = {
  "-n Add -t int",
  "-n Sub -t int",
  "-n Mul -t int",
  "-n Div -t int",
  };

int Add(int a, int b);
int Sub(int a, int b);
int Mul(int a, int b);
int Div(int a, int b);

#endif // STATIC_LIB_CALCLIB_INT_H_