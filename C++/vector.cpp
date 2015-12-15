#include <vector>
#include <iostream>
using namespace std;

int main()
{
    vector<int> v;
    size_t capacity = 0;
    for(size_t i = 0; i < 10000; i ++)
    {
        if(v.capacity() > capacity)
        {
            cout << "vector capacity changed from " << capacity << " to " << v.capacity()
                << " when i = " << i << endl;
            capacity = v.capacity();
        }
        v.push_back(i);
    }
    return 0;
}
