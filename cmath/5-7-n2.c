#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define RED "\033[31m" // 赤字
#define BOLD "\033[1m" // 太字
#define STANDARD "\033[0m" // 標準文字色・スタイル
#define STANDARD_C "\033[0m" // 標準文字色
#define MAT_EPSILON 1.0e-8
#define mat_elem(mat, i, j)  ((mat).element[(i)-1][(j)-1])

typedef struct{
  int row;
  int col;
  double **element;
  double *base_element;
} matrix;


int mat_alloc(matrix *mat, int row, int col);
void mat_free(matrix *mat);
void mat_print(matrix mat);
int check(matrix mat1, matrix mat2);
int check_mul(matrix mat1, matrix mat2, matrix mat3);
int check_trans(matrix mat1, matrix mat2);
int check_square(matrix mat);
int check_base(matrix mat, int row, int col);

int mat_copy(matrix *mat1, matrix mat2);
int mat_add(matrix *mat1, matrix mat2, matrix mat3);
int mat_sub(matrix *mat1, matrix mat2, matrix mat3);
int mat_mul(matrix *mat1, matrix mat2, matrix mat3);
int mat_muls(matrix *mat1, matrix mat2, double c);
int mat_trans(matrix *mat1, matrix mat2);
int mat_unit(matrix *mat);
int mat_compare(matrix mat1, matrix mat2);

int mat_solve(matrix *mat1, matrix mat2, matrix mat3);
int mat_inverse(matrix *mat1, matrix mat2);


main()
{
        matrix a;
        
        if (mat_alloc(&a, 3, 3)) {
                printf("allocation error\n");
                exit(-1);
        }

#ifndef FROM0
        mat_elem(a, 1, 1) = 2;
        mat_elem(a, 1, 2) = 3;
        mat_elem(a, 1, 3) = 1;
        mat_elem(a, 2, 1) = 4;
        mat_elem(a, 2, 2) = 1;
        mat_elem(a, 2, 3) = -3;
        mat_elem(a, 3, 1) = -1;
        mat_elem(a, 3, 2) = 2;
        mat_elem(a, 3, 3) = 1;
#else
        mat_elem(a, 0, 0) = 2;
        mat_elem(a, 0, 1) = 3;
        mat_elem(a, 0, 2) = 1;
        mat_elem(a, 1, 0) = 4;
        mat_elem(a, 1, 1) = 1;
        mat_elem(a, 1, 2) = -3;
        mat_elem(a, 2, 0) = -1;
        mat_elem(a, 2, 1) = 2;
        mat_elem(a, 2, 2) = 1;
#endif

        if ( mat_inverse(&a, a) ) {
                printf("Cannot make inverse matrix\n");
                exit(-1);
        }

        printf("Inverse matrix is:\n");
        mat_print(a);

        mat_free(&a);

        return 0;
}

/*
int main(){
  int i, j;
  matrix mat, matt, mat1;

  mat_alloc(&mat, 3, 3);
  mat_alloc(&matt, 3, 2);
  mat_alloc(&mat1, 3, 2);

  
  for(i=0; i < mat.row; i++){
    for(j=0; j < mat.col; j++){
      mat.element[i][j] = j + i * mat.col;
    }
  }

  for(i=0; i < matt.row; i++){
    for(j=0; j < matt.col; j++){
      matt.element[i][j] = j + i * matt.col;
    }
  }



    mat.element[0][0] = 1;
    mat.element[0][1] = -1;
    mat.element[0][2] = 1;
    matt.element[0][0] = -5;
    matt.element[0][1] = -5;

    mat.element[1][0] = 2;
    mat.element[1][1] = 1;
    mat.element[1][2] = -3;
    matt.element[1][0] = 19;
    matt.element[1][1] = 19;

    mat.element[2][0] = 3;
    mat.element[2][1] = 2;
    mat.element[2][2] = -1;
    matt.element[2][0] = 16;
    matt.element[2][1] = 16;

  

  //mat_copy(&mat1, mat);
  //mat_add(&mat1, mat, matt);
  //mat_sub(&mat1, mat, matt);
  //mat_mul(&mat1, mat, matt);
  //mat_muls(&mat1, mat, 3);

  mat_solve(&mat1, mat, matt);

  //mat_mul(&matt, mat, matt);
  //mat_unit(&mat);
  //printf("%d \n", mat_compare(mat, matt));


  
  mat_print(mat1);

  mat_free(&mat1);
  mat_free(&matt);
  mat_free(&mat);


  return 0;
}
*/


// 配列のアドレスを確保
int mat_alloc(matrix *mat, int row, int col){
  int i, j;

  mat->row = row;
  mat->col = col;
  mat->element = calloc(row, sizeof(double*));
  mat->base_element = calloc(col * row, sizeof(double));

  if (mat->element == NULL || mat->base_element == NULL){
    printf("配列用の領域が確保できませんでした\n");
    return(-1);
  }

  for(i=0; i<row; i++){
    mat->element[i] = &mat->base_element[i * col];
  }

  return 0;
}


// 配列用のメモリを解放
void mat_free(matrix *mat){
  free(mat->base_element);
  free(mat->element);
}


// 配列を行列形式でprintf
void mat_print(matrix mat){
  int i, j;

  printf("\n");
  for(i=0; i<mat.row; i++){
    for(j=0; j<mat.col; j++){
      printf("%10f ", mat.element[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}


// 行列のサイズが同じかチェック
int check(matrix mat1, matrix mat2){
  if(mat1.col != mat2.col || mat1.row != mat2.row){
    fprintf(stderr, "\n%s %s\n", RED BOLD "error:", STANDARD_C BOLD "行列のサイズが一致しません。>>check()" STANDARD);
    return -1;
  }
  return 0;
}


// 1. 行列2の列数と行列3の行数が一致するかチェック（掛け算用）
// 2. 行列2の行数・行列3の列数と行列1の行数列数が一致するかチェック（掛け算用）
int check_mul(matrix mat1, matrix mat2, matrix mat3){
  if(mat2.col != mat3.row || mat2.row != mat1.row || mat3.col != mat1.col){
    return -1;
  }
  return 0;
}


// 行列1の行列数と行列2の列行数が一致するかチェック（同順, 転置用）
int check_trans(matrix mat1, matrix mat2){
  if(mat1.col != mat2.row || mat1.row != mat2.col){
    return -1;
  }
  return 0;
}


// 正方行列かをチェック
int check_square(matrix mat){
  if(mat.col != mat.row){
    return -1;
  }
  return 0;
}


// 行列が指定した行数・列数かどうかチェック
int check_base(matrix mat, int row, int col){
  if(mat.col != col || mat.row != row){
    return -1;
  }
  return 0;
}


// mat2をmat1へコピー
int mat_copy(matrix *mat1, matrix mat2){
  int i,j;

  if(check(*mat1, mat2) != 0){
    return -1;
  }

  for(i=0; i < mat2.row; i++){
    for(j=0; j < mat2.col; j++){
      mat1->element[i][j] = mat2.element[i][j];
    }
  }

  return 0;
}


// 足し算
int mat_add(matrix *mat1, matrix mat2, matrix mat3){
  int i,j;

  if(check(*mat1, mat2) != 0 || check(*mat1, mat3) != 0){
    return -1;
  }

  for(i=0; i < mat2.row; i++){
    for(j=0; j < mat2.col; j++){
      mat1->element[i][j] = mat2.element[i][j] + mat3.element[i][j];
    }
  }

  return 0;
}


// 引き算
int mat_sub(matrix *mat1, matrix mat2, matrix mat3){
  int i,j;

  if(check(*mat1, mat2) != 0 || check(*mat1, mat3) != 0){
    return -1;
  }

  for(i=0; i < mat2.row; i++){
    for(j=0; j < mat2.col; j++){
      mat1->element[i][j] = mat2.element[i][j] - mat3.element[i][j];
    }
  }

  return 0;
}


// 掛け算
int mat_mul(matrix *mat1, matrix mat2, matrix mat3){
  int i, j, k;
  matrix mat2_w, mat3_w;
  
  if(check_mul(*mat1, mat2, mat3) != 0){
    printf("%s\n", "エラーが発生しました。");
    return -1;
  }

  mat_alloc(&mat2_w, mat2.row, mat2.col);
  mat_alloc(&mat3_w, mat3.row, mat3.col);
  mat_copy(&mat2_w, mat2);
  mat_copy(&mat3_w, mat3);
  
  for(i=0; i < mat1->row; i++){
    for(j=0; j < mat1->col; j++){
      mat1->element[i][j] = 0;

      for(k=0; k<mat2_w.col; k++){
        mat1->element[i][j] += mat2_w.element[i][k] * mat3_w.element[k][j];
      }
    }
  }

  mat_free(&mat2_w);
  mat_free(&mat3_w);
  
  return 0;
}


// スカラー倍
int mat_muls(matrix *mat1, matrix mat2, double c){
  int i, j;

  if(check(*mat1, mat2) != 0){
    printf("%s\n", "エラーが発生しました。");
    return -1;
  }
  
  for(i=0; i < mat2.row; i++){
    for(j=0; j < mat2.col; j++){
      mat1->element[i][j] = mat2.element[i][j] * c;
    }
  }

  return 0;
}


// 転置行列を返す
int mat_trans(matrix *mat1, matrix mat2){
  int i, j;
  matrix mat2_w;

  if(check_trans(*mat1, mat2) != 0){
    printf("%s\n", "エラーが発生しました。");
    return -1;
  }

  mat_alloc(&mat2_w, mat2.row, mat2.col);
  mat_copy(&mat2_w, mat2);
  
  for(i=0; i < mat1->row; i++){
    for(j=0; j < mat1->col; j++){
      mat1->element[i][j] = mat2_w.element[j][i];
    }
  }

  mat_free(&mat2_w);

  return 0;
}


// 正方行列なら単位行列を返す
int mat_unit(matrix *mat){
  int i, j;

  if(check_square(*mat) != 0){
    printf("%s\n", "エラーが発生しました。");
    return -1;
  }
  
  for(i=0; i < mat->row; i++){
    for(j=0; j < mat->col; j++){
      mat->element[i][j] = (i == j) ? 1 : 0;
    }
  }

  return 0;
}


// 2つの行列が同じか比較する
int mat_compare(matrix mat1, matrix mat2){
  int i, j;

  if(check(mat1, mat2) != 0){
    printf("%s\n", "行列数が一致しません。");
    return 0;
  }
  
  for(i=0; i < mat1.row; i++){
    for(j=0; j < mat1.col; j++){
      if(fabs(mat1.element[i][j] - mat2.element[i][j]) > MAT_EPSILON){
        printf("%s\n", "2つの行列は等しくありません");
        return 0;
      }
    }
  }
  printf("%s\n", "2つの行列は等しいです");
  return 1;
}


// アレする
int mat_solve(matrix *mat1, matrix mat2, matrix mat3){
  int i, j, k, l;
  double multiplier, divisor;
  matrix a, b;

  if(check_square(mat2) != 0 || mat2.row != mat3.row || mat1->row != mat3.row || mat1->col != mat3.col ){
    return -1;
  }
  
  mat_alloc(&a, mat2.row, mat2.col);
  mat_alloc(&b, mat3.row, mat3.col);

  mat_copy(&a, mat2);
  mat_copy(&b, mat3);

  mat_print(a);
  mat_print(b);


// Gaussの消去法
  for(i=0; i<a.row; i++){
    // i+1行目以降のi列目を消す（i=0のとき、2行目〜最終行の1列目を消去）
    for(j=i+1; j<a.row; j++){
      // j行目に、i行目を何倍したら、i列目が消えるか（例えば、i=0のとき、2行目以降の行全体に、何倍すれば、1列目が0になるか）
      multiplier = a.element[j][i] / a.element[i][i];
      for(k=0; k<a.col; k++){
        a.element[j][k] = a.element[j][k] - a.element[i][k] * multiplier;
      }

      for(l=0; l<b.col; l++){
        b.element[j][l] = b.element[j][l] - b.element[i][l] * multiplier;
      }
    }

  }

// 後退代入

  // 最終行以前の行に対して
  for(i=a.row-1; i>=0; i--){

    // 例えば、今2行目なら、3列目〜最終列に対して処理を行う
    for(j=a.row-1; j>i; j--){
      divisor = a.element[i][j];
      a.element[i][j] = a.element[i][j] - divisor * a.element[j][j];
      for(l=0; l<b.col; l++){
        b.element[i][l] = b.element[i][l] - divisor * b.element[j][l];
      }
    }

    for(l=0; l<b.col; l++){
      b.element[i][l] = b.element[i][l] / a.element[i][i];
    }
    a.element[i][i] = 1.0;
  }
  mat_copy(mat1, b);

  mat_free(&a);
  mat_free(&b);

  return 0;
}


int mat_inverse(matrix *mat1, matrix mat2){
  matrix square;

  if(check_square(mat2) != 0 || check_square(*mat1) != 0){
    return -1;
  }

  mat_alloc(&square, mat2.row, mat2.col);
  mat_unit(&square);

  mat_solve(mat1, mat2, square);

  mat_print(square);
  mat_print(mat2);
  mat_print(*mat1);

  return 0;
}








