/*
Copyright (c) 2015 @myuuuuun
https://github.com/myuuuuun
*/
#include <iostream>
#include <complex>
#include <array>
#include <vector>
#include <cmath>
#include <limits>
const double EPS = 1.0e-12;
const double PI = 3.1415926535;
using namespace std;
using vector2d = vector<vector<double>>;


// 汎用taylor 展開テンプレート
// x=point 周りで taylor 展開し、x:=value とする
// 各項の絶対値が単調に減少するケースを想定
double taylor(double (*taylor_func)(int, double, double), double point, double value){
  double t_sum = 0;
  double section_value;

  // 最大101 項まで計算する
  // 項値がEPS よりも小さい場合は、その時点で打ち切る
  // 情報落ち防止のため、先に何項まで展開すべきか計算
  int sec_max = 100;
  array<double, 101> sec_val_list;
  for(int i=0; i<=100; i++){
    section_value = taylor_func(i, point, value);
    sec_val_list[i] = section_value;
    if(abs(section_value) < EPS || section_value == std::numeric_limits<double>::infinity()){
      sec_max = i;
      break;
    }
  }

  // 情報落ち防止のため、後ろの項から足す
  for(int i=sec_max; i>=0; --i){
    t_sum += sec_val_list[i];
  }

  return t_sum;
}


// exp(value) を0 周りでtaylor 展開し求める
double taylor_exp(double value){
  double t_sum = 0;

  int sec_max = 100;
  double section_value;
  array<double, 101> sec_val_list;
  sec_val_list[0] = 1.0;

  for(int i=1; i<=100; i++){
    // exp のtaylor 展開の性質を利用
    section_value = sec_val_list[i-1] * value / i;
    sec_val_list[i] = section_value;
    if(abs(section_value) < EPS || section_value == std::numeric_limits<double>::infinity()){
      sec_max = i;
      break;
    }
  }

  for(int i=sec_max; i>=0; --i){
    t_sum += sec_val_list[i];
  }

  return t_sum;
}


// log(value) を1 周りでtaylor 展開し求める
// あまりvalue が1 から離れると精度が無い
double taylor_log(double value){
  double t_sum = 0;

  int sec_max = 100;
  double section_value;
  array<double, 101> sec_val_list;

  // 分子
  double numer = 1;

  for(int i=0; i<=100; i++){
    // log のtaylor 展開の性質を利用
    numer *= value - 1;
    section_value = pow(-1.0, i%2) * numer / (i+1.0);
    sec_val_list[i] = section_value;
    if(abs(section_value) < EPS || section_value == std::numeric_limits<double>::infinity()){
      sec_max = i;
      break;
    }
  }

  for(int i=sec_max; i>=0; --i){
    t_sum += sec_val_list[i];
  }

  return t_sum;
}


// Horner 法でべき表現多項式（a_0 + a_1 x + a_2 x^2 + ... ）の値を求める
double horner(double value, vector<double> coefs){
  double sum = 0;
  for(vector<double>::reverse_iterator it = coefs.rbegin(); it != coefs.rend(); ++it){
    sum = sum * value + (*it);
  }

  return sum;
}


// ネヴィルの算法
double neville(double x, vector2d points){
  int size = points[0].size();
  vector2d table(size*size, vector<double>(size));
  
  for(int i=0; i<size; i++){
    table[i][0] = points[i][1];
    for(int j=1; j<i+1; j++){
      table[i][j] = ((points[i][0] - x) * table[i-1][j-1] - (points[i-j][0] - x) * table[i][j-1]) / (points[i][0] - points[i-j][0]);
    }
  }

  return table[size-1][size-1];
}


vector2d make_points_horner(vector<double> x_list, vector<double> coefs){
  int size = x_list.size();
  vector2d points(size*2, vector<double>(size));
  
  for(int i=0; i<size; i++){
    points[i][0] = x_list[i];
    points[i][1] = horner(x_list[i], coefs);
  }

  return points;
}


int main(){
  vector<double> x_list;
  vector<double> coefs;

  // x_list を定義
  for(int i=0; i<6; i++){
    x_list.push_back(0.5 + 0.1 * i);
  }

  // 1+x+x^2
  for(int i=0; i<3; i++){
    coefs.push_back(1);
  }
  vector2d points = make_points_horner(x_list, coefs);
  cout << "近似値" << neville(0.75, points)
    << ", 真値との差" << abs(horner(0.75, coefs) - neville(0.75, points)) << endl;

  // +x^3
  coefs.push_back(1);
  points = make_points_horner(x_list, coefs);
  cout << "近似値" << neville(0.75, points)
    << ", 真値との差" << abs(horner(0.75, coefs) - neville(0.75, points)) << endl;

  // +x^4
  coefs.push_back(1);
  points = make_points_horner(x_list, coefs);
  cout << "近似値" << neville(0.75, points)
    << ", 真値との差" << abs(horner(0.75, coefs) - neville(0.75, points)) << endl;

  // +x^5
  coefs.push_back(1);
  points = make_points_horner(x_list, coefs);
  cout << "近似値" << neville(0.75, points)
    << ", 真値との差" << abs(horner(0.75, coefs) - neville(0.75, points)) << endl;

  // +x^6
  coefs.push_back(1);
  points = make_points_horner(x_list, coefs);
  cout << "近似値" << neville(0.75, points)
    << ", 真値との差" << abs(horner(0.75, coefs) - neville(0.75, points)) << endl;

  // +x^7
  coefs.push_back(1);
  points = make_points_horner(x_list, coefs);
  cout << "近似値" << neville(0.75, points)
    << ", 真値との差" << abs(horner(0.75, coefs) - neville(0.75, points)) << endl;

  // e^x
  vector2d points2(6*2, vector<double>(6));
  for(int i=0; i<7; i++){
    points2[i][0] = x_list[i];
    points2[i][1] = taylor_exp(x_list[i]);
  }
  cout << "近似値" << neville(0.75, points2)
    << ", 真値との差" << abs(exp(0.75) - neville(0.75, points2)) << endl;

  // logx
  for(int i=0; i<7; i++){
    points2[i][0] = x_list[i];
    points2[i][1] = taylor_log(x_list[i]);
  }
  cout << "近似値" << neville(0.75, points2)
    << ", 真値との差" << abs(log(0.75) - neville(0.75, points2)) << endl;

  
  return 0;
}





