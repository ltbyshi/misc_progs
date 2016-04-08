#ifndef __IOUTILS_H__
#define __IOUTILS_H__
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>
#include <stdint.h>
#include "Exception.h"

// OS
bool FileExists(const std::string& filename);
size_t FileSize(const std::string& filename);
void MakeDir(const std::string& path, bool parent = false);
void RemoveFile(const std::string& path);
void RemoveDir(const std::string& path, bool recursive = false);
void CheckWrite(std::ostream& os, const char* s, std::streamsize n);

// IO
char* ReadFileAsString(const std::string& filename);
template <typename Type>
inline void WriteItem(std::ostream& os, const Type& item)
{
    os.write(reinterpret_cast<const char*>(&item), sizeof(item));
}

template <typename Type>
inline void ReadItem(std::istream& is, Type& item)
{
    is.read((char*)(&item), sizeof(item));
}

// vector
template <typename Type>
void LoadVector(std::vector<Type>& x, const std::string& filename)
{
    char typecode[4];
    uint32_t typesize;
    uint64_t size;
    
    std::ifstream fin(filename, std::ios::binary);
    if(!fin)
        throw IOError("cannot open the input file " + filename);
    fin.read(typecode, 4);
    fin.read((char*)&typesize, sizeof(typesize));
    fin.read((char*)&size, sizeof(size));
    if(strcmp(typecode, typeid(Type).name()))
    {
        fin.close();
        return;
    }
    x.resize(size);
    fin.read((char*)x.data(), size*typesize);
    fin.close();
}

template <typename Type>
void SaveVector(const std::vector<Type>& x, const std::string& filename)
{
    char typecode[4] = {};
    uint32_t typesize = sizeof(Type);
    uint64_t size = x.size();
    
    strncpy(typecode, typeid(Type).name(), 3);
    std::ofstream fout(filename, std::ios::binary);
    if(!fout)
        throw IOError("cannot open the input file " + filename);
    fout.write(typecode, 4);
    fout.write(reinterpret_cast<const char*>(&typesize), sizeof(typesize));
    fout.write(reinterpret_cast<const char*>(&size), sizeof(size));
    fout.write(reinterpret_cast<const char*>(x.data()), size*typesize);
    fout.close();
}

class MappedFile
{
public:
    MappedFile(const std::string& filename, bool readonly = true);
    ~MappedFile();
    void* GetData() { return _data; }
    size_t GetLength() { return _length; }
private:
    FILE* _fp;
    void* _data;
    size_t _length;
};

// checked iostreams
// BasicOutputStream
template <class charT, class Traits=std::char_traits<charT> >
class BasicOutputStream: public std::basic_ostream<charT, Traits>
{
    typedef std::basic_ostream<charT, Traits> _ostream_type;
public:
    BasicOutputStream(_ostream_type& os)
        : _ostream_type(os.rdbuf()) {}
    
};
typedef BasicOutputStream<char> OutputStream;

// BasicInputStream
template <class charT, class Traits=std::char_traits<charT> >
class BasicInputStream: public std::basic_istream<charT, Traits>
{
    typedef std::basic_istream<charT, Traits> _istream_type;
public:
    BasicInputStream(_istream_type& is)
        : _istream_type(is.rdbuf()) {}
    
};
typedef BasicInputStream<char> InputStream;

// BasicFileOutputStream
template <class charT, class Traits=std::char_traits<charT> >
class BasicFileOutputStream: public std::basic_ofstream<charT, Traits>
{
    typedef std::basic_ostream<charT, Traits> _ostream_type;
    typedef std::basic_ofstream<charT, Traits> _ofstream_type;
public:
    BasicFileOutputStream(const char* filename,
            ios_base::openmode mode = ios_base::out)
        : _ofstream_type(filename, mode) {
        if(!_ofstream_type::good())
            throw IOError(string("cannot open the output file ") + filename);
    }
};
typedef BasicFileOutputStream<char> FileOutputStream;

// BasicFileInputStream
template <class charT, class Traits=std::char_traits<charT> >
class BasicFileInputStream: public std::basic_ifstream<charT, Traits>
{
    typedef std::basic_istream<charT, Traits> _istream_type;
    typedef std::basic_ifstream<charT, Traits> _ifstream_type;
public:
    BasicFileInputStream(const char* filename,
            ios_base::openmode mode = ios_base::out)
        : _ifstream_type(filename, mode) {
        if(!_ifstream_type::good())
            throw IOError(string("cannot open the input file ") + filename);
    }
};
typedef BasicFileInputStream<char> FileInputStream;

char* ReadFileAsString(const std::string& filename);

#endif
