#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double average(double *input, int input_num);
double variance(double *input, int input_num);
double standard_deviation(double *input, int input_num);

int main(){

  double *a;
  int input_num, i;

  printf("入力する実数の合計数を入力してください：");
  scanf("%d", &input_num);
  
  a = calloc(input_num+1, sizeof(double) );
  if (a == NULL) {
    printf("配列用の領域が確保できませんでした\n");
    exit(-1);
  }

  for (i = 0; i < input_num; i++) {
    printf("%d番目の実数を入力してください：", i+1);
    scanf("%lf", &a[i]);
  }

  printf("標準偏差は: %f \nです。\n", standard_deviation(a, input_num));
  free(a);

  return 0;
}


// 平均を求める
double average(double *input, int input_num){
  int i;
  double sum;
  sum = 0;

  for(i=0; i<input_num; i++){
    sum += input[i];
  }
  return sum / (double) input_num;
}


// 分散を求める
double variance(double *input, int input_num){
  int i;
  double distance_sum, av;
  distance_sum = 0;
  av = average(input, input_num);

  for(i=0; i<input_num; i++){
    distance_sum += pow(input[i] - av, 2);
  }
  return distance_sum / (double) input_num;
}


// 標準偏差を求める
double standard_deviation(double *input, int input_num){
  return sqrt(variance(input, input_num));
}







