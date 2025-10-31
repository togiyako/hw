#include <stdio.h>
#include "sum.h"

int main()
{
    double a = 12.5;
    double b = 10;
    
    double result = sum(a, b);
    printf("\nResult: %.2f\n", result);
    return 0;
}
