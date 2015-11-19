#include <stdio.h>

long double factorial(int n)
{
    long double result = 1.0;
    int i;
    for(i = 2; i <= n; i ++)
        result *= i;
    return result;
}

int main()
{
    int i;

    for(i = 1; i < 600; i ++)
        printf("factorial(%d) = %Lg\n", i, factorial(i));

    return 0;
}
