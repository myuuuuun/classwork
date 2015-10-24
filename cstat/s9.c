/* 152042 松下 旦 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define INPUT_NUM 100
#define STATIC_LIST_LENGTH 1000
#define MAX_LOOP 1000


typedef struct{
  int row;
  int col;
  double **element;
  double *base_element;
} matrix;

int mat_alloc(matrix *mat, int row, int col);
void mat_free(matrix *mat);

double average(double *list, int n);
double variance(double *list, int n);
double unbiased_variance(double *list, int n);
double standard_deviation(double *list, int n);
double unbiased_standard_deviation(double *list, int n);

double s_uniform_rv(void);
int bernoulli_rv(double p);
int binomial_rv(double p, int n);

int factorial(int n);
int partial(int n, int k);
int combination(int n, int k);

// 2項分布の分布関数を返す
double binomial_distribution_value(double p, int n, int k);

// 1標本t検定（母分散未知）
// H0: μ = μ0 を検定するための自由度と検定統計量を返す
// Input: list = [標本数, 平均, 分散]
// Output: result_list = [自由度, 検定統計量]
void t_test_statistic(double *list, double ave, double *result_list);

// χ^2検定
// H0: σ = σ0 を検定するための自由度と検定統計量を返す
// Input: list = [標本数, 平均, 分散]
// Output: result_list = [自由度, 検定統計量]
void chi_test_statistic(double *list, double var, double *result_list);

// 2標本t検定（A, B母分散未知 かつ A, Bの母分散は等しい場合）
// H0: μ_A = μ_B を検定するための自由度と検定統計量を返す
// Input: a_list = [aの標本数, aの平均, aの分散], b_list = [bの標本数, bの平均, bの分散]
// Output: result_list = [自由度, 検定統計量]
void two_t_test_statistic(double *a_list, double *b_list, double *result_list);

// F検定（母分散が等しいか検定）
// H0: σ_A = σ_B を検定するための自由度と検定統計量を返す
// Input: a_list = [aの標本数, aの平均, aの分散], b_list = [bの標本数, bの平均, bの分散]
// Output: result_list = [自由度1, 自由度2, 検定統計量]
void f_test_statistic(double *a_list, double *b_list, double *result_list);


int main(void){
  int i, j, n, bin, count, a_n, b_n, flag;
  double rv_dist_list[1001]={0};
  double tv_list[26]={0};
  double data_list[1001]={0};
  double result_list[3]={0};
  double a_list[3], b_list[3];
  double z, chi_025, chi_975, z_025, t_025, f_025, f_975, ave, var, sd, test_statistic, ave0, var0, ave_a, ave_b, var_a, var_b;
  char *is_accept, *is_accept2, *message;

  // 乱数の初期化
  srand(time(0));
  //srand(2042);


  // ----------------------- EX.8-1 ----------------------- //
  // 25匹コイを捕まえたところ、17匹がメス、8匹がオスだった。コイはメスに偏っているといえるか。
  // 有意水準5%で片側二項検定
  /*
  z = binomial_distribution_value(0.5, 25, 8);

  if(z >= 0.05){
    is_accept = "受容";
  }else{
    is_accept = "棄却";
  }

  printf("メスが17匹以上になる確率は %f なので、帰無仮説は%sされます\n", z, is_accept);
  */


  // ----------------------- EX.8-2 ----------------------- /
  // file7.dat
  // 20羽のヒヨコの体重（file7）の平均は、与えられている母平均と等しいか。
  // 有意水準5%の1標本両側t検定
  /*
  t_025 = 2.093;

  scanf("%lf", &ave);
  scanf("%d", &n);

  // 入力データを配列にいれる
  for(i=0; i<n; i++){
    scanf("%lf", &data_list[i]);
  }

  ave0 = average(data_list, n);
  var0 = unbiased_variance(data_list, n);
  a_list[0] = n;
  a_list[1] = ave0;
  a_list[2] = var0;

  t_test_statistic(a_list, ave, result_list);

  if(-1 * t_025 < result_list[1] && result_list[1] < t_025){
    is_accept = "受容";
  }else{
    is_accept = "棄却";
  }

  printf("自由度は%2.0f, 検定統計量は%f です。\n", result_list[0], result_list[1]);
  printf("95%%信頼区間は %f < t < %f なので、帰無仮説は %s されます。\n", -1 * t_025, t_025, is_accept);
  */


  // ----------------------- EX.8-3 ----------------------- /
  // file8.dat
  // 20羽のヒヨコの体重（file7）の分散は、与えられている母分散と等しいか。
  // 有意水準5%の両側χ^2検定
  /*
  chi_025 = 32.8523;
  chi_975 = 8.9065;

  scanf("%lf", &var);
  scanf("%d", &n);

  // 入力データを配列にいれる
  for(i=0; i<n; i++){
    scanf("%lf", &data_list[i]);
  }

  ave0 = average(data_list, n);
  var0 = unbiased_variance(data_list, n);
  a_list[0] = n;
  a_list[1] = ave0;
  a_list[2] = var0;

  chi_test_statistic(a_list, var, result_list);

  if(chi_975 < result_list[1] && result_list[1] < chi_025){
    is_accept = "受容";
  }else{
    is_accept = "棄却";
  }

  printf("自由度は%2.0f, 検定統計量は%f です。\n", result_list[0], result_list[1]);
  printf("95%%信頼区間は %f < χ^2 < %f なので、帰無仮説は %s されます。\n", chi_975, chi_025, is_accept);
  */  


  // ----------------------- EX.8-4 ----------------------- /
  // file9.dat
  // A群(10匹): プリミドンを与えたマウス  B群(9匹): 生理食塩水を与えたマウス
  // 両群のマウスにベメグライドを徐々に与え、何mlで痙攣を起こすか調べた
  // H0: μA = μB, H1: μA ≠ μB を有意水準5%の両側t検定で調べる
  /*
  t_025 = 2.1098;

  // A群
  scanf("%d", &a_n);
  for(i=0; i<a_n; i++){
    scanf("%lf", &data_list[i]);
  }
  ave_a = average(data_list, a_n);
  var_a = unbiased_variance(data_list, a_n);

  // B群
  scanf("%d", &b_n);
  for(i=0; i<b_n; i++){
    scanf("%lf", &data_list[i]);
  }
  ave_b = average(data_list, b_n);
  var_b = unbiased_variance(data_list, b_n);

  a_list[0] = a_n;
  a_list[1] = ave_a;
  a_list[2] = var_a;
  b_list[0] = b_n;
  b_list[1] = ave_b;
  b_list[2] = var_b;

  two_t_test_statistic(a_list, b_list, result_list);

  if(-1 * t_025 < result_list[1] && result_list[1] < t_025){
    is_accept = "受容";
  }else{
    is_accept = "棄却";
  }
  
  printf("自由度は%2.0f, 検定統計量は%f です。\n", result_list[0], result_list[1]);
  printf("95%%信頼区間は %f < t < %f なので、帰無仮説は %s されます。\n", -1 * t_025, t_025, is_accept);
  */


  // ----------------------- EX.8-5 ----------------------- /
  // file9.dat
  // A群(10匹): プリミドンを与えたマウス  B群(9匹): 生理食塩水を与えたマウス
  // 両群のマウスにベメグライドを徐々に与え、何mlで痙攣を起こすか調べた
  // H0: σA = σB, H1: σA ≠ σB を有意水準5%の両側F検定で調べる
  /*
  f_975 = 0.24379;
  f_025 = 4.357;

  // A群
  scanf("%d", &a_n);
  for(i=0; i<a_n; i++){
    scanf("%lf", &data_list[i]);
  }
  ave_a = average(data_list, a_n);
  var_a = unbiased_variance(data_list, a_n);

  // B群
  scanf("%d", &b_n);
  for(i=0; i<b_n; i++){
    scanf("%lf", &data_list[i]);
  }
  ave_b = average(data_list, b_n);
  var_b = unbiased_variance(data_list, b_n);

  a_list[0] = a_n;
  a_list[1] = ave_a;
  a_list[2] = var_a;
  b_list[0] = b_n;
  b_list[1] = ave_b;
  b_list[2] = var_b;

  f_test_statistic(a_list, b_list, result_list);

  if(f_975 < result_list[2] && result_list[2] < f_025){
    is_accept = "受容";
  }else{
    is_accept = "棄却";
  }
  
  printf("自由度は%2.0f, %2.0f, 検定統計量は%f です。\n", result_list[0], result_list[1], result_list[2]);
  printf("95%%信頼区間は %f < F < %f なので、帰無仮説は %s されます。\n", f_975, f_025, is_accept);
  */


  // ----------------------- HW8 ----------------------- /
  // file10.dat
  // A群(11匹): 健康ネコ  B群(7匹): 水俣病罹患ネコ
  // 両群の肝臓中の総水銀量が異なるといえるか
  // 1. H0: σA = σB, H1: σA ≠ σB を有意水準5%の両側F検定で調べる
  // 2. 1.でH0が受容されたら（母分散が等しかったら）、次にH0: μA = μB, H1: μA ≠ μB を有意水準5%の2標本両側t検定で調べる
  
  f_975 = 0.24557;
  f_025 = 5.461;
  t_025 = 2.1199;

  // A群
  scanf("%d", &a_n);
  for(i=0; i<a_n; i++){
    scanf("%lf", &data_list[i]);
  }
  ave_a = average(data_list, a_n);
  var_a = unbiased_variance(data_list, a_n);

  // B群
  scanf("%d", &b_n);
  for(i=0; i<b_n; i++){
    scanf("%lf", &data_list[i]);
  }
  ave_b = average(data_list, b_n);
  var_b = unbiased_variance(data_list, b_n);

  a_list[0] = a_n;
  a_list[1] = ave_a;
  a_list[2] = var_a;
  b_list[0] = b_n;
  b_list[1] = ave_b;
  b_list[2] = var_b;

  f_test_statistic(a_list, b_list, result_list);

  if(f_975 < result_list[2] && result_list[2] < f_025){
    is_accept = "受容";
    flag = 1;
  }else{
    is_accept = "棄却";
    flag = 0;
  }
  
  printf("1. 両群の母分散が一致するかを確かめるため、有意水準5%%で両側F検定\n");
  printf("自由度は%2.0f, %2.0f, 検定統計量は%f です。\n", result_list[0], result_list[1], result_list[2]);
  printf("95%%信頼区間は %f < F < %f なので、帰無仮説は %s されます。\n\n", f_975, f_025, is_accept);

  if(flag){
    two_t_test_statistic(a_list, b_list, result_list);

    if(-1.0 * t_025 < result_list[1] && result_list[1] < t_025){
      is_accept2 = "受容";
      message = "ありません";
    }else{
      is_accept2 = "棄却";
      message = "あります";
    }

    printf("2. 両群の母平均が一致するかを確かめるため、有意水準5%%で両側2標本検定\n");
    printf("自由度は%2.0f, 検定統計量は%f です。\n", result_list[0], result_list[1]);
    printf("95%%信頼区間は %f < t < %f なので、帰無仮説は %s されます。\n\n", -1.0 * t_025, t_025, is_accept);
    printf("したがって、健康ネコと水俣病罹患ネコの肝臓中の水銀量には違いが %s\n", message);
  }
  
  return 0;
}


/*----------------  以下関数集  ----------------*/

// 行列のアドレスを確保
int mat_alloc(matrix *mat, int row, int col){
  int i, j;

  mat->row = row;
  mat->col = col;
  mat->element = calloc(row, sizeof(double*));
  mat->base_element = calloc(col * row, sizeof(double));

  if (mat->element == NULL || mat->base_element == NULL){
    printf("配列用の領域が確保できませんでした\n");
    return -1;
  }

  for(i=0; i<row; i++){
    mat->element[i] = &mat->base_element[i * col];
  }

  return 0;
}


// 行列用のメモリを解放
void mat_free(matrix *mat){
  free(mat->base_element);
  free(mat->element);
}


// 平均を返す
double average(double *list, int n){
  int i;
  double sum;

  sum = 0;
  for(i=0; i<n; i++){
    sum += list[i];
  }

  return sum * 1.0 / n;
}


// 分散を返す
double variance(double *list, int n){
  int i;
  double sum, ave;

  ave = average(list, n);
  sum = 0;
  for(i=0; i<n; i++){
    sum += pow((list[i] - ave), 2);
  }

  return sum * 1.0 / n;
}


// 不偏分散を返す
double unbiased_variance(double *list, int n){
  int i;
  double sum, ave;

  ave = average(list, n);
  sum = 0;
  for(i=0; i<n; i++){
    sum += pow((list[i] - ave), 2);
  }

  return sum * 1.0 / (n-1);
}


// 標準偏差を返す
double standard_deviation(double *list, int n){
  double var;

  var = variance(list, n);
  return sqrt(var);
}


// 不偏標準偏差を返す
double unbiased_standard_deviation(double *list, int n){
  double var;

  var = unbiased_variance(list, n);
  return sqrt(var);
}


// 0〜1までの一様分布を返す
double s_uniform_rv(void){
  return 1.0 * rand() / RAND_MAX;
}


// ベルヌーイr.vを返す
int bernoulli_rv(double p){
  return s_uniform_rv() < p ? 1 : 0;
}


// 二項分布のr.vを返す
int binomial_rv(double p, int n){
  int i, count;

  count = 0;
  for(i=0; i<n; i++){
    count += bernoulli_rv(p);
  }

  return count;
}


// n!を求める
int factorial(int n){
  // 負の整数は排除
  if(n < 0){
    return -1;
  }

  if(n == 0){
    return 1;
  }

  return factorial(n-1) * n;
}


// nPk = n * (n-1) * ... * (n-k)を求める
// nP0 = 1 とする
int partial(int n, int k){
  // 負の引数およびn<kの時はエラー
  if(n < 0 || k < 0 || n < k){
    return -1;
  }

  if(k == 0){
    return 1;
  }

  return partial(n-1, k-1) * n;
}


// nCkを求める
int combination(int n, int k){
  // 負の引数およびn<kの時はエラー
  if(n < 0 || k < 0 || n < k){
    return -1;
  }

  // パスカルの三角形の端（k=0とk=n）は1
  if(k==0 || k==n){
    return 1;
  }

  return combination(n-1, k) + combination(n-1, k-1);
}


// 二項分布Bin(p, n)の、k=0, 1, 2,..., nにおける理論値を配列として返す
int binomial_tv(double p, int n, double *list){
  int k;

  for(k=0; k<=n; k++){
    list[k] = combination(n, k) * pow(p, k) * pow(1-p, n-k);
  }

  return 0;
}


// 2項分布の分布関数を返す
double binomial_distribution_value(double p, int n, int k){
  int i, size;
  double F;
  double tv_list[1000]={0};
  
  binomial_tv(p, n, tv_list);

  F = 0;
  for(i=0; i<=k; i++){
    F += tv_list[i];
  }

  return F;
}


// 1標本t検定（母分散未知）
// H0: μ = μ0 を検定するための自由度と検定統計量を返す
// Input: list = [標本数, 平均, 分散]
// Output: result_list = [自由度, 検定統計量]
void t_test_statistic(double *list, double ave, double *result_list){
  int size, flexibility;
  double ave0, var0, s_estimate, test_statistic;

  size = list[0];
  ave0 = list[1];
  var0 = list[2];

  flexibility = size - 1;
  s_estimate = var0 / size;
  test_statistic = (ave - ave0) / sqrt(s_estimate);

  result_list[0] = flexibility;
  result_list[1] = test_statistic;
}


// χ^2検定
// H0: σ = σ0 を検定するための自由度と検定統計量を返す
// Input: list = [標本数, 平均, 分散]
// Output: result_list = [自由度, 検定統計量]
void chi_test_statistic(double *list, double var, double *result_list){
  int size, flexibility;
  double ave0, var0, s_estimate, test_statistic;

  size = list[0];
  ave0 = list[1];
  var0 = list[2];

  flexibility = size - 1;
  test_statistic = flexibility * var0 / var;

  result_list[0] = flexibility;
  result_list[1] = test_statistic;
}


// 2標本t検定（A, B母分散未知 かつ A, Bの母分散は等しい場合）
// H0: μ_A = μ_B を検定するための自由度と検定統計量を返す
// Input: a_list = [aの標本数, aの平均, aの分散], b_list = [bの標本数, bの平均, bの分散]
// Output: result_list = [自由度, 検定統計量]
void two_t_test_statistic(double *a_list, double *b_list, double *result_list){
  int a_size, b_size, flexibility;
  double a_ave, b_ave, a_var, b_var, s_estimate, test_statistic;

  a_size = a_list[0];
  b_size = b_list[0];
  a_ave = a_list[1];
  b_ave = b_list[1];
  a_var = a_list[2];
  b_var = b_list[2];

  flexibility = a_size + b_size - 2;
  s_estimate = ( (a_size - 1) * a_var + (b_size - 1) * b_var ) / flexibility;
  test_statistic = (a_ave - b_ave) / sqrt((1.0/a_size + 1.0/b_size) * s_estimate);

  result_list[0] = flexibility;
  result_list[1] = test_statistic;
}


// F検定（母分散が等しいか検定）
// H0: σ_A = σ_B を検定するための自由度と検定統計量を返す
// Input: a_list = [aの標本数, aの平均, aの分散], b_list = [bの標本数, bの平均, bの分散]
// Output: result_list = [自由度1, 自由度2, 検定統計量]
void f_test_statistic(double *a_list, double *b_list, double *result_list){
  int a_size, b_size, flexibility1, flexibility2;
  double a_ave, b_ave, a_var, b_var, s_estimate, test_statistic;

  a_size = a_list[0];
  b_size = b_list[0];
  a_ave = a_list[1];
  b_ave = b_list[1];
  a_var = a_list[2];
  b_var = b_list[2];

  flexibility1 = a_size - 1;
  flexibility2 = b_size - 1;
  test_statistic = a_var / b_var;

  result_list[0] = flexibility1;
  result_list[1] = flexibility2;
  result_list[2] = test_statistic;
}




