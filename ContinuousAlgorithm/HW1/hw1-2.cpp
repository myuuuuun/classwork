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

array<double, 4> f_to_d(array<float, 4> f_array);
template <class T> array<T, 2> quadratic(T a, T b, T c);
template <class T>
  T compute_clash_time(array<T, 4> circle1, array<T, 4> circle2, T radius);


int main(){
  float radius = 1.0;
  float distance = 10.0;
  float rad, cos_rad, sin_rad, clash_time_f;
  double clash_time_d;
  array<float, 4> circle1, circle2;

  cout.precision(numeric_limits<float>::max_digits10);
  for(int deg=0; deg<=180; deg++){
    rad = deg * PI / 180;
    cos_rad = cos(rad / 2);
    sin_rad = sin(rad / 2);

    circle1[0] = 10 * sin_rad + 1;
    circle1[1] = 10 * cos_rad;
    circle1[2] = -10 * sin_rad;
    circle1[3] = -10 * cos_rad;

    circle2[0] = -1 * circle1[0];
    circle2[1] = -1 * circle1[1];
    circle2[2] = -1 * circle1[2];
    circle2[3] = -1 * circle1[3];

    array<double, 4> circle1_d, circle2_d;
    circle1_d = f_to_d(circle1);
    circle2_d = f_to_d(circle2);

    clash_time_f = compute_clash_time(circle1, circle2, radius);
    clash_time_d = compute_clash_time(circle1_d, circle2_d, (double)radius);
    cout << deg << ", " << abs(clash_time_d - clash_time_f) << endl;
  }
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
  
  //cout << b * b << ", " << 4 * a * c << endl;
  
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


// 2円の半径, 初期位置, 速度（等速）から衝突時刻（最初の1回）を計算
// circle1, 2: [初期位置x座標, 初期位置y座標, x方向速度, y方向速度]
template <class T>
T compute_clash_time(array<T, 4> circle1, array<T, 4> circle2, T radius){
  T x, y, vx, vy;
  T clash_time;
  
  x = circle1[0] - circle2[0];
  y = circle1[1] - circle2[1];
  vx = circle1[2] - circle2[2];
  vy = circle1[3] - circle2[3];

  cout << x << ", " << y << ", " << vx << ", " << vy << ", "<< endl;
  cout << pow(x * vy - y * vx, 2) << ", " << 4 * radius * radius * (vx * vx + vy * vy) << endl;

  if(x*x + y*y <= 4*radius*radius){
    clash_time = 0;
  }
  else{
    array<T, 2> time_list = quadratic(vx*vx+vy*vy, 2*(x*vx+y*vy), x*x+y*y-4*radius*radius);

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
  }

  return clash_time;
}


// use in main()
array<double, 4> f_to_d(array<float, 4> f_array){
  array<double, 4> d_array;
  for(int i=0; i<4; i++){
    d_array[i] = f_array[i];
  }
  return d_array;
}


