/* 152042 松下 旦 */

#include <stdio.h>
#include <math.h>
#define INPUT_NUM 100
#define ROW 10
#define COL 10

double average(double input[INPUT_NUM]);
double variance(double input[INPUT_NUM]);
double standard_deviation(double input[INPUT_NUM]);
void score(double input[INPUT_NUM], double *score_arr);
void print_arr(double *arr);


int main(void){
  int i;
  double x[INPUT_NUM], score_arr[INPUT_NUM];

  for(i=0; i<INPUT_NUM; i++){
    scanf("%lf", &x[i]);
  }

  //printf("分散は: %f \n標準偏差は: %f \nです。\n", variance(x), standard_deviation(x));
  score(x, score_arr);
  print_arr(score_arr);
  return 0;
}


// 平均を求める
double average(double input[INPUT_NUM]){
  int i;
  double sum;
  sum = 0;

  for(i=0; i<INPUT_NUM; i++){
    sum += input[i];
  }
  return sum / (double) INPUT_NUM;
}


// 分散を求める
double variance(double input[INPUT_NUM]){
  int i;
  double distance_sum, av;
  distance_sum = 0;
  av = average(input);

  for(i=0; i<INPUT_NUM; i++){
    distance_sum += pow(input[i] - av, 2);
  }
  return distance_sum / (double) INPUT_NUM;
}


// 標準偏差を求める
double standard_deviation(double input[INPUT_NUM]){
  return sqrt(variance(input));
}


// 標準得点を求める
void score(double input[INPUT_NUM], double *score_arr){
  int i;
  double av, sd;
  av = average(input);
  sd = standard_deviation(input);

  for(i=0; i<INPUT_NUM; i++){
    scanf("%lf", &input[i]);
    score_arr[i] = (input[i] - av) / sd;
  }
}


// 配列を行列形式でprintf
void print_arr(double *arr){
  int row, col, i=0;

  printf("\n");
  for(row=0; row<ROW; row++){
    for(col=0; col<COL; col++){
      if(i < INPUT_NUM){
        printf("%10f ", arr[row*COL+col]);
      }else{
        goto FINISH;
      }
      i++;
    }
    printf("\n");
  }
  FINISH: printf("\n");
}









