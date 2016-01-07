#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
#include <stdlib.h>

template <typename Container>
struct ArgLess
{
    const Container& data;
    ArgLess(const Container& data): data(data) {}
    bool operator()(size_t i, size_t j) {
        return (data[i] < data[j]);
    }
};

int main()
{
    const size_t n = 100;
    vector<int> data(n);
    for(size_t i = 0; i < n; i ++)
        data[i] = rand()%1000;
    vector<int> index(n);
    for(size_t i = 0; i < n; i ++)
        index[i] = i;
    cout << "Before sorting:" << endl;
    for(size_t i = 0; i < 10; i ++)
        cout << data[i] << " ";
    cout << endl;

    std::sort(index.begin(), index.end(), ArgLess<vector<int> >(data));
    
    cout << "After sorting:" << endl;
    for(size_t i = 0; i < 10; i ++)
        cout << data[index[i]] << " ";
    cout << endl;

    return 0;
}
