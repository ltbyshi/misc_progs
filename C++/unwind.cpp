#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

struct Object {
    string name;
    Object(const string& name) : name(name) { 
        cout << "Object " << name << " is constructed" << endl;
    }
    ~Object() {
        cout << "Object " << name << " is destructed" << endl;
    }
};

int main()
{

    //try {
        Object a("A"), b("B");
        cout << "Throw exception" << endl;
        throw runtime_error("Exception thrown");
    //} 
#if 0
    catch(const runtime_error& e) {
        cout << "Catch exception" << endl;
        exit(1);
    }
#endif
    return 0;
}
