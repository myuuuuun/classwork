/* 152042 松下 旦 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define ROW 1
#define COL 12

double average(double *input, int input_size);
double variance(double *input, int input_size);
double standard_deviation(double *input, int input_size);
double covariance(double *input1, double *input2, int input_size);
double coefficient(double *input1, double *input2, int input_size);
void   linear_regression(double *input1, double *input2, int input_size, double *output[2]);
double determination(double *inputX, double *inputY, int input_size);
void   multiple_regression(double *inputX1, double *inputX2, double *inputY, int input_size, double *output[3]);
void   partial_regression(double *inputX1, double *inputX2, double *inputY, int input_size, double *output[2]);
void   standaraized_partial_regression(double *inputX1, double *inputX2, double *inputY, int input_size, double *output[2]);
double multiple_determination(double *inputX1, double *inputX2, double *inputY, int input_size);
void   print_arr(double *arr, int input_size);


int main(void){
  int i, input_size;
  double *inputX1, *inputX2, *inputY, *output3[3], *output2[3], *output2_2[3];

  scanf("%d", &input_size);
  inputX1 = calloc(input_size, sizeof(double));
  inputX2 = calloc(input_size, sizeof(double));
  inputY  = calloc(input_size, sizeof(double));
  if (inputX1 == NULL || inputX2 == NULL || inputY == NULL){
    printf("配列用の領域が確保できませんでした\n");
    exit(-1);
  }

  for(i=0; i<input_size; i++){
    scanf("%lf", &inputY[i]);
    scanf("%lf", &inputX1[i]);
    scanf("%lf", &inputX2[i]);
  }
  
  print_arr(inputY, input_size);
  print_arr(inputX1, input_size);
  print_arr(inputX2, input_size);
  
  //printf("\n\n共分散は：%lfです。\n", covariance(input1, input2, input_size));
  //printf("相関係数は：%lfです。\n\n", coefficient(input1, input2, input_size));
  //linear_regression(input1, input2, input_size, output);
  //printf("\n\n回帰方程式は：Y = %lf + (%lf * X) です。\n", *output[0], *output[1]);
  //printf("決定係数R^2は：%lfです。\n\n", determination(input1, input2, input_size));

  multiple_regression(inputX1, inputX2, inputY, input_size, output3);
  printf("\n重回帰方程式は：Y = %lf + (%lf * X1) + (%lf * X2) です。\n", *output3[0], *output3[1], *output3[2]);

  partial_regression(inputX1, inputX2, inputY, input_size, output2);
  printf("偏回帰係数は：b1 = %lf, b2 = %lf です。\n", *output2[0], *output2[1]);

  standaraized_partial_regression(inputX1, inputX2, inputY, input_size, output2_2);
  printf("標準化偏回帰係数は：sb1 = %lf, sb2 = %lf です。\n", *output2_2[0], *output2_2[1]);

  printf("重決定係数は：R^2 = %lf です。\n\n", multiple_determination(inputX1, inputX2, inputY, input_size));

  free(inputY);
  free(inputX2);
  free(inputX1);

  return 0;
}


// 平均を求める
double average(double *input, int input_size){
  int i;
  double sum;
  sum = 0;

  for(i=0; i<input_size; i++){
    sum += input[i];
  }

  return sum / (double) input_size;
}


// 分散を求める
double variance(double *input, int input_size){
  int i;
  double distance_sum, av;
  distance_sum = 0;
  av = average(input, input_size);

  for(i=0; i<input_size; i++){
    distance_sum += pow(input[i] - av, 2);
  }
  return distance_sum / (double) input_size;
}


// 標準偏差を求める
double standard_deviation(double *input, int input_size){
  return sqrt(variance(input, input_size));
}


// 共分散を求める
double covariance(double *input1, double *input2, int input_size){
  int i;
  double average1, average2, sum=0;

  average1 = average(input1, input_size);
  average2 = average(input2, input_size);

  for(i=0; i<input_size; i++){
    sum += (input1[i] - average1) * (input2[i] - average2);
  }

  return (sum / (double) input_size);
}


// 相関係数を求める
double coefficient(double *input1, double *input2, int input_size){
  return (covariance(input1, input2, input_size)) / (standard_deviation(input1, input_size) * standard_deviation(input2, input_size));
}


// 最小二乗法から回帰方程式を求める
void linear_regression(double *inputX, double *inputY, int input_size, double *output[2]){
  double a, b; // Y = a + bX

  b = covariance(inputX, inputY, input_size) / variance(inputX, input_size);
  a = average(inputY, input_size) - b * average(inputX, input_size);

  output[0] = &a;
  output[1] = &b;
}


// 決定係数を求める
double determination(double *input1, double *input2, int input_size){
  return pow(covariance(input1, input2, input_size) / (standard_deviation(input1, input_size) * standard_deviation(input2, input_size)), 2);
}


// 重回帰方程式を求める
void multiple_regression(double *inputX1, double *inputX2, double *inputY, int input_size, double *output[3]){
  double a, b1, b2, co1Y, co2Y, co12, av1, av2, avY, s1, s2, sY;

  co1Y = coefficient(inputX1, inputY, input_size);
  co2Y = coefficient(inputX2, inputY, input_size);
  co12 = coefficient(inputX1, inputX2, input_size);
  av1  = average(inputX1, input_size);
  av2  = average(inputX2, input_size);
  avY  = average(inputY, input_size);
  s1   = standard_deviation(inputX1, input_size);
  s2   = standard_deviation(inputX2, input_size);
  sY   = standard_deviation(inputY, input_size);

  b1 = ((co1Y - co2Y * co12) * sY) / ((1 - pow(co12, 2)) * s1);
  b2 = ((co2Y - co1Y * co12) * sY) / ((1 - pow(co12, 2)) * s2);
  a  = avY - b1 * av1 - b2 * av2;

  output[0] = &a;
  output[1] = &b1;
  output[2] = &b2;
}


// 偏回帰係数を求める
void partial_regression(double *inputX1, double *inputX2, double *inputY, int input_size, double *output[2]){
  double *mr[3];

  multiple_regression(inputX1, inputX2, inputY, input_size, mr);
  output[0] = mr[1];
  output[1] = mr[2];
}


// 標準化偏回帰係数を求める
void standaraized_partial_regression(double *inputX1, double *inputX2, double *inputY, int input_size, double *output[2]){
  double *pr[2], s1, s2, sY;

  s1 = standard_deviation(inputX1, input_size);
  s2 = standard_deviation(inputX2, input_size);
  sY = standard_deviation(inputY, input_size);
  partial_regression(inputX1, inputX2, inputY, input_size, pr);
  *pr[0] = *pr[0] * s1 / sY;
  *pr[1] = *pr[1] * s2 / sY;
  output[0] = pr[0];
  output[1] = pr[1];
}


// 重決定係数を求める
double multiple_determination(double *inputX1, double *inputX2, double *inputY, int input_size){
  double *spr[2], r1Y, r2Y, sb1, sb2;

  standaraized_partial_regression(inputX1, inputX2, inputY, input_size, spr);
  r1Y = coefficient(inputX1, inputY, input_size);
  r2Y = coefficient(inputX2, inputY, input_size);
  sb1 = *spr[0];
  sb2 = *spr[1];

  return r1Y * sb1 + r2Y * sb2;
}


// 配列を行列形式でprintf
void print_arr(double *arr, int input_size){
  int row, col, i=0;

  printf("\n");
  for(row=0; row<ROW; row++){
    for(col=0; col<COL; col++){
      if(i < input_size){
        printf("%10f ", arr[row*COL+col]);
      }else{
        goto FINISH;
      }
      i++;
    }
    printf("\n");
  }
  FINISH: printf("");
}









