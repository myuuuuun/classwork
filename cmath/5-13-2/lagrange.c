#include <stdio.h>
#include "matrix.h"

double lagrange(double x, matrix points);

/*
int main(){
  int i, j, order;
  matrix points, a;

  order = 2;

  mat_alloc(&points, 3, 2);
  mat_alloc(&a, order+1, 1);

  points.element[0][0] = -1;
  points.element[0][1] = 1;
  points.element[1][0] = 0;
  points.element[1][1] = 0;
  points.element[2][0] = 1;
  points.element[2][1] = 1;
  mat_print(points);
  least_square(&a, points, order);

  mat_print(a);
  mat_free(&a);
  mat_free(&points);
  return 0;
}
*/

double lagrange(double x, matrix points){
	int i, j;
  double sigma, product;
  
  sigma = 0;
  for(i=0; i<points.row; i++){
    product = points.element[i][1];

    for(j=0; j<points.row; j++){
      if(i != j){
        product *= (x - points.element[j][0]) / (points.element[i][0] - points.element[j][0]);
      }
    }

    sigma += product;
  }

  return sigma;
}
