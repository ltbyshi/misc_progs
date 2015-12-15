#include <iostream>
using namespace std;

#define PRINTFUNC2(f) {printf("%p\t"#f"\n", (void*)&f);}
#define PRINTFUNC(ret, cls, fn, args, pobj) {ret (cls::* pfn) args = &cls::fn; \
    printf("%p\t"#ret" "#cls"::"#fn#args"\n", (void*)(pobj->*pfn)); fflush(stdout); }

class Base
{
public:
    virtual ~Base() {}
    virtual void Print() { cout << "Base::Print" << endl; }
    virtual void Print(int a) { cout << "Base::Print(int)" << endl; }
};

class Derived: public Base
{
public:
    virtual ~Derived() {}
    virtual void Print() { cout << "Derived::Print" << endl; }
    virtual void Write() { cout << "Derived::Write" << endl; }
};

int main()
{
    Base base;
    Derived derived;

    Base* pBase = &base;
    Derived* pDerived = &derived;

    PRINTFUNC(void, Base, Print, (void), pBase);
    PRINTFUNC(void, Base, Print, (int),  pBase);
    PRINTFUNC(void, Derived, Print, (void), pDerived);
    PRINTFUNC(void, Base, Print, (int),  pDerived);
    PRINTFUNC(void, Derived, Write, (void), pDerived);
    //PRINTFUNC(Base::Print(int));
    //PRINTFUNC(Derived::Print);
    //PRINTFUNC(Derived::Write);

    pBase->Print();
    pDerived->Write();
   
    pDerived = (Derived*)&base;
    pDerived->Write();

    //pBase = (Base*)main;
    //pBase->Print();

    return 0;
}

