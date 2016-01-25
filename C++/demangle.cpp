#include <iostream>
#include <string>
using namespace std;
#include <demangle.h>

int main(int argc, char** argv)
{
    string line;
    while(getline(cin, line))
    {
        string name = demangle(line);
        if(name.size() > 0)
            cout << demangle(line) << endl;
        else
            cout << "Failed to demangle " << line << endl;
    }
    return 0;
}
