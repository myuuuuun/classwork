#include <math.h>
#include "matrix.h"

int least_square(matrix *a, matrix points, int order);

/*
int main(){
	int i, j, order;
	matrix points, a;

	order = 1;

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

/*

g: (n+1 Å~ m+1)		a: (m+1 Å~ 1)		f: (n+1 Å~ m+1)

( x0^0  x0^1  x0^2  ...  x0^m )     ( a0 )     ( f0 )
( x1^0  x1^1  x1^2  ...  x1^m )     ( a1 )     ( f1 )
( 						 .							)     (  . )     (  . )
( 						 .							)  *  (  . )  =  (  . )
( 						 .							)     ( am )     (  . )
( xn^0  xn^1  xn^2  ...  xn^m )                ( fn )

*/
int least_square(matrix *a, matrix points, int order){
	int i, j, k, n;
	double sigma;
	matrix g, f;

	//n = points.row;
	mat_alloc(&g, order+1, order+1);
	mat_alloc(&f, order+1, 1);

	// gÇãÅÇﬂÇÈ
	for(i=0; i<g.row; i++){
		for(j=0; j<g.col; j++){
			sigma = 0.0;
			for(k=0; k<points.row; k++){
				sigma += pow(points.element[k][0], i) * pow(points.element[k][0], j);
			}
			g.element[i][j] = sigma;
		}
	}

	// fÇãÅÇﬂÇÈ
	for(i=0; i<f.row; i++){
		sigma = 0;
		for(k=0; k<points.row; k++){
			sigma += pow(points.element[k][0], i) * points.element[k][1];
		}
		f.element[i][0] = sigma;
	}

	// aÇåvéZ
	mat_solve(a, g, f);

	mat_free(&g);
	mat_free(&f);

	return 0;
}
