/*
Copyright (c) 2015 @myuuuuun
https://github.com/myuuuuun
*/
#include <iostream>
#include <array>
#include <cmath>
#include <limits>
const double EPS = 1.0e-8;
const double PI = 3.1415926535;
using namespace std;

// 自動微分
template <class Type> class Dual
{

public:
  Type value, diff;
  Dual();
  Dual(Type v);
  Dual(Type v, Type d);
  Type get_value();
  Type get_diff();
  void set_value(Type v);
  void set_diff(Type d);
  
  template <class SType> Dual<Type>& operator=(SType x);
  Dual<Type>& operator=(const Dual<Type> &d1);

  template <class SType> bool operator==(const SType &x);
  bool operator==(const Dual<Type> &d1);
  template <class SType> bool operator!=(const SType &x);
  bool operator!=(const Dual<Type> &d1);

  template <class SType> Dual<Type> operator+(const SType &x);
  template <class SType> Dual<Type> operator-(const SType &x);
  template <class SType> Dual<Type> operator*(const SType &x);
  template <class SType> Dual<Type> operator/(const SType &x);
  template <class SType> Dual<Type>& operator+=(const SType &x);
  template <class SType> Dual<Type>& operator-=(const SType &x);
  template <class SType> Dual<Type>& operator*=(const SType &x);
  template <class SType> Dual<Type>& operator/=(const SType &x);

  Dual<Type> operator+(const Dual<Type> &d1);
  Dual<Type> operator-(const Dual<Type> &d1);
  Dual<Type> operator*(const Dual<Type> &d1);
  Dual<Type> operator/(const Dual<Type> &d1);
  Dual<Type>& operator+=(const Dual<Type> &d1);
  Dual<Type>& operator-=(const Dual<Type> &d1);
  Dual<Type>& operator*=(const Dual<Type> &d1);
  Dual<Type>& operator/=(const Dual<Type> &d1);


};


// コンストラクタ
template <class Type> Dual<Type>::Dual(){value = 0, diff = 0;}
template <class Type> Dual<Type>::Dual(Type v){value = v, diff = 0;}
template <class Type> Dual<Type>::Dual(Type v, Type d){value = v, diff = d;}


// 値の取得
template <class Type> Type Dual<Type>::get_value(){return value;};
template <class Type> Type Dual<Type>::get_diff(){return diff;};

// 値の設定
template <class Type> void Dual<Type>::set_value(Type v){value = v;};
template <class Type> void Dual<Type>::set_diff(Type d){diff = d;};

// print
template <class Type> void print(Dual<Type> dual){
  cout << "Value: " << dual.value << ", Diff: " << dual.diff << endl;
}


// 代入
// 定数を代入
template <class Type> template <class SType>
Dual<Type>& Dual<Type>::operator=(SType x){
  value = x;
  diff = 0;
  return *this;
}

// Dualを代入
template <class Type>
Dual<Type>& Dual<Type>::operator=(const Dual<Type> &d1){
  value = d1.value;
  diff = d1.diff;
  return *this;
}


// 比較
// Dualと定数を比較
template <class Type> template <class SType>
bool Dual<Type>::operator==(const SType &x){
  if(abs(value - x) && abs(diff) < EPS){
    return true;
  }
  return false;
}

template <class Type> template <class SType>
bool Dual<Type>::operator!=(const SType &x){
  return !(this == x);
}

// 定数とDualを比較
// ここかなりまずい（なんでも演算が出来てしまう）が、後で直す
template <class SType, class DType>
bool operator==(const SType &x, const DType &d1){
  return d1 == x;
}

template <class SType, class Type>
bool operator!=(const SType &x, const Dual<Type> &d1){
  return d1 != x;
}

// Dual同士を比較
template <class Type>
bool Dual<Type>::operator==(const Dual<Type> &d1){
  if(abs(value - d1.value) < EPS && abs(diff - d1.diff) < EPS){
    return true;
  }
  return false;
}

template <class Type>
bool Dual<Type>::operator!=(const Dual<Type> &d1){
  return !(this == d1);
}


// 演算集
// Dualと一般型（数値を想定）の演算
template <class Type> template <class SType>
Dual<Type> Dual<Type>::operator+(const SType &x){
  Dual<Type> d2;
  d2.value = value + x;
  d2.diff = diff;
  return d2;
}

template <class Type> template <class SType>
Dual<Type> Dual<Type>::operator-(const SType &x){
  Dual<Type> d2;
  d2.value = value - x;
  d2.diff = diff;
  return d2;
}

template <class Type> template <class SType>
Dual<Type> Dual<Type>::operator*(const SType &x){
  Dual<Type> d2;
  d2.value = value * x;
  d2.diff =  diff * x;
  return d2;
}

template <class Type> template <class SType>
Dual<Type> Dual<Type>::operator/(const SType &x){
  Dual<Type> d2;
  d2.value = value / x;
  d2.diff =  diff / x;
  return d2;
}

template <class Type> template <class SType>
Dual<Type>& Dual<Type>::operator+=(const SType &x){
  value += x;
  return *this;
}

template <class Type> template <class SType>
Dual<Type>& Dual<Type>::operator-=(const SType &x){
  value -= x;
  return *this;
}

template <class Type> template <class SType>
Dual<Type>& Dual<Type>::operator*=(const SType &x){
  value *= x;
  diff *= x;
  return *this;
}

template <class Type> template <class SType>
Dual<Type>& Dual<Type>::operator/=(const SType &x){
  value /= x;
  diff /= x;
  return *this;
}


// 一般型（数値を想定）とDualの演算
// ここかなりまずい（なんでも演算が出来てしまう）が、後で直す
template <class SType, class DType>
DType operator+(const SType &x, const DType &d1){
  DType d2;
  d2.value = x + d1.value;
  d2.diff = d1.diff;
  return d2;
}

template <class SType, class DType>
DType operator-(const SType &x, const DType &d1){
  DType d2;
  d2.value = x - d1.value;
  d2.diff = d1.diff;
  return d2;
}

template <class SType, class DType>
DType operator*(const SType &x, const DType &d1){
  DType d2;
  d2.value = x * d1.value;
  d2.diff = x * d1.diff;
  return d2;
}

template <class SType, class DType>
DType operator/(const SType &x, const DType &d1){
  DType d2;
  d2.value = x / d1.value;
  d2.diff = -1 * x * d1.diff / d1.value;
  return d2;
}

template <class SType, class DType>
DType& operator+=(const SType &x, DType &d1){
  d1.value += x;
  return d1;
}

template <class SType, class DType>
DType& operator-=(const SType &x, DType &d1){
  d1.value -= x;
  return d1;
}

template <class SType, class DType>
DType& operator*=(const SType &x, DType &d1){
  d1.value *= x;
  d1.diff *= x;
  return d1;
}

template <class SType, class DType>
DType& operator/=(const SType &x, DType &d1){
  d1.diff = -1 * x * d1.diff / d1.value;
  d1.value /= x;
  return d1;
}

// Dual同士の演算
template <class Type> Dual<Type> Dual<Type>::operator+(const Dual<Type> &d1){
  Dual<Type> d2(value, diff);
  d2.value += d1.value;
  d2.diff += d1.diff;
  return d2;
}

template <class Type> Dual<Type> Dual<Type>::operator-(const Dual<Type> &d1){
  Dual<Type> d2(value, diff);
  d2.value -= d1.value;
  d2.diff -= d1.diff;
  return d2;
}

template <class Type> Dual<Type> Dual<Type>::operator*(const Dual<Type> &d1){
  Dual<Type> d2(value, diff);
  d2.value *= d1.value;
  d2.diff = diff * d1.value + value * d1.diff;
  return d2;
}

template <class Type> Dual<Type> Dual<Type>::operator/(const Dual<Type> &d1){
  Dual<Type> d2(value, diff);
  d2.value /= d1.value;
  d2.diff = (diff * d1.value - value * d1.diff) / pow(d1.value, 2);
  return d2;
}

template <class Type> Dual<Type>& Dual<Type>::operator+=(const Dual<Type> &d1){
  value += d1.value;
  diff += d1.diff;
  return *this;
}

template <class Type> Dual<Type>& Dual<Type>::operator-=(const Dual<Type> &d1){
  value -= d1.value;
  diff -= d1.diff;
  return *this;
}

template <class Type> Dual<Type>& Dual<Type>::operator*=(const Dual<Type> &d1){
  Type old_value = value;
  value = old_value * d1.value;
  diff = diff * d1.value + old_value * d1.diff;
  return *this;
}

template <class Type> Dual<Type>& Dual<Type>::operator/=(const Dual<Type> &d1){
  Type old_value = value;
  value = old_value / d1.value;
  diff = (diff * d1.value - old_value * d1.diff) / pow(d1.value, 2);
  return *this;
}

template <class Type> Dual<Type> sin(const Dual<Type> d1){
  Dual<Type> d2;
  d2.value = sin(d1.value);
  d2.diff = cos(d1.value) * d1.diff;
  return d2;
}

template <class Type> Dual<Type> cos(const Dual<Type> d1){
  Dual<Type> d2;
  d2.value = cos(d1.value);
  d2.diff = -1 * sin(d1.value) * d1.diff;
  return d2;
}

template <class Type> Dual<Type> tan(const Dual<Type> d1){
  Dual<Type> d2;
  d2.value = tan(d1.value);
  d2.diff = d1.diff / pow(cos(d1.value), 2);
  return d2;
}

template <class Type> Dual<Type> exp(const Dual<Type> d1){
  Dual<Type> d2;
  d2.value = exp(d1.value);
  d2.diff = d1.diff * d2.value;
  return d2;
}

template <class Type1, class Type2> Dual<Type1> pow(const Dual<Type1> d1, const Dual<Type2> d2){
  Dual<Type1> d3;
  d3.value = pow(d1.value, d2.value);
  d3.diff = d3.value * (d2.diff * log(d1.value) + d2.value * d1.diff / d1.value);
  return d3;
}

template <class Type> Dual<Type> pow(const Dual<Type> d1, const double n){
  Dual<Type> d2;
  d2.value = pow(d1.value, n);
  d2.diff = n * pow(d1.value, n-1) * d1.diff;
  return d2;
}

template <class Type> Dual<Type> pow(const double n, const Dual<Type> d1){
  Dual<Type> d2;
  d2.value = pow(n, d1.value);
  d2.diff = log(n) * d1.diff * d2.value;
  return d2;
}

template <class Type> Dual<Type> sqrt(const Dual<Type> d1){
  return pow(d1, 0.5);
}

template <class Type> Dual<Type> log(const Dual<Type> d1){
  Dual<Type> d2;
  d2.value = log(d1.value);
  d2.diff = d1.diff / d2.value;
  return d2;
}

template <class Type> Type newton(Dual<Type> (*func)(Dual<Type>), Type initial_v, Type epsilon = EPS){
  Type new_v;
  Dual<Type> func_d, initial_d;
  // newton main loop
  while(true){
    initial_d.value = initial_v;
    initial_d.diff = 1;
    func_d = func(initial_d);

    new_v = initial_v - func_d.value / func_d.diff;
    cout << new_v << endl;
    if(abs(initial_v - new_v) < epsilon || isinf(new_v)){
      break;
    };
    initial_v = new_v;
  }

  return new_v;
}

inline Dual<double> f(Dual<double> x){
  return pow(x, 3) - 1;
}


int main(int argc, char *argv[])
{
  Dual<double> rst;
  Dual<double> d1(2.0, 1);
  rst = newton(f, 10.0);
  //rst = f(d1);
  cout << rst.value << endl;
}





