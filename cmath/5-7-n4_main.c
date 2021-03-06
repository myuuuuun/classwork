#include <stdio.h>
#include "5-7-n4.h"


int main()
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




