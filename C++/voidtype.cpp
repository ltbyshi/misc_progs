#include <iostream>
#include <vector>
using namespace std;

#include <anytype.h>

#define PRINTOP(op, var) op; cout << #op << " => "; \
    cout << #var << "(" << var.TypeStr() << ") = " << var << endl;
int main()
{
    vector<AnyType> a;
    a.push_back(AnyType(10));
    a.push_back(AnyType('a'));
    a.push_back(AnyType(100L));
    PRINTOP(AnyType b, b);
    PRINTOP(b = 1, b);
    PRINTOP(b = "a", b);
    PRINTOP(b = std::string("hello"), b);
    PRINTOP(b = 3.1415, b);
    PRINTOP(b = b + 1.0, b);
    PRINTOP(b = "cde", b);
    PRINTOP(AnyType c = "abc", c);
    PRINTOP(AnyType v = (b == c), v);
    PRINTOP(c = std::string("a"), c);
    
    return 0;
}
