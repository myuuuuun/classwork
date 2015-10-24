#include <stdio.h>
#include <math.h>
#define EPSILON 1.0e-12

int main(void){
  double a, b, c;
  scanf("%lf%lf%lf", &a, &b, &c);
  printf("%s\n", (fabs(a-b-c) < EPSILON) ? "a-b == c" : "a-b != c");
  return 0;
}