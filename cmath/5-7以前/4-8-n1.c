#include <stdio.h>

int main(void){
  double a, b, c;
  scanf("%lf%lf%lf", &a, &b, &c);
  printf("%s\n", (a-b == c) ? "a-b == c" : "a-b != c");
  return 0;
}