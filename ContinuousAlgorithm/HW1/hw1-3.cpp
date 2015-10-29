#include <iostream>
#include <array>
#include <cmath>
#include <limits>
#define PI 3.1415926535
using namespace std;

array<double, 4> f_to_d(array<float, 4> f_array);
template <class Type> array<Type, 2> quadratic(Type a, Type b, Type c);
template <class Type>
  Type compute_clash_time(array<Type, 4> circle1, array<Type, 4> circle2, Type radius);


int main(){
  Vector<double, 2> vector1, vector2;
  vector1[0] = 1;
  vector1[1] = 1;
  vector2[0] = 1;
  vector2[1] = 1;

  vector1 = vector1 + vector2;
  cout << vector1[0] << ", " << vector1[1] << ", " << vector2[0] << ", " << vector2[1] << endl;

  return 0;
}


// ax^2 + bx + c = 0 の形の2次方程式を解く 
template <class Type>
array<Type, 2> quadratic(Type a, Type b, Type c){
  Type x1, x2;
  Type d;
  array<Type, 2> rst;

  // 判別式D < 0ならnanを返す 
  d = sqrt(pow(b, 2) - 4 * a * c);
  
  //cout << b * b << ", " << 4 * a * c << endl;
  
  if(isnan(d)){
    rst[0] = numeric_limits<Type>::quiet_NaN();
    rst[1] = numeric_limits<Type>::quiet_NaN();
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
template <class Type>
Type compute_clash_time(array<Type, 4> circle1, array<Type, 4> circle2, Type radius){
  Type x, y, vx, vy;
  Type clash_time;
  
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
    array<Type, 2> time_list = quadratic(vx*vx+vy*vy, 2*(x*vx+y*vy), x*x+y*y-4*radius*radius);

    if( isnan(time_list[0]) || (time_list[0] < 0 && time_list[1] < 0) ){
      clash_time = numeric_limits<Type>::quiet_NaN();
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


// 衝突時刻と2円が衝突した際の位置と、反発後の速度を計算
// circle1, 2: [初期位置x座標, 初期位置y座標, x方向速度, y方向速度]
template <class Type>
Type compute_clash(array<Type, 4> circle1, array<Type, 4> circle2, Type radius){
  Type x, y, vx, vy;
  // 衝突時刻
  Type clash_time = compute_clash_time(circle1, circle2, radius);

  // 衝突時のcircle1, 2のx座標, y座標
  Type clash_circle1_x = circle1[0] + circle1[2] * clash_time;
  Type clash_circle1_y = circle1[0] + circle1[2] * clash_time;
  Type clash_circle2_x = circle2[0] + circle2[2] * clash_time;
  Type clash_circle2_y = circle2[0] + circle2[2] * clash_time;

  // 衝突地点の座標
  Type clash_x = (clash_circle1_x + clash_circle2_x) / 2;
  Type clash_y = (clash_circle1_y + clash_circle2_y) / 2;

  // 衝突地点からcircle1, 2の重心方向へ伸びるベクトル
  Type c1_to_g_x = clash_circle1_x - clash_x;
  Type c1_to_g_y = clash_circle1_y - clash_y;
  Type c2_to_g_x = clash_circle2_x - clash_x;
  Type c2_to_g_y = clash_circle2_y - clash_y;

  // circle1の速度ベクトルを, 衝突地点からcircle2の重心方向へ伸びるベクトルに射影
  Type move_vx = 2;

}


// use in main()
array<double, 4> f_to_d(array<float, 4> f_array){
  array<double, 4> d_array;
  for(int i=0; i<4; i++){
    d_array[i] = f_array[i];
  }
  return d_array;
}