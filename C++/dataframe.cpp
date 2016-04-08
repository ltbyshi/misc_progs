#include <iostream>
using namespace std;
#include <DataFrame.h>

int main(int argc, char** argv)
{
    istream& fin = cin;
    DataFrame df;
    df.read_table(fin);
    cout << "nrow: " << df.nrow() << ", ncol: " << df.ncol() << endl;
    cout << "header: ";
    for(size_t i = 0; i < df.ncol(); i ++)
        cout << df.header()[i] << " ";
    cout << endl;
    for(size_t r = 0; r < 5; r ++)
    {
        for(size_t c = 0; c < df.ncol(); c ++)
            cout << df.col<string>(c)[r] << "\t";
        cout << endl;
    }
    return 0;
}