#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include <Exception.h>

template <class charT, class Traits=char_traits<charT> >
class _OutputStream: public basic_ostream<charT, Traits>
{
public:
    _OutputStream(basic_ostream<charT, Traits>& os)
        : basic_ostream<charT, Traits>(os.rdbuf()) {}
    virtual basic_ostream<charT, Traits>& operator<<(int value){
        (*this) << "<int>";
        basic_ostream<charT, Traits>::operator<<(value);
        (*this) << "</int>";
        return *this;
    }
    
};
typedef _OutputStream<char> OutputStream;

template <class charT, class Traits=char_traits<charT> >
class BasicFileOutputStream: public basic_ofstream<charT, Traits>
{
    typedef basic_ostream<charT, Traits> _ostream_type;
public:
    BasicFileOutputStream(const char* filename,
            ios_base::openmode mode = ios_base::out)
        : basic_ofstream<charT, Traits>(filename, mode) {
            if(!basic_ofstream<charT, Traits>::good())
                throw IOError(string("cannot open the file ") + filename);
        }
    /*
    virtual BasicFileOutputStream& operator<<(int value) {
        (*this) << "<int>";
        _ostream_type::operator<<(value);
        (*this) << "</int>";
        return *this;
    }
    virtual BasicFileOutputStream& flush() {
        cerr << "flushed" << endl;
        basic_ofstream<charT, Traits>::flush();
        return *this;
    }
    
    virtual BasicFileOutputStream& operator<<(_ostream_type& (*func)(_ostream_type&) ){
        _ostream_type::operator<<(func);
        return *this;
    }
    */
};

typedef BasicFileOutputStream<char> FileOutputStream;

int main()
{
    FileOutputStream out("/dev/stdout");
    out.flush();
    out << endl;
    return 0;
}
