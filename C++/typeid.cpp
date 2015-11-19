#include <iostream>
#include <typeinfo>
#include <complex>
using namespace std;
#include <stdint.h>
#include <cxxabi.h>
#include <stdlib.h>

inline std::string demangle(const char* name)
{
    int status;
    char* s = abi::__cxa_demangle(name, 0, 0, &status);
    std::string fullname(s);
    free(s);
    return fullname;
}

#define TYPEID(type) cout << #type << "\t" <<  typeid(type).name() \
    << "\t" << demangle(typeid(type).name()) << "\t" << sizeof(type) << endl;

namespace NsA{
namespace NsB{
class ClassA
{
    int i;
};
}
}

int main()
{
    TYPEID(char);
    TYPEID(unsigned char);
    TYPEID(short);
    TYPEID(unsigned short);
    TYPEID(int);
    TYPEID(unsigned int);
    TYPEID(long);
    TYPEID(unsigned long int);
    TYPEID(long long int);
    TYPEID(unsigned long long int);
    TYPEID(float);
    TYPEID(double);
    TYPEID(long double);
    TYPEID(std::complex<double>);
    TYPEID(std::complex<float>);
    TYPEID(std::complex<long double>);
    TYPEID(void*);
    TYPEID(NsA::NsB::ClassA);

    return 0;
}
