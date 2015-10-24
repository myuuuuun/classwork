#include "s1.h"
#include <stdio.h>

int main(void){
    int i;
    double x;
    for (i = 0; i < 10; i++){
        x = (double) i;
        printf("i = %d, x = %f\n", i, x);
    }
    return 0;
};