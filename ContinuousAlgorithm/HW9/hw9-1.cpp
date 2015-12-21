/*
Copyright (c) 2015 @myuuuuun
https://github.com/myuuuuun
*/
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
#include <iomanip>
#include <array>
#include <cmath>
const double EPS = 1.0e-12;
using namespace std;

template <size_t row, size_t col>
  using array_2d = std::array<std::array<double, col>, row>;


// 切片, 傾きの順で値を返す
template <size_t size> 
std::array<double, 2> least_squared_2d(std::array<double, size> x_list, std::array<double, size> y_list){
  double x_sum=0, x2_sum=0, y_sum=0, xy_sum=0;
  for(int i=0; i<size; i++){
    x_sum += x_list[i];
    x2_sum += x_list[i]*x_list[i];
    y_sum += y_list[i];
    xy_sum += x_list[i] * y_list[i];
  }

  std::array<double, 2> result;
  double denom = size * x2_sum - pow(x_sum, 2);
  result[0] = (x2_sum*y_sum - x_sum*xy_sum) / denom;
  result[1] = (size*xy_sum - x_sum*y_sum) / denom;
  return result;
}


int main(void)
{
  FILE *fp;
  int ret;
  std::array<double, 201> x_list, y_list;

  fp = fopen("data.csv", "r");
  for(int i=0; i<201; i++){
    fscanf(fp, "%lf, %lf", &x_list[i], &y_list[i]);
  }
  fclose(fp);

  std::array<double, 2> result;
  result = least_squared_2d(x_list, y_list);

  cout << result[0] << ", " << result[1] << endl;
  return 0;
}




