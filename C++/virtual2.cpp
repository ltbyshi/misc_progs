#include <iostream>
using namespace std;

#define DOECHO(stmt) \
    cout << #stmt << ": "; \
    stmt; \
    cout << endl;

class Base
{
public:
    virtual void PrintV() { cout << "Base::PrintV" << endl; }
    void Print() { cout << "Base::Print" << endl; }
};

class Derived: public Base
{
public:
    virtual void PrintV() { cout << "Derived::PrintV" << endl; }
    void Print() { cout << "Derived:Print" << endl; }
};

int main()
{
    Base base;
    Derived derived;

    Base& rbase = base;
    Base& rderived = derived;

    DOECHO(rbase.PrintV());
    DOECHO(rbase.Print());
    DOECHO(rderived.PrintV());
    DOECHO(rderived.Print());

    return 0;
}

