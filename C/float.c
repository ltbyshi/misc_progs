#include <stdio.h>
#include <float.h>

#define PrintConstI(x) printf("%s: %d\n", #x, x)

int main()
{
    PrintConstI(FLT_MAX_10_EXP);
    PrintConstI(DBL_MAX_10_EXP);
    PrintConstI(LDBL_MAX_10_EXP);
    
    return 0;
}
