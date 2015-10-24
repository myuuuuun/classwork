#include <stdio.h>
#include "5-7.h"


// エラー出力用関数
void printe(char message[99]){
  fprintf(stderr, "\n%s %s\n%s", RED BOLD "ERROR:" STANDARD_C BOLD, message, STANDARD"");
}


// 配列のアドレスを確保
int mat_alloc(matrix *mat, int row, int col){
  int i, j;

  mat->row = row;
  mat->col = col;
  mat->element = calloc(row, sizeof(double*));
  mat->base_element = calloc(col * row, sizeof(double));

  if (mat->element == NULL || mat->base_element == NULL){
    printe("配列用の領域が確保できませんでした。 >>mat_alloc()");
    return -1;
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
    return -1;
  }
  return 0;
}


// mat1 = mat2 * mat3
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


// mat2をmat1へコピー
int mat_copy(matrix *mat1, matrix mat2){
  int i,j;

  if(check(*mat1, mat2) != 0){
    printe("引数の行列のサイズが一致しません。 >>mat_copy()");
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
    printe("引数の行列のサイズが一致しません。 >>mat_add()");
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
    printe("引数の行列のサイズが一致しません。 >>mat_sub()");
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
    printe("引数の行列のサイズが不適切です。 >>mat_mul()");
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
    printe("引数の行列のサイズが一致しません。 >>mat_muls()");
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
    printe("引数の行列のサイズが不適切です。 >>mat_trans()");
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
    printe("引数の行列が正方行列ではありません。 >>mat_unit()");
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
    printf("%s\n", "2つの行列は等しくありません");
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


// n行目とm行目を入れ替える
int swap_rows(matrix *mat, int n, int m){
  int i;
  double s;

  for(i=0; i<mat->col; i++){
    s = mat->element[n][i];
    mat->element[n][i] = mat->element[m][i];
    mat->element[m][i] = s;
  }

  return 0;
}


// aのn+1列目に対してピボット選択を行い、a, bに関して行を入れ替える
int pivot(matrix *a, matrix *b, int n){
  int i, max_row;
  max_row = n;

  // n+1行目が最初の行。n+1行目以降にn+1行目よりもn+1列目の値の絶対値が大きい行がないか探す。
  for(i=n+1; i<a->row; i++){
    if(fabs(a->element[max_row][n] - a->element[i][n]) > MAT_EPSILON){
      max_row = i;
    }
  }

  // 最大ピボットが0の時、rankが行数より少ないため解けない。
  if(fabs(a->element[max_row][n]) < MAT_EPSILON){
    return -1;
  }

  // n行目が最大ピボットでなければ、行を入れ替える
  if(max_row != n){
    swap_rows(a, n, max_row);
    swap_rows(b, n, max_row);
  }

  return 0;
}


// mat2(A) * ? = mat3(B) を求める
int mat_solve(matrix *mat1, matrix mat2, matrix mat3){
  int i, j, k, l;
  double multiplier, divisor;
  matrix a, b;

  if(check_square(mat2) != 0 || check_mul(*mat1, mat2, mat3)){
    printe("引数の行列のサイズが不適切です。 >>mat_solve()");
    return -1;
  }
  
  mat_alloc(&a, mat2.row, mat2.col);
  mat_alloc(&b, mat3.row, mat3.col);
  mat_copy(&a, mat2);
  mat_copy(&b, mat3);

  // Gaussの消去法
  for(i=0; i<a.row; i++){
    if(pivot(&a, &b, i) != 0){
      printe("この問題は解けません。 >>mat_solve()");
      goto ERROR;
    }
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

  ERROR:

  mat_free(&a);
  mat_free(&b);

  return 0;
}


// 逆行列を求める
int mat_inverse(matrix *mat1, matrix mat2){
  matrix square;

  if(check_square(mat2) != 0 || check_square(*mat1) != 0){
    return -1;
  }

  mat_alloc(&square, mat2.row, mat2.col);
  mat_unit(&square);
  mat_solve(mat1, mat2, square);
  mat_free(&square);

  return 0;
}








