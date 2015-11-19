#include <stdio.h>

int main()
{
L1:
    main();
    goto L1;
    return 0;
}
