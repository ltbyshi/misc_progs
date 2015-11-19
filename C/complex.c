#include <complex.h>
#include <math.h>
#include <stdio.h>

int main()
{
    double pi;
    double complex z;
   
    pi = 4 * atan(1.0);
    z = cexp(I * pi);
    printf("sizeof z: %lu\n", sizeof(z));
    printf("%f + %fi\n", creal(z*z), cimag(z*z));
    return 0;
}
