#ifndef _MATRIX_H

#define _MATRIX_H
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

void printe(char message[99]);
int mat_alloc(matrix *mat, int row, int col);
void mat_free(matrix *mat);
void mat_print(matrix mat);
int check(matrix mat1, matrix mat2);
int check_mul(matrix mat1, matrix mat2, matrix mat3);
int check_trans(matrix mat1, matrix mat2);
int check_square(matrix mat);

int mat_copy(matrix *mat1, matrix mat2);
int mat_add(matrix *mat1, matrix mat2, matrix mat3);
int mat_sub(matrix *mat1, matrix mat2, matrix mat3);
int mat_mul(matrix *mat1, matrix mat2, matrix mat3);
int mat_muls(matrix *mat1, matrix mat2, double c);
int mat_trans(matrix *mat1, matrix mat2);
int mat_unit(matrix *mat);
int mat_compare(matrix mat1, matrix mat2);
int swap_rows(matrix *mat, int n, int m);
int pivot(matrix *a, matrix *b, int n);

int mat_solve(matrix *mat1, matrix mat2, matrix mat3);
int mat_inverse(matrix *mat1, matrix mat2);

#endif