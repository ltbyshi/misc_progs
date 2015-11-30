#include <iostream>
using namespace std;

class Private
{
public:
    static Private* Create(int data) { return new Private(data); }
    void Destroy() { delete this; }
    int GetData() const { return _data; }
private:
    Private(int data): _data(data) {}
    ~Private() {}
    int _data;
};

int main()
{
    Private* pri = Private::Create(10);
    cout << pri->GetData() << endl;
    pri->Destroy();
    return 0;
}
