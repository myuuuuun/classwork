/*
Copyright (c) 2015 @myuuuuun
https://github.com/myuuuuun
*/
#include <iostream>
#include <array>
#include <cmath>
#include <limits>
#define PI 3.1415926535
using namespace std;


template <class T> array<T, 2> quadratic(T a, T b, T c);
template <class T>
  T compute_clash_time(array<T, 3> circle, T radius, T gravity);


int main(){
  double radius = 1.0;
  double clash_time;
  array<double, 3> circle={0, 0, 6};
  cout.precision(numeric_limits<double>::max_digits10);
  clash_time = compute_clash_time(circle, -1.0, 1.0);
  cout << clash_time << endl;

  return 0;
}


// ax^2 + bx + c = 0 の形の2次方程式を解く 
template <class T>
array<T, 2> quadratic(T a, T b, T c){
  T x1, x2;
  T d;
  array<T, 2> rst;

  // 判別式D < 0ならnanを返す 
  d = sqrt(pow(b, 2) - 4 * a * c);
  
  if(isnan(d)){
    rst[0] = numeric_limits<T>::quiet_NaN();
    rst[1] = numeric_limits<T>::quiet_NaN();
    return rst;
  }

  // 桁落ち防止
  if(b < 0){
    x1 = (-1 * b + d) / (2 * a);
  }
  else{
    x1 = (-1 * b - d) / (2 * a);
  }

  // 解と係数の関係
  x2 = c / (a * x1);
  rst[0] = x1;
  rst[1] = x2;

  return rst;
}


// 重力は下に落ちる場合、マイナスに設定する
template <class T>
T compute_clash_time(array<T, 3> circle, T radius, T gravity){
  T x, y, z, c;
  T clash_time;
  
  x = circle[0];
  y = circle[1];
  z = circle[2];
  c = cos(sqrt(x*x+y*y));

  if(x*x+y*y+pow(z-c, 2) < radius * radius){
    return numeric_limits<T>::quiet_NaN();
  }

  array<T, 2> time_list =
    quadratic(gravity*gravity, -2*gravity*(z-c), x*x+y*y+pow(z-c, 2)-radius*radius);

  if( isnan(time_list[0]) || (time_list[0] < 0 && time_list[1] < 0) ){
    clash_time = numeric_limits<T>::quiet_NaN();
  }
  else{
    if(time_list[0] < 0 || time_list[1] < time_list[0]){
      clash_time = time_list[1];
    }
    else{
      clash_time = time_list[0];
    }
  }

  return clash_time;
}

