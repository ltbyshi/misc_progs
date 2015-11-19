#include <stdio.h>
#include <string.h>

struct BitField
{
    unsigned int var01 : 1;
    unsigned int var02 : 1;
    unsigned int var03 : 1;
    unsigned int var04 : 1;
    unsigned int var05 : 1;
    unsigned int var06 : 1;
    unsigned int var07 : 1;
    unsigned int var08 : 1;
    unsigned int var09 : 1;
    unsigned int var10 : 1;
    unsigned int var11 : 1;
    unsigned int var12 : 1;
    unsigned int var13 : 1;
    unsigned int var14 : 1;
    unsigned int var15 : 1;
    unsigned int var16 : 1;
    unsigned int var17 : 1;
    unsigned int var18 : 1;
    unsigned int var19 : 1;
    unsigned int var20 : 1;
    unsigned int var21 : 1;
    unsigned int var22 : 1;
    unsigned int var23 : 1;
    unsigned int var24 : 1;
    unsigned int var25 : 1;
    unsigned int var26 : 1;
    unsigned int var27 : 1;
    unsigned int var28 : 1;
    unsigned int var29 : 1;
    unsigned int var30 : 1;
    unsigned int var31 : 1;
    unsigned int var32 : 1;
    unsigned int var33 : 1;
    unsigned int var34 : 1;
    unsigned int var35 : 1;
    unsigned int var36 : 1;
    unsigned int var37 : 1;
    unsigned int var38 : 1;
    unsigned int var39 : 1;
    unsigned int var40 : 1;
    unsigned int var41 : 1;
    unsigned int var42 : 1;
    unsigned int var43 : 1;
    unsigned int var44 : 1;
    unsigned int var45 : 1;
    unsigned int var46 : 1;
    unsigned int var47 : 1;
    unsigned int var48 : 1;
    unsigned int var49 : 1;
    unsigned int var50 : 1;
    unsigned int var51 : 1;
    unsigned int var52 : 1;
    unsigned int var53 : 1;
    unsigned int var54 : 1;
    unsigned int var55 : 1;
    unsigned int var56 : 1;
    unsigned int var57 : 1;
    unsigned int var58 : 1;
    unsigned int var59 : 1;
    unsigned int var60 : 1;
    unsigned int var61 : 1;
    unsigned int var62 : 1;
    unsigned int var63 : 1;
    unsigned int var64 : 1;
};

int main()
{
    struct BitField bf;
    memset(&bf, 0, sizeof(bf));
    printf("%u\n", bf.var32);
    return 0;
}

