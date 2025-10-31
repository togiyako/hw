#include "calc.h"

long long factorial(int n) 
{
    if(n < 0)
    {
        return -1;
    }
    
    if( n > 20)
    {
    	return -2;
    }
    	
    if(n == 0) {
        return 1;
    }

    long long result = 1;

    for(int i = 1; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

