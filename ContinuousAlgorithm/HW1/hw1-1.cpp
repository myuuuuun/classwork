#include <iostream>
#include <array>
#include <cmath>
#include <limits>
using namespace std;


// ax^2 + bx + c = 0 の形の2次方程式を解く 
template <class T>
array<T, 2> quadratic(T a, T b, T c){
  T x1, x2;
  T d;
  array<T, 2> rst;

  // 判別式D < 0ならnanを返す
  d = pow(b, 2) - 4 * a * c;
  if(d < 0){
    rst[0] = numeric_limits<T>::quiet_NaN();
    rst[1] = numeric_limits<T>::quiet_NaN();
    return rst;
  }

  // 桁落ち防止
  if(b < 0){
    x1 = (-1 * b + sqrt(d)) / (2 * a);
  }
  else{
    x1 = (-1 * b - sqrt(d)) / (2 * a);
  }

  x2 = c / (a * x1);
  rst[0] = x1;
  rst[1] = x2;

  return rst;
}


int main(){
  double r, x1, y1, x2, y2, vx1, vy1, vx2, vy2;
  double x, y, vx, vy;

  r = 1.0;

  x1 = 10;
  y1 = 30;
  vx1 = 1.0;
  vy1 = 1.0;

  x2 = 10;
  y2 = 0;
  vx2 = -0.1;
  vy2 = 0;

  x = x1 - x2;
  y = y1 - y2;
  vx = vx1 - vx2;
  vy = vy1 - vy2;

  array<float, 2> clash_time_f = quadratic((float)(vx*vx+vy*vy), (float)(2*(x*vx+y*vy)), (float)(x*x+y*y-4*r*r));
  array<double, 2> clash_time_d = quadratic(vx*vx+vy*vy, 2*(x*vx+y*vy), x*x+y*y-4*r*r);

  cout << "x1, y1, vx1, vy1: " << x1 << ", " << y1 << ", " << vx1 << ", " << vy1 << endl;
  cout << "x2, y2, vx2, vy2: " << x2 << ", " << y2 << ", " << vx2 << ", " << vy2 << endl;

  if(isnan(clash_time_d[0])){
    cout << "clash time: " << "NaN" << endl;
  }
  else if(clash_time_d[0] < 0 && clash_time_d[1] < 0){
    cout << "clash time: " << "NaN" << endl;
  }
  else{
    if(clash_time_d[0] < 0 || clash_time_d[1] < clash_time_d[0]){
      cout << "clash time(double): " << clash_time_d[1] << "clash time(float): " << clash_time_f[1] << endl;
    }
    else{
      cout << "clash time(double): " << clash_time_d[0] << "clash time(float): " << clash_time_f[0] << endl;
    }
  }

  return 0;
}
