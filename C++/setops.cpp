#include <iostream>
#include <algorithm>
#include <iterator>
using namespace std;

template <typename T>
struct StaticPointer: public std::iterator<std::output_iterator_tag, T>
{
    T value;
    size_t pos;
    StaticPointer(const T& value): value(value), pos(0) {}
    T& operator*() { return value; }
    StaticPointer& operator--() { pos--; return *this; }
    StaticPointer& operator--(int) { pos--; return *this; }
    StaticPointer& operator-=(size_t n) { pos -= n; return *this; }
    StaticPointer& operator++() { pos++; return *this; }
    StaticPointer& operator++(int) { pos++; return *this; }
    StaticPointer& operator+=(size_t n) { pos += n; return *this; }
};

template <class InputIterator, class OutputIterator>
void PrintArray(InputIterator first, OutputIterator last)
{
    cout << "[ ";
    while(first != last)
    {
        cout << *first << " ";
        ++ first;
    }
    cout << "]";
}

int main()
{
    int a[] = {1, 3, 5, 6, 10};
    size_t a_size = 5;
    int b[] = {3, 5, 7, 10, 15};
    size_t b_size = 5;
    StaticPointer<int> it(0);
    StaticPointer<int> size(0);

    cout << "a[" << a_size << "]: "; PrintArray(a, a + a_size); cout << endl;
    cout << "b[" << b_size << "]: "; PrintArray(b, b + b_size); cout << endl;
    
    it.pos = 0;
    size = set_union(a, a + a_size, b, b + b_size, it);
    cout << "set_union: " << size.pos << endl;

    it.pos = 0;
    size = set_intersection(a, a + a_size, b, b + b_size, it);
    cout << "set_intersection: " << size.pos << endl;

    it.pos = 0;
    size = set_difference(a, a + a_size, b, b + b_size, it);
    cout << "set_difference: " << size.pos << endl;

    return 0;
}

