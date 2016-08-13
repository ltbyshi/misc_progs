#include "Thread.h"

void* ThreadFunc(void* args)
{
    int* retval = new int;
    *retval = ((Thread*)args)->Run();
    return (void*)retval;
}
