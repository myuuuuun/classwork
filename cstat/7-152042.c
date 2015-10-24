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

int confidence_interval(double z, double average, double sd, int n, double *result_list);
int confidence_interval_2(double z_lower, double z_upper, double average, double var, int n, double *result_list);



int main(void){
  int i, j, n, bin, count, index, size, count_ave, count_var, count_ave2;
  double rv_dist_list[1001]={0}; //0で初期化
  double data_list[1001]={0}; //0で初期化
  double ave_list[1001]={0}; //0で初期化
  double var_list[1001]={0};
  double sd_list[1001]={0};

  double ave_cd_upper_list[1001]={0};
  double ave_cd_lower_list[1001]={0};
  double var_cd_upper_list[1001]={0};
  double var_cd_lower_list[1001]={0};

  double height_list[100]={0};
  double weight_list[100]={0};
  double result_list[2]={0};
  double result_list2[2]={0};
  double result_list3[3]={0};
  double result_list1_h[2]={0};
  double result_list1_w[2]={0};
  double result_list2_h[2]={0};
  double result_list2_w[2]={0};
  double z, z1, z2_1, z2_2, ave, var, sd, sd_h, sd_w, ave_h, ave_w, var_h, var_w, mu, sigma2;


  // 乱数の初期化
  srand(time(0));
  //srand(2042);

  // ----------------------- EX.7-1 ----------------------- //
  // 表2-1(file2)のデータから大きさ10の標本を無作為抽出し、95%信頼区間を求める。
  // ただしx1, x2,... , iid ~ N(μ, 0.390^2) を仮定する。

  /*
  // 入力データを配列にいれる
  for(i=0; i<100; i++){
    scanf("%lf", &data_list[i]);
  }

  for(i=0; i<10; i++){
    // [0, 1)の一様分布を使って抽出する配列番号を記録
    index = fmod(s_uniform_rv() * 100, 100);

    // 対応する配列番号の配列を抽出
    rv_dist_list[i] = data_list[index];
  }

  z =  1.96;
  ave = average(rv_dist_list, 10);
  sd = 0.390;

  confidence_interval(z, ave, sd, 10, result_list);

  printf("95%%信頼区間は、%f ≦ μ ≦ %f\n", result_list[0], result_list[1]);
  */
  

  // ----------------------- EX.7-2 ----------------------- //
  // 表7-1は日本人成人男子の無作為標本である。
  // 身長と体重の母平均、母分散の推定値と95%信頼区間を求めよ。

  /*
  // 標本数
  scanf("%d", &n);

  // 入力データを配列にいれる
  for(i=0; i<n; i++){
    scanf("%lf", &height_list[i]);
    scanf("%lf", &weight_list[i]);
  }

  ave_h = average(height_list, n);
  ave_w = average(weight_list, n);
  sd_h = unbiased_standard_deviation(height_list, n);
  sd_w = unbiased_standard_deviation(weight_list, n);
  var_h = unbiased_variance(height_list, n);
  var_w = unbiased_variance(weight_list, n);

  z1 = 2.2281;
  z2_1 = 3.2470;
  z2_2 = 20.4832;

  confidence_interval(z1, ave_h, sd_h, n, result_list1_h);
  confidence_interval(z1, ave_w, sd_w, n, result_list1_w);
  confidence_interval_2(z2_1, z2_2, ave_h, var_h, n, result_list2_h);
  confidence_interval_2(z2_1, z2_2, ave_h, var_w, n, result_list2_w);

  printf("\n身長の母平均の推定値は、%f, 母分散の推定値は %f\n", ave_h, var_h);
  printf("平均の95%%信頼区間は、%f ≦ μ ≦ %f, 分散の95%%信頼区間は、%f ≦ σ ≦ %f\n", result_list1_h[0], result_list1_h[1], result_list2_h[0], result_list2_h[1]);

  printf("\n体重の母平均の推定値は、%f, 母分散の推定値は %f\n", ave_w, var_w);
  printf("平均の95%%信頼区間は、%f ≦ μ ≦ %f, 分散の95%%信頼区間は、%f ≦ σ ≦ %f\n\n", result_list1_w[0], result_list1_w[1], result_list2_w[0], result_list2_w[1]);
  */


  // ----------------------- HW.7 ----------------------- //
  // 「表2-1(file2)から大きさ10の標本を無作為抽出し、平均と分散の95%信頼区間を求める（母平均, 母分散とも未知とする）」過程を1000回くりかえし、
  // そのうちμ = 4.55, σ^2 = 0.152 というデータが1000回のうち何回95%信頼区間に含まれるかをカウント。
  // また、標準化した標本平均が仮に、t分布ではなく、N(0, 1)に従うとして、同様の実験を行う。

  // 入力データを配列にいれる
  for(i=0; i<100; i++){
    scanf("%lf", &data_list[i]);
  }

  mu = 4.55;
  sigma2 = 0.152;

  z1 = 2.2281;
  z2_1 = 3.2470;
  z2_2 = 20.4832;
  z = 1.96;

  count_ave = 0;
  count_var = 0;
  count_ave2 = 0;

  for(j=0; j<1000; j++){
    for(i=0; i<10; i++){
      // [0, 1)の一様分布を使って抽出する配列番号を記録
      index = fmod(s_uniform_rv() * 100, 100);

      // 対応する配列番号の配列を抽出
      rv_dist_list[i] = data_list[index];
    }

    ave = average(rv_dist_list, 10);
    var = unbiased_variance(rv_dist_list, 10);
    sd = unbiased_standard_deviation(rv_dist_list, 10);

    confidence_interval(z1, ave, sd, 10, result_list);
    confidence_interval_2(z2_1, z2_2, ave, var, 10, result_list2);
    confidence_interval(z, ave, sd, 10, result_list3);

    if(result_list[0] <= mu && mu <= result_list[1]){
      count_ave++;
    }

    if(result_list2[0] <= sigma2 && sigma2 <= result_list2[1]){
      count_var++;
    }

    if(result_list3[0] <= mu && mu <= result_list3[1]){
      count_ave2++;
    }

  }

  printf("\n母平均・母分散の推定に、t分布、χ^2分布を適切に使った場合\n");
  printf("μ=4.55 が95%%信頼区間に入った回数は、1000回中 %d 回, σ^2=0.152は %d 回\n", count_ave, count_var);

  printf("\n母平均の推定に、誤ってN(0, 1)を使った場合\n");
  printf("μ=4.55 が95%%信頼区間に入った回数は、1000回中 %d 回\n\n", count_ave2);

  


  return 0;
}



/*----------------  以下関数集  ----------------*/

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


// 95%信頼区間を求める(左右対称な分布)
int confidence_interval(double z, double ave, double sd, int n, double *result_list){
  double upper, lower;

  lower = ave - z * sd / sqrt(n);
  upper = ave + z * sd / sqrt(n);

  result_list[0] = lower;
  result_list[1] = upper;

  return 0;
}


// 95%信頼区間を求める（カイ二乗分布）
int confidence_interval_2(double z_lower, double z_upper, double average, double var, int n, double *result_list){
  double upper, lower;

  lower = (n - 1) * var / z_upper;
  upper = (n - 1) * var / z_lower;

  result_list[0] = lower;
  result_list[1] = upper;

  return 0;
}



