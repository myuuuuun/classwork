#include <stdio.h>
#include <math.h>
#define EPS 1.0e-16
#define MAXLOOP 1000

double myexp(double x);
double taylor(double x, double k, double x_k, double fact, double sum);

int main(){
  double x;

  printf("Enter a real number\n");
  printf("or enter q to quit.\n");
  while (scanf("%lf", &x) == 1){
    printf("Exp(%lf) = %.15e (%.15e)\n", x, myexp(x), exp(x));
  }

  return 0;
}

// e^x乗の計算 - xの正負で場合分け
double myexp(double x) {
  return x >= 0 ? taylor(x, 1, 1, 1, 0) : 1 / taylor(-x, 1, 1, 1, 0); //xが負の値なら、逆数を取る
}

// テーラー展開部
// e^x= Σ{x^(k-1) / (k-1)!}
// x: 定数, k: 現在何項目か, x_k: k項目の分子, fact: k項目の分母, sum: k項目までの合計
double taylor(double x, double k, double x_k, double fact, double sum) {
  double sum_b = sum;
  sum += x_k / fact;
  if (fabs(sum - sum_b) < EPS || k >= MAXLOOP){
    return sum;
  }
  //printf("%.15e \n", sum);
  return taylor(x, k+1, x_k * x, fact * k, sum);
}














