/* 152042 松下 旦 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#define INPUT_NUM 100
#define STATIC_LIST_LENGTH 1000
#define MAX_LOOP 1000


typedef struct{
  int row;
  int col;
  double **element;
  double *base_element;
} matrix;


typedef struct{
  int row;
  int col;
  unsigned long long **element;
  unsigned long long *base_element;
} lmatrix;


int mat_alloc(matrix *mat, int row, int col);
int lmat_alloc(lmatrix *mat, int row, int col);
void mat_free(matrix *mat);
void lmat_free(lmatrix *mat);
void mat_print(matrix mat);
void lmat_print(lmatrix mat);

double average(double *list, int n);
double variance(double *list, int n);
double unbiased_variance(double *list, int n);
double standard_deviation(double *list, int n);
double unbiased_standard_deviation(double *list, int n);

unsigned long long factorial(int n);
unsigned long long partial(int n, int k);
// n=60くらいまでしか計算出来ない
unsigned long long combination(int n, int k);

double s_uniform_rv(void);
int bernoulli_rv(double p);
int binomial_rv(double p, int n);
int poison_tv(double lambda, double *list, int size);
int geometric_tv(double p, double *list, int size);
int negative_binomial_tv(double p, int m, double *list, int size);
// 超幾何分布HyperGeo(N, K, s)の、t=0, 1, 2,..., n における理論値を配列として返す
// Input: N=全要素、K=成功要素、s=非復元抽出する個数
// Output: t_list=取り出したの内の成功要素をt個(t=0, 1,..., s)とり出す確率の配列
int hypergeometric_tv(int n, int k, int s, double *list);
double binomial_distribution_value(double p, int n, int k);
double hypergeometric_distribution_value(double n, int k, int s, int t);

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

// χ^2適合度検定
// Input: observed: それぞれのケースの観測値, theoretical_value: それぞれのケースの理論値, size: ケース数
// Output: result_list = [自由度, 検定統計量]
void chi_fit_test_statistic(double *observed, double *theoretical_value, int size, double *result_list);

// χ^2独立性検定
// Input: matrix: データ
// Output: result_list = [自由度, 検定統計量]
void chi_independent_test_statistic(matrix data, double *result_list);

// フィッシャーの正確確率検定
// Input: matrix（2行2列）
// Output: p（与えられたデータよりも偏る確率）
double fisher_exact_test(matrix table);


int main(void){
  int i, j, n, bin, count, a_n, b_n, flag, sum, size, row, col;
  double observed[4]={0};
  double theoretical_probability[4]={0};
  double theoretical_value[4]={0};
  double result_list[3]={0};
  double theoretical_values[1000]={0};
  double a_list[3], b_list[3];
  double p, z, chi_025, chi_975, z_025, t_025, f_025, f_975, ave, var, sd, test_statistic;
  char *is_accept;
  matrix data;

  // 乱数の初期化
  srand(time(0));
  //srand(2042);

  // ----------------------- EX.9-1 ----------------------- //
  // メンデルのえんどうの実験(file11.dat)
  // 丸・黄色 / しわ・黄色 / 丸・緑色 / しわ・緑色の観測値の比率が、理論値9:3:3:1と等しいか調べる。
  // データは、それぞれのケースの観測値と理論確率を与えている。
  /*
  chi_025 = 9.3484;
  chi_975 = 0.2158;

  // データ読み込み
  sum = 0;
  scanf("%d", &size);
  for(i=0; i<size; i++){
    scanf("%lf", &observed[i]);
    scanf("%lf", &theoretical_probability[i]);
    sum += observed[i];
  }

  // 理論値を求めつつ、カイ二乗適合度検定を用いるのが適切か検査
  count = 0;
  for(i=0; i<size; i++){
    theoretical_value[i] = sum * theoretical_probability[i];
    if(theoretical_value[i] <= 1.0){
      printf("理論値が1以下のケースがあるため、χ^2適合度検定を用いるのは不適切です");
      return -1;
    }

    if(theoretical_value[i] <= 5.0){
      count += theoretical_value[i];
    }
  }
  if(count * 1.0 / sum >= 0.2){
    printf("理論値が5以下のケースが2割以上あるため、χ^2適合度検定を用いるのは不適切です");
    return -1;
  }

  chi_fit_test_statistic(observed, theoretical_value, size, result_list);

  if(chi_975 < result_list[1] && result_list[1] < chi_025){
    is_accept = "受容";
  }else{
    is_accept = "棄却";
  }

  printf("自由度は%2.0f, 検定統計量は%f です。\n", result_list[0], result_list[1]);
  printf("95%%信頼区間は %f < χ^2 < %f なので、帰無仮説は %s されます。\n", chi_975, chi_025, is_accept);
  */

  // ----------------------- EX.9-2 ----------------------- //
  // χ^2独立性検定(file12.dat)
  /*
  chi_025 = 11.1433;
  chi_975 = 0.4844;

  // データ読み込み
  sum = 0;
  scanf("%d", &row);
  scanf("%d", &col);
  mat_alloc(&data, row, col);
  for(i=0; i<row; i++){
    for(j=0; j<col; j++){
      scanf("%lf", &data.element[i][j]);
    }
  }

  mat_print(data);

  chi_independent_test_statistic(data, result_list);

  if(chi_975 < result_list[1] && result_list[1] < chi_025){
    is_accept = "受容";
  }else{
    is_accept = "棄却";
  }

  printf("自由度は%2.0f, 検定統計量は%f です。\n", result_list[0], result_list[1]);
  printf("95%%信頼区間は %f < χ^2 < %f なので、帰無仮説は %s されます。\n", chi_975, chi_025, is_accept);
  */

  // ----------------------- HW.9 ----------------------- //
  // フィッシャーの正確確率法(file13.dat)。片側検定でよい（はず）。
  // 2*2の表は自由度1（各行, 列の小計が変わらないので、表の要素を1つ決めれば他の要素は全て決まる）

  sum = 0;
  scanf("%d", &row);
  scanf("%d", &col);
  mat_alloc(&data, row, col);
  for(i=0; i<row; i++){
    for(j=0; j<col; j++){
      scanf("%lf", &data.element[i][j]);
    }
  }

  mat_print(data);

  p = fisher_exact_test(data);

  if(p > 0.05){
    is_accept = "受容";
  }else{
    is_accept = "棄却";
  }

  printf("有意水準を5%%とする。サンプルが与えられたデータと同じか、それよりも偏る確率は%lf \n", p);
  printf("したがって、帰無仮説は %s されます。\n", is_accept);


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


// 行列のアドレスを確保
int lmat_alloc(lmatrix *mat, int row, int col){
  int i, j;

  mat->row = row;
  mat->col = col;
  mat->element = calloc(row, sizeof(unsigned long long*));
  mat->base_element = calloc(col * row, sizeof(unsigned long long));

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


// 行列用のメモリを解放
void lmat_free(lmatrix *mat){
  free(mat->base_element);
  free(mat->element);
}


// 配列を行列形式でprintf
void mat_print(matrix mat){
  int i, j;

  printf("\n");
  for(i=0; i<mat.row; i++){
    for(j=0; j<mat.col; j++){
      printf("%5f ", mat.element[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}


// 配列を行列形式でprintf
void lmat_print(lmatrix mat){
  int i, j;

  printf("\n");
  for(i=0; i<mat.row; i++){
    for(j=0; j<mat.col; j++){
      printf("%llu ", mat.element[i][j]);
    }
    printf("\n");
  }
  printf("\n");
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


// n!を求める
unsigned long long factorial(int n){
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
unsigned long long partial(int n, int k){
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
// DP
unsigned long long combination(int n, int k){
  int i, j;
  unsigned long long result;
  lmatrix table;

  // 負の引数およびn<kの時はエラー
  if(n < 0 || k < 0 || n < k){
    return -1;
  }


  // tableを作成。全要素0で初期化。
  lmat_alloc(&table, n+1, n+1);

  for(i=0; i<n+1; i++){
    for(j=0; j<i+1; j++){
      if(j == 0 || j == i){
        table.element[i][j] = 1;
      }
      else{
        table.element[i][j] = table.element[i-1][j-1] + table.element[i-1][j];
      }
    }
  }

  result = table.element[n][k];
  lmat_free(&table);

  return result;
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


// 二項分布Bin(p, n)の、k=0, 1, 2,..., nにおける理論値を配列として返す
int binomial_tv(double p, int n, double *list){
  int k;

  for(k=0; k<=n; k++){
    list[k] = combination(n, k) * pow(p, k) * pow(1-p, n-k);
  }

  return 0;
}


// ポアソン分布Po(λ)の、k=0, 1, 2,... における理論値を配列として返す
// sizeの分だけkを返す
int poison_tv(double lambda, double *list, int size){
  int k;

  for(k=0; k<size; k++){
    list[k] = pow(lambda, k) / (factorial(k) * exp(lambda));
    // もし桁あふれがおきていたら、0.0をかえす
    // ポアソン分布の峰はふつう1つなので、平均よりも大きい場所で、k項目が負になったり、k-1の時より大きくなったりしたら、それは不正な値

    if(k > lambda && ( list[k] < 0 || list[k-1] < list[k])){
      list[k] = 0.0;
    }
  }

  return 0;
}


// 幾何分布Geo(p)の、k=1, 2,... における理論値を配列として返す（つまり、*listの先頭の要素にはk=1の結果が入る）
// sizeの分だけkを返す
int geometric_tv(double p, double *list, int size){
  int k;

  list[0] = 0; // 0回目で表が出る確率は0としておく
  for(k=1; k<size; k++){
    list[k] = p * pow(1-p, k-1);
  }

  return 0;
}


// 負の二項分布NBin(p, m)の、k=m, m+1, m+2,...  における理論値を配列として返す（つまり、*listの先頭の要素にはk=mの結果が入る）
// sizeの分だけkを返す
int negative_binomial_tv(double p, int m, double *list, int size){
  int k;

  for(k=m; k<m+size; k++){
    list[k] = factorial(k-1) * pow(p, m) * pow(1-p, k-m) / (factorial(m-1) * factorial(k-m));
  }

  return 0;
}


// 超幾何分布HyperGeo(N, K, s)の、t=0, 1, 2,..., n における理論値を配列として返す
// N:全要素、K:成功要素、s:非復元抽出する個数、t:sの内の成功要素の個数
int hypergeometric_tv(int n, int k, int s, double *list){
  int t;

  for(t=0; t<=s; t++){
    list[t] = 1.0 * combination(s, t) * combination(n-s, k-t) / combination(n, k);
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


// 超幾何分布の分布関数を返す
double hypergeometric_distribution_value(double n, int k, int s, int t){
  int i, size;
  double F;
  double tv_list[1000]={0};
  
  hypergeometric_tv(n, k, s, tv_list);

  F = 0;
  for(i=0; i<=t; i++){
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


// χ^2適合度検定
// Input: observed: それぞれのケースの観測値, theoretical_value: それぞれのケースの理論値, size: ケース数
// Output: result_list = [自由度, 検定統計量]
void chi_fit_test_statistic(double *observed, double *theoretical_value, int size, double *result_list){
  int i, j, flexibility;
  double sum, s_estimate, test_statistic;

  flexibility = size - 1;

  sum = 0;
  for(i=0; i<size; i++){
    sum += pow(observed[i]-theoretical_value[i], 2) * 1.0 / theoretical_value[i];
  }
  test_statistic = sum;

  result_list[0] = flexibility;
  result_list[1] = test_statistic;
}


// χ^2独立性検定
// Input: matrix: データ
// Output: result_list = [自由度, 検定統計量]
void chi_independent_test_statistic(matrix data, double *result_list){
  int row, col, i, j, flexibility;
  double sum, row_sum, col_sum, total, s_estimate, test_statistic;
  matrix table; // dataにくわえ、最後にそれぞれの行/列の和を追加したデータテーブル

  row = data.row;
  col = data.col;

  mat_alloc(&table, row+1, col+1);

  flexibility = (row-1) * (col-1);

  // tableを作成
  // dataから要素をコピー + 行ごとの和を計算
  for(i=0; i<row; i++){
    row_sum = 0;
    for(j=0; j<col+1; j++){
      if(j == col){
        table.element[i][j] = row_sum;
      }else{
        table.element[i][j] = data.element[i][j];
        row_sum += data.element[i][j];
      }
    }
  }
  // 列ごとの和を計算
  for(j=0; j<col+1; j++){
    col_sum = 0;
    for(i=0; i<row+1; i++){
      if(i == row){
        table.element[i][j] = col_sum;
      }else{
        col_sum += table.element[i][j];
      }
    }
  }

  total = table.element[row][col];

  sum = 0;
  for(i=0; i<row; i++){
    for(j=0; j<col; j++){
      sum += pow(table.element[i][j] - 1.0 * table.element[i][col] * table.element[row][j] / total, 2)
               / (table.element[i][col] * table.element[row][j] / total);
    }
  }
  test_statistic = sum;

  result_list[0] = flexibility;
  result_list[1] = test_statistic;

  mat_free(&table);
}


// フィッシャーの正確確率検定
// Input: matrix（2行2列）
// Output: p（与えられたデータよりも偏る確率）
double fisher_exact_test(matrix table){
  int row, col, i, j, sum=0, row_sum[2]={0}, col_sum[2]={0};

  row = table.row;
  col = table.col;

  if(row != 2 || col != 2){
    printf("フィッシャーの正確確率法は2*2の検定にしか使えません。\n");
    return -1;
  }

  for(i=0; i<row; i++){
    for(j=0; j<col; j++){
      sum += table.element[i][j];
      row_sum[i] += table.element[i][j];
      col_sum[j] += table.element[i][j];
    }
  }

  if(sum > 60){
    printf("総計が大きすぎるため値を求めることができません。\n");
    return -1;
  }

  if(table.element[0][0] <= table.element[0][1]){
    return hypergeometric_distribution_value(sum, row_sum[0], col_sum[0], table.element[0][0]);
  }
  else{
    return 1.0 - hypergeometric_distribution_value(sum, row_sum[0], col_sum[0], table.element[0][0]);
  }
}






















