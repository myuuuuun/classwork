/* 152042 松下 旦 */
// フィッシャーの正確確率検定を両側に修正

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


typedef struct{
  int row;
  int col;
  int size;
  double ***element;
  double **base_element;
  double *base_base_element;
} threed_matrix;


int mat_alloc(matrix *mat, int row, int col);
int lmat_alloc(lmatrix *mat, int row, int col);
int threed_mat_alloc(threed_matrix *mat, int row, int col, int size);
void mat_free(matrix *mat);
void lmat_free(lmatrix *mat);
void threed_mat_free(threed_matrix *mat);
void mat_print(matrix mat);
void lmat_print(lmatrix mat);
void threed_mat_print(threed_matrix mat);
int check(matrix mat1, matrix mat2);

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

/* 一元配置分散分析
 Input: matrix data(行に水準を、列にデータをとったもの)
 Output: aves(0番目に総平均, それ以降にそれぞれのlevelの平均を入れる),
         level_diff = [級間自由度, 級間平方和, 級間平方平均],
         errors = [誤差自由度, 誤差平方和, 誤差平方平均],
         total = [総自由度, 総平方和]
         return = 自由度(級間自由度, 誤差自由度)のF分布の検定統計量*/
double one_way_anova(matrix data, double *aves, double *level_diff, double *errors, double *total);

/* 二元配置分散分析
 Input: threed_matrix data(行, 列に要因1, 2を、要素にさらに配列をとった)
 Output: factor1_info = [要因1の自由度, 平方和, 平方平均],
         factor2_info = [要因2の自由度, 平方和, 平方平均],
         factor12_info = [要因12の自由度, 平方和, 平方平均],
         errors = [誤差自由度, 平方和, 平方平均],
         total = [総自由度, 総平方和]
         f_statistics = [要因1の主効果のF検定統計量, 要因2の主効果のF検定統計量, 要因1・2の交互作用の検定統計量]*/      
void two_way_anova(threed_matrix data, double *factor1_info, double *factor2_info,
                       double *factor12_info, double *errors, double *total, double *f_statistics);


int main(void){
  int i, j, k, n, bin, count, a_n, b_n, flag, sum, size, row, col, data_size, factor_size, factor_size1, factor_size2;
  double observed[4]={0};
  double theoretical_probability[4]={0};
  double theoretical_value[4]={0};
  double theoretical_values[1000]={0};
  double a_list[3], b_list[3], mu_list[5], ave_list[6], level_diff[3], errors[3], total[2];
  double factor1_info[3], factor2_info[3], factor12_info[3], f_statistics[3];
  double p, z, ave, var, sd, test_statistic, f_statistic, f_05, f_1_42_05, f_2_42_05;
  char *is_accept;
  matrix data;
  threed_matrix data3;

  // 乱数の初期化
  srand(time(0));

  // ----------------------- HW.9 ----------------------- //
  // フィッシャーの正確確率法(file13.dat)。片側検定でよい（はず）。-> 両側にしました。
  // 2*2の表は自由度1（各行, 列の小計が変わらないので、表の要素を1つ決めれば他の要素は全て決まる）
  /*
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


  //printf("%f \n", hypergeometric_distribution_value(5, 3, 2, 1));

  p = fisher_exact_test(data);

  if(p > 0.05){
    is_accept = "受容";
  }else{
    is_accept = "棄却";
  }

  printf("有意水準を5%%とする。サンプルが与えられたデータと同じか、それよりも偏る確率は%lf \n", p);
  printf("したがって、帰無仮説は %s されます。\n", is_accept);
  */


  // ----------------------- EX.10-1, 2 ----------------------- //
  // 一元配置分散分析(file14.dat)
  // H0: μ = μ1 = μ2 = μ3 = μ4 = μ5 を検定
  /*
  f_05 = 2.58;

  scanf("%d", &data_size);
  scanf("%d", &factor_size);
  mat_alloc(&data, factor_size, data_size);

  for(i=0; i<data_size; i++){
    for(j=0; j<factor_size; j++){
      scanf("%lf", &data.element[j][i]);
    }
  }

  f_statistic = one_way_anova(data, ave_list, level_diff, errors, total);

  printf("総平均は%lf\n", ave_list[0]);
  for(i=0; i<factor_size; i++){
    printf("水準%d の平均は%lf  ", i+1, ave_list[i+1]);
  }
  printf("\n");
  printf("級間自由度は %lf, 級間平方和は %lf, 平均級間平方は %lf\n", level_diff[0], level_diff[1], level_diff[2]);
  printf("誤差自由度は %lf, 誤差平方和は %lf, 平均誤差平方は %lf\n", errors[0], errors[1], errors[2]);
  printf("総自由度は %lf, 総平方和は %lf\n", total[0], total[1]);
  printf("----------------------------------------------\n");

  printf("いま、検定統計量Fは %lf で、これは自由度 %lf, %lf のF分布に従う\n", f_statistic, level_diff[0], errors[0]);

  if(f_statistic < f_05){
    is_accept = "受容";
  }else{
    is_accept = "棄却";
  }

  printf("有意水準5%%で片側検定すると、95%%信頼区間は F < %lf なので、帰無仮説は %s されます\n", f_05, is_accept);
  mat_free(&data);
  */

  // ----------------------- HW.10 ----------------------- //
  // 二元配置分散分析(file15.dat)
  // 海水の濃度を3次元配列の行、種を列にとる
  // 二種類の因子それぞれの結果への影響（主効果）、および両因子の組み合わせによる結果への影響（交互作用）の有無を調べる
  // それぞれの因子による主効果をα, β、交互作用をγとおく
  // H_α0: α1 = α2 = ... = 0, H_β0 = β1 = β2 = ... 0, H_γ0: γ11 = γ22 = ... = 0 をそれぞれ検定
  /*
  f_1_42_05 = 4.07;
  f_2_42_05 = 3.22;

  scanf("%d", &factor_size1);
  scanf("%d", &factor_size2);
  scanf("%d", &data_size);
  threed_mat_alloc(&data3, factor_size1, factor_size2, data_size);

  for(i=0; i<factor_size1; i++){
    for(j=0; j<factor_size2; j++){
      for(k=0; k<data_size; k++){
        scanf("%lf", &data3.element[i][j][k]);
      }
    }
  }

  threed_mat_print(data3);
  two_way_anova(data3, factor1_info, factor2_info, factor12_info, errors, total, f_statistics);

  printf("要因1（海水濃度）: \n");
  printf("自由度は %lf, 平方和は %lf, 平方平均は %lf\n\n", factor1_info[0], factor1_info[1], factor1_info[2]);

  printf("要因2（種）: \n");
  printf("自由度は %lf, 平方和は %lf, 平方平均は %lf\n\n", factor2_info[0], factor2_info[1], factor2_info[2]);

  printf("要因１・2: \n");
  printf("自由度は %lf, 平方和は %lf, 平方平均は %lf\n\n", factor12_info[0], factor12_info[1], factor12_info[2]);

  printf("誤差: \n");
  printf("自由度は %lf, 平方和は %lf, 平方平均は %lf\n\n", errors[0], errors[1], errors[2]);

  printf("合計: \n");
  printf("自由度は %lf, 平方和は %lf\n", total[0], total[1]);
  printf("----------------------------------------------\n");

  printf("要因1の主効果の検定統計量Fは %lf で、これは自由度 %lf, %lf のF分布に従う\n", f_statistics[0], factor1_info[0], errors[0]);
  if(f_statistics[0] < f_2_42_05){
    is_accept = "受容";
  }else{
    is_accept = "棄却";
  }
  printf("有意水準5%%で片側検定すると、95%%信頼区間は F < %lf なので、帰無仮説は %s されます\n\n", f_2_42_05, is_accept);

  printf("要因2の主効果の検定統計量Fは %lf で、これは自由度 %lf, %lf のF分布に従う\n", f_statistics[1], factor2_info[0], errors[0]);
  if(f_statistics[1] < f_1_42_05){
    is_accept = "受容";
  }else{
    is_accept = "棄却";
  }
  printf("有意水準5%%で片側検定すると、95%%信頼区間は F < %lf なので、帰無仮説は %s されます\n\n", f_1_42_05, is_accept);

  printf("要因1・2の交互作用の検定統計量Fは %lf で、これは自由度 %lf, %lf のF分布に従う\n", f_statistics[2], factor12_info[0], errors[0]);
  if(f_statistics[2] < f_2_42_05){
    is_accept = "受容";
  }else{
    is_accept = "棄却";
  }
  printf("有意水準5%%で片側検定すると、95%%信頼区間は F < %lf なので、帰無仮説は %s されます\n", f_2_42_05, is_accept);

  threed_mat_free(&data3);
  */

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


// 行列のアドレスを確保
int threed_mat_alloc(threed_matrix *mat, int row, int col, int size){
  int i, j;

  mat->row = row;
  mat->col = col;
  mat->size = size;
  mat->element = calloc(row, sizeof(double**));
  mat->base_element = calloc(col * row, sizeof(double*));
  mat->base_base_element = calloc(col * row * size, sizeof(double));

  if (mat->element == NULL || mat->base_element == NULL || mat->base_base_element == NULL){
    printf("配列用の領域が確保できませんでした\n");
    return -1;
  }

  for(i=0; i<row; i++){
    mat->element[i] = &mat->base_element[i * col];
  
    for(j=0; j<col; j++){
      mat->base_element[i * col + j] = &mat->base_base_element[i * col * size + j * size];
    }
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


// 行列用のメモリを解放
void threed_mat_free(threed_matrix *mat){
  free(mat->base_base_element);
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


// 配列を行列形式でprintf
void threed_mat_print(threed_matrix mat){
  int i, j, k;

  printf("\n");
  for(i=0; i<mat.row; i++){
    for(j=0; j<mat.col; j++){
      printf("[");
      for(k=0; k<mat.size; k++){
        if(k!=mat.size-1){
          printf("%7.3f, ", mat.element[i][j][k]);
        }
        else{
          printf("%7.3f", mat.element[i][j][k]);
        }
      }
      printf("] ");
    }
    printf("\n");
  }
  printf("\n");
}


// 行列のサイズが同じかチェック
int check(matrix mat1, matrix mat2){
  if(mat1.col != mat2.col || mat1.row != mat2.row){
    return -1;
  }
  return 0;
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
  double a_ave, b_ave, a_var, b_var, test_statistic;

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
  int row, col, i, j, sum=0, row_sum[2]={0}, col_sum[2]={0}, a, b, c, d, aa;
  double diff;

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

  diff = fabs(1.0 * table.element[0][0] * table.element[1][1] - 1.0 * table.element[0][1] * table.element[1][0]);

  if(table.element[0][0] <= table.element[1][0]){
    for(i=table.element[0][0]+1; i<col_sum[0]; i++){
      a = i;
      b = row_sum[0] - a;
      c = col_sum[0] - a;
      d = row_sum[1] - c;
      if(diff <= fabs(1.0 * a * d - 1.0 * b * c)){
        aa = i-1;
      }
    }
    return hypergeometric_distribution_value(sum, row_sum[0], col_sum[0], table.element[0][0])
     + 1.0 - hypergeometric_distribution_value(sum, row_sum[0], col_sum[0], aa);
  }
  else{
    for(i=0; i<table.element[0][0]; i++){
      a = i;
      b = row_sum[0] - a;
      c = col_sum[0] - a;
      d = row_sum[1] - c;
      if(diff <= fabs(1.0 * a * d - 1.0 * b * c)){
        aa = i;
      }
    }
    return hypergeometric_distribution_value(sum, row_sum[0], col_sum[0], aa)
     + 1.0 - hypergeometric_distribution_value(sum, row_sum[0], col_sum[0], table.element[0][0]-1);
  }
}


/* 一元配置分散分析
 Input: matrix data(行に水準を、列にデータをとったもの)
 Output: aves(0番目に総平均, それ以降にそれぞれのlevelの平均を入れる),
         level_diff = [級間自由度, 級間平方和, 級間平方平均],
         errors = [誤差自由度, 誤差平方和, 誤差平方平均],
         total = [総自由度, 総平方和]
         return = 自由度(級間自由度, 誤差自由度)のF分布の検定統計量*/      
double one_way_anova(matrix data, double *aves, double *level_diff, double *errors, double *total){
  int i, j, factor_size, data_size;
  double all_ave, sum, sum_squared, sum_total, sum_factor_diff, sum_errors,
   flexibility_total, flexibility_factor_diff, flexibility_errors, f_value,
   ave_factor_diff, ave_errors, ave_list[1000];

  factor_size = data.row;
  data_size = data.col;

  // 各水準の平均
  for(i=0; i<factor_size; i++){
    sum = 0;
    for(j=0; j<data_size; j++){
      sum += data.element[i][j];
    }
    ave_list[i] = sum * 1.0 / data_size;
  }

  // 総平均
  all_ave = average(ave_list, factor_size);

  // 各水準の平均および総平均をavesに入れる
  aves[0] = all_ave;
  for(i=0; i<factor_size; i++){
    aves[i+1] += ave_list[i];
  }

  // 級間自由度
  flexibility_factor_diff = factor_size - 1;

  // 誤差自由度
  flexibility_errors = factor_size * (data_size - 1);

  // 総自由度
  flexibility_total = flexibility_factor_diff + flexibility_errors;

  // 総平方和を計算
  sum_squared = 0;
  for(i=0; i<factor_size; i++){
    for(j=0; j<data_size; j++){
      sum_squared += pow(data.element[i][j], 2);
    }
  }
  sum_total = sum_squared - factor_size * data_size * pow(all_ave, 2);

  // 誤差平方和（各水準内での変動の大きさ）を計算
  sum_errors = sum_squared;
  sum = 0;
  for(i=0; i<factor_size; i++){
    sum += pow(ave_list[i], 2);
  }
  sum_errors -= data_size * sum;

  // 誤差平方平均を計算
  ave_errors = sum_errors * 1.0 / flexibility_errors;

  // 級間平方和を計算
  sum = 0;
  for(i=0; i<factor_size; i++){
    sum += pow(ave_list[i] - all_ave, 2);
  }
  sum_factor_diff = data_size * sum;

  // 級間平方平均を計算
  ave_factor_diff = sum_factor_diff * 1.0 / flexibility_factor_diff;

  // F値を計算
  f_value = ave_factor_diff * 1.0 / ave_errors;

  // 結果をリストに入れる
  level_diff[0] = flexibility_factor_diff;
  level_diff[1] = sum_factor_diff;
  level_diff[2] = ave_factor_diff;

  errors[0] = flexibility_errors;
  errors[1] = sum_errors;
  errors[2] = ave_errors;

  total[0] = flexibility_total;
  total[1] = sum_total;

  return f_value;
}


/* 二元配置分散分析
 Input: threed_matrix data(行, 列に要因1, 2を、要素にさらに配列をとった)
 Output: factor1_info = [要因1の自由度, 平方和, 平方平均],
         factor2_info = [要因2の自由度, 平方和, 平方平均],
         factor12_info = [要因12の自由度, 平方和, 平方平均],
         errors = [誤差自由度, 平方和, 平方平均],
         total = [総自由度, 総平方和]
         f_statistics = [要因1の主効果のF検定統計量, 要因2の主効果のF検定統計量, 要因1・2の交互作用の検定統計量]*/      
void two_way_anova(threed_matrix data, double *factor1_info, double *factor2_info,
                       double *factor12_info, double *errors, double *total, double *f_statistics)
{
  int i, j, k, factor_size1, factor_size2, data_size;
  double all_ave, sum, data1_ave_list[1000], data2_ave_list[1000],
   flexibility_total, flexibility_factor1, flexibility_factor2, flexibility_factor12, flexibility_errors,
   sum_squared_factor1, sum_squared_factor2, sum_squared_factor12, sum_squared_errors, sum_squared_total,
   mean_squared_factor1, mean_squared_factor2, mean_squared_factor12, mean_squared_errors,
   f_statistics_factor1, f_statistics_factor2, f_statistics_factor12;
  
  matrix data12_ave_matrix;

  factor_size1 = data.row; //海水濃度
  factor_size2 = data.col; //種
  data_size = data.size;

  // 要因1, 2, 1*2, 誤差, 全体の自由度を計算
  flexibility_factor1 = factor_size1 - 1;
  flexibility_factor2 = factor_size2 - 1;
  flexibility_factor12 = flexibility_factor1 * flexibility_factor2;
  flexibility_errors = factor_size1 * factor_size2 * (data_size - 1);
  flexibility_total = factor_size1 * factor_size2 * data_size - 1;

  // 総平均
  sum = 0;
  for(i=0; i<factor_size1; i++){
    for(j=0; j<factor_size2; j++){
      for(k=0; k<data_size; k++){
        sum += data.element[i][j][k];
      }
    }
  }
  all_ave = sum * 1.0 / (factor_size1 * factor_size2 * data_size);

  // 要因1の各要素の平均を計算
  for(i=0; i<factor_size1; i++){
    sum = 0;
    for(j=0; j<factor_size2; j++){
      for(k=0; k<data_size; k++){
        sum += data.element[i][j][k];
      }
    }
    data1_ave_list[i] = sum * 1.0 / (factor_size2 * data_size);
  }

  // 要因2の各要素の平均を計算
  for(j=0; j<factor_size2; j++){
    sum = 0;
    for(i=0; i<factor_size1; i++){
      for(k=0; k<data_size; k++){
        sum += data.element[i][j][k];
      }
    }
    data2_ave_list[j] = sum * 1.0 / (factor_size1 * data_size);
  }

  // 要因1*2の各要素の平均を計算
  mat_alloc(&data12_ave_matrix, factor_size1, factor_size2);
  for(j=0; j<factor_size2; j++){
    for(i=0; i<factor_size1; i++){
      sum = 0;
      for(k=0; k<data_size; k++){
        sum += data.element[i][j][k];
      }
      data12_ave_matrix.element[i][j] = sum * 1.0 / (data_size);
    }
  }

  // 要因1の平方和を計算
  sum = 0;
  for(i=0; i<factor_size1; i++){
    sum += pow(data1_ave_list[i] - all_ave, 2);
  }
  sum_squared_factor1 = data_size * factor_size2 * sum;

  // 要因2の平方和を計算
  sum = 0;
  for(i=0; i<factor_size2; i++){
    sum += pow(data2_ave_list[i] - all_ave, 2);
  }
  sum_squared_factor2 = data_size * factor_size1 * sum;

  // 要因1*2の平方和を計算
  sum = 0;
  for(i=0; i<factor_size1; i++){
    for(j=0; j<factor_size2; j++){
      sum += pow(data12_ave_matrix.element[i][j] - data1_ave_list[i] - data2_ave_list[j] + all_ave, 2);
    }
  }
  sum_squared_factor12 = data_size * sum;

  // 誤差の平方和を計算
  sum = 0;
  for(i=0; i<factor_size1; i++){
    for(j=0; j<factor_size2; j++){
      for(k=0; k<data_size; k++){
        sum += pow(data.element[i][j][k] - data12_ave_matrix.element[i][j], 2);
      }
    }
  }
  sum_squared_errors = sum;

  // 総平方和を計算
  sum_squared_total = sum_squared_factor1 + sum_squared_factor2 + sum_squared_factor12 + sum_squared_errors;

  // 要因1の平方平均を計算
  mean_squared_factor1 = sum_squared_factor1 * 1.0 / flexibility_factor1;

  // 要因2の平方平均を計算
  mean_squared_factor2 = sum_squared_factor2 * 1.0 / flexibility_factor2;

  // 要因1*2の平方平均を計算
  mean_squared_factor12 = sum_squared_factor12 * 1.0 / flexibility_factor12;

  // 誤差の平方平均を計算
  mean_squared_errors = sum_squared_errors * 1.0 / flexibility_errors;

  // 要因1の主効果の検定統計量
  f_statistics_factor1 = mean_squared_factor1 * 1.0 / mean_squared_errors;

  // 要因2の主効果の検定統計量
  f_statistics_factor2 = mean_squared_factor2 * 1.0 / mean_squared_errors;

  // 要因12の交互作用の検定統計量
  f_statistics_factor12 = mean_squared_factor12 * 1.0 / mean_squared_errors;

  // 代入
  factor1_info[0] = flexibility_factor1;
  factor1_info[1] = sum_squared_factor1;
  factor1_info[2] = mean_squared_factor1;

  factor2_info[0] = flexibility_factor2;
  factor2_info[1] = sum_squared_factor2;
  factor2_info[2] = mean_squared_factor2;

  factor12_info[0] = flexibility_factor12;
  factor12_info[1] = sum_squared_factor12;
  factor12_info[2] = mean_squared_factor12;

  errors[0] = flexibility_errors;
  errors[1] = sum_squared_errors;
  errors[2] = mean_squared_errors;

  total[0] = flexibility_total;
  total[1] = sum_squared_total;

  f_statistics[0] = f_statistics_factor1;
  f_statistics[1] = f_statistics_factor2;
  f_statistics[2] = f_statistics_factor12;

  mat_free(&data12_ave_matrix);
}
























