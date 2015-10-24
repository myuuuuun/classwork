//
//  s2.c
//  
//
//  Created by akira on 2015/04/08.
//
//

#include <stdio.h>

int main(void)
{
    int i, j, sum, dif, pro, quo, res;
    double x, y, rat;
    
    for(i=1; i<5; i++)
    {
        for(j=1; j<5; j++)
        {
            x = (double)i;
            y = (double)j;
            sum = i + j;
            dif = i - j;
            pro = i * j;
            quo = i / j;
            res = i % j;
            rat = x / y;
            printf("i = %d, j = %d: ", i, j);
            printf("%2d %2d %2d %2d %2d %5.2f\n", sum , dif, pro, quo, res, rat);
        }
    }
    return 0;
}