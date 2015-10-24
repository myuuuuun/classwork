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
int combination2(int n, int k);
int binomial_tv(double p, int n, double *list);
int poison_tv(double lambda, double *list, int size);
int geometric_tv(double p, double *list, int size);
int negative_binomial_tv(double p, int m, double *list, int size);
int hypergeometric_tv(int n, int k, int s, double *list);



int main(void){
  int i, j, bin, count, index;
  double rv_dist_list[1001]={0}; //0で初期化
  double data_list[1001]={0}; //0で初期化
  double ave_list[1001]={0}; //0で初期化
  double ave, u_var, u_sd, ave_u_var, t_ave, t_u_var, t_u_sd, t_ave_u_var;


  // 乱数の初期化
  //srand(time(0));
  srand(2042);

  // ----------------------- EX.6-1 ----------------------- //
  // 「コインを投げる」試行を10回繰り返した時の標本平均・分散・標準偏差を、母集団のそれと比較する

  /*
  // 標本平均・標本分散・標本標準偏差を求める
  for(i=0; i<10; i++){
    rv_dist_list[i] = bernoulli_rv(0.5);
  }
  ave = average(rv_dist_list, 10);
  u_var = unbiased_variance(rv_dist_list, 10);
  u_sd = unbiased_standard_deviation(rv_dist_list, 10);

  // 理論値
  t_ave = 0.5;
  t_u_var = 0.25;
  t_u_sd = 0.5;

  // 出力
  printf("\n「コインを投げる」試行を10回繰り返した時の標本平均・分散・標準偏差を、母集団のそれと比較する\n\n");
  printf("標本平均: %f, 標本分散: %f, 標本標準偏差: %f\n", ave, u_var, u_sd);
  printf("母平均: %f, 母分散: %f, 母標準偏差: %f\n\n", t_ave, t_u_var, t_u_sd);
  */



  // ----------------------- EX.6-2 ----------------------- //
  // 「サイコロをふる」試行を10回繰り返した時、出た目の標本平均・分散・標準偏差を、母集団のそれと比較する

  /*
  // 標本平均・標本分散・標本標準偏差を求める
  for(i=0; i<10; i++){
    // [0, 1)の一様分布を使ってサイコロの出目を記録
    rv_dist_list[i] = fmod(s_uniform_rv() * 6, 6.0) + 1;
  }
  ave = average(rv_dist_list, 10);
  u_var = unbiased_variance(rv_dist_list, 10);
  u_sd = unbiased_standard_deviation(rv_dist_list, 10);

  // 理論値
  t_ave = 3.5;
  t_u_var = 2.92;
  t_u_sd = 1.71;

  // 出力
  printf("\n「サイコロをふる」試行を10回繰り返した時、出た目の標本平均・分散・標準偏差を、母集団のそれと比較する\n\n");
  printf("標本平均: %f, 標本分散: %f, 標本標準偏差: %f\n", ave, u_var, u_sd);
  printf("母平均: %f, 母分散: %f, 母標準偏差: %f\n\n", t_ave, t_u_var, t_u_sd);
  */
  


  // ----------------------- EX.6-3 ----------------------- //
  // 「表2-1から無作為に10コのデータを復元抽出した」時、出た目の標本平均・分散・標準偏差を、母集団のそれと比較する
  /*
  // 入力データを配列にいれる
  for(i=0; i<100; i++){
    scanf("%lf", &data_list[i]);
  }

  // 標本平均・標本分散・標本標準偏差を求める
  for(i=0; i<10; i++){
    // [0, 1)の一様分布を使って抽出する配列番号を記録
    index = fmod(s_uniform_rv() * 100, 100.0);

    // 対応する配列番号の配列を抽出
    rv_dist_list[i] = data_list[index];
  }

  ave = average(rv_dist_list, 10);
  u_var = unbiased_variance(rv_dist_list, 10);
  u_sd = unbiased_standard_deviation(rv_dist_list, 10);

  // 理論値
  t_ave = average(data_list, 100);
  t_u_var = unbiased_variance(data_list, 100);
  t_u_sd = unbiased_standard_deviation(data_list, 100);

  // 出力
  printf("\n「表2-1から無作為に10コのデータを復元抽出した」時、出た目の標本平均・分散・標準偏差を、母集団のそれと比較する\n\n");
  printf("標本平均: %f, 標本分散: %f, 標本標準偏差: %f\n", ave, u_var, u_sd);
  printf("母平均: %f, 母分散: %f, 母標準偏差: %f\n\n", t_ave, t_u_var, t_u_sd);
  */



  // ----------------------- HW6 ----------------------- //
  // 「表2-1から無作為に10コのデータを復元抽出し、標本平均・分散・標準偏差を求める」
  // 　試行を1000回繰り返した時、標本平均の平均、標本分散の平均、標本標準偏差の平均、標本平均の分散を
  //   求め、母集団のそれと比較する。

  // 入力データを配列にいれる
  for(i=0; i<1000; i++){
    scanf("%lf", &data_list[i]);
  }

  // 標本平均の平均・標本分散の平均・標本標準偏差の平均、標本平均の分散を求める
  // 初期化
  ave = 0.0;
  u_var = 0;
  u_sd = 0;
  ave_u_var = 0;

  for(j=0; j<1000; j++){
    for(i=0; i<10; i++){
      // [0, 1)の一様分布を使って抽出する配列番号を記録
      index = fmod(s_uniform_rv() * 100, 100);

      // 対応する配列番号の配列を抽出
      rv_dist_list[i] = data_list[index];
    }

    ave_list[j] = average(rv_dist_list, 10);
    ave += ave_list[j];
    u_var += unbiased_variance(rv_dist_list, 10);
    u_sd += unbiased_standard_deviation(rv_dist_list, 10);
    
  }

  ave = ave / 1000.0;
  u_var /= 1000.0;
  u_sd /= 1000.0;
  ave_u_var = unbiased_variance(ave_list, 1000);


  // 理論値
  t_ave = 4.550;
  t_u_var = 0.152;
  t_u_sd = 0.390;
  t_ave_u_var = 0.0152;

  // 出力
  printf("\n「表2-1から無作為に10コのデータを復元抽出し、標本平均・分散・標準偏差を求める」\n");
  printf("試行を1000回繰り返した時、標本平均の平均、標本分散の平均、標本標準偏差の平均、");
  printf("\n標本平均の分散を求め、母集団のそれと比較する。\n\n");
  printf("標本平均の平均: %f\n標本分散の平均: %f\n標本標準偏差の平均: %f\n標本平均の分散 %f\n\n", ave, u_var, u_sd, ave_u_var);
  printf("母平均: %f\n母分散: %f\n母標準偏差: %f\n標本平均の分散の期待値: %f\n\n", t_ave, t_u_var, t_u_sd, t_ave_u_var);


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

  // k=0の時は、nP-1が定義されないので例外扱い
  if(k == 0){
    return 1;
  }

  return partial(n, k) / factorial(k);
}


// せっかくなのでcombination自体の再帰でnCkを求める方法も考えてみる
// パスカルの三角形を使う
int combination2(int n, int k){
  // 負の引数およびn<kの時はエラー
  if(n < 0 || k < 0 || n < k){
    return -1;
  }

  // パスカルの三角形の端（k=0とk=n）は1
  if(k==0 || k==n){
    return 1;
  }

  return combination2(n-1, k) + combination2(n-1, k-1);
}


// 二項分布Bin(p, n)の、k=0, 1, 2,..., nにおける理論値を配列として返す
int binomial_tv(double p, int n, double *list){
  int k;

  for(k=0; k<=n; k++){
    list[k] = combination2(n, k) * pow(p, k) * pow(1-p, n-k);
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
    if(k>lambda && (list[k]<0 || list[k-1]<list[k])){
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
    list[t] = 1.0 * combination2(s, t) * combination2(n-s, k-t) / combination2(n, k);
  }

  return 0;
}


