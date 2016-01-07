#include <stdexcept>
#include <iostream>
#include <sstream>
using namespace std;

void Recurse(int n)
{
    try {
        if(n < 1000)
            Recurse(n + 1);
        else
        {
            ostringstream os;
            os << "recursion limit reached";
            throw invalid_argument(os.str());
        }
    } catch(const invalid_argument& e) {
        cout << e.what() << " (" << n << ")" << endl;
        throw e;
    } 
}

int main()
{
    Recurse(0);
    return 0;
}
