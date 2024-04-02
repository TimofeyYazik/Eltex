#ifndef STATIC_LIB_CALCLIB_FLOAT_H_
#define STATIC_LIB_CALCLIB_FLOAT_H_

int g_number_functions = 4;

//-a realiaze

char g_name_func[4][30] = {
  "-n Add -t float",
  "-n Sub -t float",
  "-n Mul -t float",
  "-n Div -t float",
  };

float Add(float a, float b);
float Sub(float a, float b);
float Mul(float a, float b);
float Div(float a, float b);

#endif // STATIC_LIB_CALCLIB_FLOAT_H_