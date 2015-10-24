#include <stdio.h>

int main(void)
{
  int i;
  float sum, average;
  sum = 0;

  for(i = 1; i <= 100; i++)
  {
    float input;
    scanf("%f", &input);

    sum += input;
  }

  average = sum / 100.0;

  printf("%f \n", average);

  return 0;
}