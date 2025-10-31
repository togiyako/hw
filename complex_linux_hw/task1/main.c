#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lib/calc.h"

void print_menu() 
{
    printf("\n--- Calculator ---\n");
    printf("Addition       - 1\n");
    printf("Subtraction    - 2\n");
    printf("Multiplication - 3\n");
    printf("Division       - 4\n");
    printf("Factorial      - 5\n");
    printf("Square Root    - 6\n");
    printf("Exit           - 0\n");
    printf("Select operation: ");
}

int main() 
{
    int choice;
    double num1, num2, result;
    int num_int;

    while (1) 
    {
        print_menu();

        if (scanf("%d", &choice) != 1) 
        {
            printf("Invalid input, please enter a number.\n");
            while (getchar() != '\n'); 
            continue;
        }

        if (choice == 0) 
        {
            break;
        }

        if (choice >= 1 && choice <= 4) 
        {
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &num1, &num2) != 2) 
            {
                printf("Invalid input for numbers!\n");
                while (getchar() != '\n');
                continue;
            }
        } 
        else if (choice == 5 || choice == 6) 
        {
            printf("Enter one number: ");
            if (scanf("%lf", &num1) != 1) 
            {
                printf("Invalid input for number!\n");
                while (getchar() != '\n');
                continue;
            }
        } else 
        {
            printf("Invalid operation, please choose a number from 0 to 6.\n");
            continue;
        }

        switch (choice) 
        {
            case 1:
                result = add(num1, num2);
                printf("\nResult: %.2f\n", result);
                break;
            case 2:
                result = sub(num1, num2);
                printf("\nResult: %.2f\n", result);
                break;
            case 3:
                result = mul(num1, num2);
                printf("\nResult: %.2f\n",result);
                break;
            case 4:
                result = division(num1, num2);
		printf("\nResult:%.2f\n", result);
                break;
            case 5:
                num_int = (int)num1;
                if (num1 != num_int) 
                {
                     printf("Factorial can only be calculated for an integer.\n");
                } 
                else 
                {
                    long long fact_result = factorial(num_int);
                    
                    if (fact_result == -1)
                    {
                        printf("Factorial can only be calculated for a non-negative number.\n");
                    }
                    else if (fact_result == -2)
                    {
                        printf("Error: Input %d is too large, result will overflow.\n", num_int);
                    }
                    else 
                    {
                        printf("\nResult: %lld\n", fact_result);
                    }
                }
                break;
            case 6:
                result = square_root(num1);
                if (isnan(result)) 
                {
                    printf("Cannot calculate the square root of a negative number.\n");
                } 
                else 
                {
                    printf("\nResult:%.2f\n", result);
                }
                break;
            default:
                printf("Invalid operation, try again.\n");
                break;
        }
    }

    return 0;
}

