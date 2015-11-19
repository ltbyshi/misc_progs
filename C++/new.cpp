#include <cstdio>
#include <cstdlib>
#include <new>
#include <iostream>
#include <malloc.h>
using namespace std;

class MemTracker
{
public:
    ~MemTracker()
    {
        if(mem != 0)
            cout << "Detected memory leaks: " << mem << " bytes." << endl;
    }
    static std::size_t mem;
};

std::size_t MemTracker::mem = 0;

void* operator new(std::size_t size)
{
    void* ptr = std::malloc(size);
    std::size_t sz = malloc_usable_size(ptr);
    cout << "allocated " << sz << " bytes" << endl;
    MemTracker::mem += sz;
    return ptr;
}

void operator delete(void* ptr)
{
    std::size_t size = malloc_usable_size(ptr);
    cout << "deleted " << size << " bytes" << endl;
    MemTracker::mem -= malloc_usable_size(ptr);
    std::free(ptr);
}

MemTracker mtracker;

int main()
{
    int* p =  new int[16];
    delete p;
    delete p;
    return 0;
}
