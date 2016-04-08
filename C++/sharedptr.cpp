#include <iostream>
#include <memory>
using namespace std;

class Object {
    int id;
public:
    Object(int id) : id(id) { 
        cout << "Created Object " << id << endl; 
    }

    ~Object() {
        cout << "Deleted Object " << id << endl;
    }
    
    Object* Create(int id) {
        return new Object(id);
    }
};

int main()
{
    shared_ptr<Object> p1 = make_shared<Object>(1);

    cout << "Exiting main" << endl;
    return 0;   
}
