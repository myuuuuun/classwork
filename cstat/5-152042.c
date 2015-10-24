/* 152042 松下 旦 */
// 6/3 - poison_tv()の桁あふれの問題を修正

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

int test(void);
void mat_free(matrix *mat);
int mat_alloc(matrix *mat, int row, int col);

double average(double *input, int input_size);
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
  int i, j, k, bin, count, male, female, is_female;
  int rv_dist_list[1001]={0}; //0で初期化
  double tv_dist_list[31]={0}; //0で初期化

  // 乱数の初期化
  srand(time(0));

  // 関数のテスト
  //test();


  // ----------------------- Ex.5-1 ----------------------- //
  
  // 「p=0.005のベルヌーイ試行を500回行う」試行を1000回
  /*
  for(i=0; i<1000; i++){

    bin = binomial_rv(0.005, 500);
    rv_dist_list[bin] += 1;
  }

  // ポアソン分布~Bin(0.5, 10)の理論値を配列に記録
  poison_tv(2.5, tv_dist_list, 31);

  // 理論値と実測値を合わせて出力
  printf("\n「p=0.005のベルヌーイ試行を500回行い成功回数を数える」試行を1000回繰り返したとき、\n");
  for(k=0; k<=30; k++){
    printf("「%d回成功する」回数の　理論値は%f　実測値は%d\n", k, tv_dist_list[k] * 1000, rv_dist_list[k]);
  }
  printf("\nとなりました。（実測値は31回以上成功するケースを省略。）\n");
  */



  // ----------------------- Ex.5-2 ----------------------- //
  
  /*
  // 「コインを繰り返しなげ、はじめて表が出るまでにかかった回数を数える」試行を1000回
  rv_dist_list[0] = 0;
  for(i=0; i<1000; i++){
    count = 0;
    for(j=0; j<MAX_LOOP; j++){
      if(bernoulli_rv(0.5) == 1){
        count++;
        break;
      }
      count++;
    }
    rv_dist_list[count] += 1;
  }

  // 幾何分布~Geo(0.5)の理論値を配列に記録
  geometric_tv(0.5, tv_dist_list, 31);

  // 理論値と実測値を合わせて出力
  printf("\n「コインを繰り返しなげ、はじめて表が出るまでにかかった回数を数える」試行を1000回繰り返したとき、\n");
  for(k=1; k<=30; k++){
    printf("「%d回目に初めて表が出る」回数の　理論値は%f　実測値は%d\n", k, tv_dist_list[k] * 1000, rv_dist_list[k]);
  }
  printf("\nとなりました。（実測値は31回以上かかるケースを省略。）\n");
  */
  


  // ----------------------- HW.5 ----------------------- //
  
  // 「オス10匹、メス10匹　のいけすから10匹を非復元抽出して、メスの数を数える」試行を1000回
  for(i=0; i<1000; i++){
    count = 0;
    male = 10; female = 10;

    for(j=0; j<10; j++){
      is_female = bernoulli_rv(1.0 * female / (male + female));
      if(is_female == 1){
        female--;
        count++;
      } else{
        male--;
      }
    }
    rv_dist_list[count] += 1;
  }

  // 超幾何分布~HyperGeo(20, 10, 10)の理論値を配列に記録
  hypergeometric_tv(20, 10, 10, tv_dist_list);

  // 理論値と実測値を合わせて出力
  printf("\n「オス10匹、メス10匹　のいけすから10匹を非復元抽出して、メスの数を数える」試行を1000回繰り返したとき、\n");
  for(k=0; k<=10; k++){
    printf("「メスが%d匹いる」試行回数の　理論値は%f　実測値は%d\n", k, tv_dist_list[k] * 1000, rv_dist_list[k]);
  }
  printf("\nとなりました。\n");
  
  

  return 0;
}

// テスト用関数
int test(void){
  int i;
  matrix t;

  if(mat_alloc(&t, 10, 2) < 0){
    return -1;
  };

  t.element[0][0] = 4;  t.element[0][1] = 0;
  t.element[1][0] = 4;  t.element[1][1] = 1;
  t.element[2][0] = 4;  t.element[2][1] = 2;
  t.element[3][0] = 4;  t.element[3][1] = 3;
  t.element[4][0] = 4;  t.element[4][1] = 4;
  t.element[5][0] = 0;  t.element[5][1] = 0;
  t.element[6][0] = 0;  t.element[6][1] = 3;
  t.element[7][0] = 2;  t.element[7][1] = 3;
  t.element[8][0] = -1; t.element[8][1] = 6;
  t.element[9][0] = 6;  t.element[9][1] = -1;

  for(i=0; i<t.row; i++){
    printf("nCk (n=%d, k=%d): %d\n", (int)t.element[i][0], (int)t.element[i][1], combination2(t.element[i][0], t.element[i][1]));
  }

  mat_free(&t);

  return 0;

}


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


// 0〜1までの一様分布を返す
double s_uniform_rv(void){
  float rndm;
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











