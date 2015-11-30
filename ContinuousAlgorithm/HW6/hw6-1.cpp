/*
Copyright (c) 2015 @myuuuuun
https://github.com/myuuuuun
*/
#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <array>
#include <cmath>
#include <limits>
const double EPS = 1.0e-16;
using namespace std;

template <size_t row, size_t col>
  using array_2d = std::array<std::array<double, col>, row>;


// [-1, 1]の範囲でDE積分
double DE_integration1(double (*integrand)(double), double partition, double lower_b, double upper_b){
  double step = (upper_b - lower_b) * 1.0 / partition;
  double sum = 0;
  double f_inner;
  double point;
  for(int i=0; i<=partition; ++i){
    point = lower_b + step * i;
    f_inner = tanh(M_PI / 2 * sinh(point));
    sum += integrand(f_inner) * cosh(point) / pow(cosh(M_PI*sinh(point)/2), 2);
  }

  return sum * M_PI * step / 2;
}


double func1(double x){
  return x * x;
}


int main(){
  double f;
  cout.precision(std::numeric_limits<double>::max_digits10);

  for(int i=0; i<10; ++i){
    f = DE_integration1(func1, 5*pow(2, i), -5, 5);
    cout << f << endl;
  }
  
  return 0;
}





