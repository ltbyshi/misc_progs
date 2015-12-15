#include <stdio.h>

int main()
{
        // throw main;
    try{
        throw (void*)NULL;
    } catch(...)
    {
        *((int*)NULL) = 0;
    }
    return 0;
}
