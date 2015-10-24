/* 152042 松下 旦 */

#include <stdio.h>

int main(void)
{
  double x, sum;
  int i;
  for(i = 0; i < 8; i++)
  {
    scanf("%lf", &x);
    sum += x;
  }
  printf("the answer is... \n");
  printf("%f ", sum);
  printf("\n");

  return 0;
}