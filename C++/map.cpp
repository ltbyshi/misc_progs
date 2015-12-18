#include <map>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>

const size_t NumString = 500000;
const size_t Length = 32;

struct QueryMap
{
    map<string, int>& dict;
    const vector<string>& strings;

    QueryMap(map<string, int>& dict, const vector<string>& strings)
        : dict(dict), strings(strings) {}
    
    void operator()()
    {
        size_t times = NumString*100;
        cout << "Query map " << NumString*1000 << " times"  << endl;
        long sum = 0;
        for(size_t i = 0; i < times; i ++)
            sum += dict[strings[i % NumString]];
        cout << "Sum of values: " << sum << endl; 
    }
};

template <typename TargetFunc>
void TimeIt(const string& message, TargetFunc target)
{
    cout << "Time it: " << message << endl;
    struct timeval tvstart;
    gettimeofday(&tvstart, NULL);
    
    target();
    
    struct timeval tvend;
    gettimeofday(&tvend, NULL);
    double totalus = ((1e6*tvend.tv_sec) + tvend.tv_usec - (1e6*tvstart.tv_sec) - tvstart.tv_usec);
    cout << "Total time: " << totalus/1e3 << "ms" << endl;
}

int main()
{
    srand(time(0));
    cout << "Creating " << NumString << " strings of length " << Length << endl;
    vector<string> strings(NumString);
    for(size_t i = 0;i < NumString; i ++)
    {
        string s(Length, 0);
        for(size_t j = 0; j < Length; j ++)
            s[j] = rand();
        strings[i] = s;
    }
    cout << "Creating map" << endl;
    map<string, int> dict;
    for(size_t i = 0; i < NumString; i ++)
        dict[strings[i]] = i;

    TimeIt("QueryMap", QueryMap(dict, strings));
    return 0;
}
