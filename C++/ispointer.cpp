#include <iostream>
#include <vector>
using namespace std;
#include <demangle.h>

class NullType {};

template <typename T>
class TypeTraits
{
private:
    template <typename U> struct PointerTraits
    {
        enum { result = false };
        typedef NullType target;
    };
    template <typename U> struct PointerTraits<U*>
    {
        enum { result = true };
        typedef U target;
    };
public:
    enum { isPointer = PointerTraits<T>::result };
    typedef typename PointerTraits<T>::target target;
};

#define IsPointerType(x) cout << demangle(typeid(x).name()) << " is " \
    <<  (TypeTraits<x>::isPointer? "" : "not") << " a pointer." << endl;

template <typename T>
void IsPointer(const T& x)
{
    cout << demangle(typeid(T).name()) << " is "
        << (TypeTraits<T>::isPointer? "" : "not") << " a pointer." << endl;
}

int main()
{
    IsPointerType(vector<int>::iterator);
    IsPointerType(FILE);
    IsPointerType(vector<int>);
    IsPointer(stdout);
    IsPointer(cout);
    IsPointer(main);
    IsPointer(IsPointer<int>);

    return 0;
}
