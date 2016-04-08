#include <iostream>
using namespace std;
#include <stdlib.h>

extern "C" {
int AddInteger(int a, int b);
double AddDouble(double a, double b);
int AddInteger(int a, int b) { return a + b; }
double AddDouble(double a, double b) { return a + b; }
class Object
{
    int id;
public:
    Object(int id): id(id) {}
    int GetId() { return id; }
};
}
int main()
{
    cout << "int " << AddInteger((int)rand(), (int)rand()) << endl;
    cout << "double " << AddDouble(drand48(), drand48()) << endl;
    cout << "object " << Object(rand()).GetId() << endl;
    return 0;
}
