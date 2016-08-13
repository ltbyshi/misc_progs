#include <iostream>
#include <stdio.h>
#include <stdlib.h>
// Linux
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "IOUtils.h"
#include "Exception.h"

// OS
bool FileExists(const std::string& filename)
{
    FILE* fp = fopen(filename.c_str(), "r");
    if(!fp)
        return false;
    else
    {
        fclose(fp);
        return true;
    }
}

size_t FileSize(const std::string& filename)
{
    struct stat st;
    if(stat(filename.c_str(), &st) == 0)
        return st.st_size;
    else
        return 0;
}

void MakeDir(const std::string& path, bool parent)
{
    if(parent)
    {
        if(system((std::string("mkdir -p ") + path).c_str()) != 0)
            throw OSError("cannot create directory " + path, false);
    }
    else
    {
        if(mkdir(path.c_str(), 0777) != 0)
            throw OSError("cannot create directory " + path);
    }
}

void RemoveFile(const std::string& path)
{
    if(unlink(path.c_str()) != 0)
        throw OSError("cannot remove file " + path);
}

void RemoveDir(const std::string& path, bool recursive)
{
    if(recursive)
    {
        if(system((std::string("rm -r ") + path).c_str()) != 0)
            throw OSError("cannot remove directory " + path, false);
    }
    else
    {
        if(rmdir(path.c_str()) != 0)
            throw OSError("cannot remove directory " + path);
    }
}

char* ReadFileAsString(const std::string& filename)
{
    FILE* fp = fopen(filename.c_str(), "rb");
    if(!fp)
        throw IOError("cannot open file " + filename);
    if(fseek(fp, 0, SEEK_END) != 0)
        throw IOError("cannot seek in file " + filename);
    unsigned long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* s = new char[fsize + 1];
    if(fread(s, 1, fsize, fp) < fsize)
    {
        delete s;
        throw IOError("read less than file size " + filename);
    }
    fclose(fp);
    return s;
}

void CheckWrite(std::ostream& os, const char* s, std::streamsize n)
{
    os.write(s, n);
    if(!os)
    {
        if(os.eof())
            IOError("end of file reached");
        if(os.bad())
            IOError("logical error on I/O operation");
        if(os.bad())
            IOError("writing error on I/O operation");
    }
}

MappedFile::MappedFile(const std::string& filename, bool readonly)
{
    if(readonly)
        _fp = fopen(filename.c_str(), "rb");
    else
        _fp = fopen(filename.c_str(), "rb+");
    if(!_fp)
        IOError("cannot open the input file " + filename);
    int fd = fileno(_fp);
    struct stat filestat;
    fstat(fd, &filestat);
    _length = filestat.st_size;
    _data = mmap(NULL, filestat.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if(_data == MAP_FAILED)
        OSError("mmap failed");
}

MappedFile::~MappedFile()
{
    munmap(_data, _length);
    fclose(_fp);
}

char* ReadFileAsString(const std::string& filename)
{
    FILE* fp = fopen(filename.c_str(), "rb");
    if(!fp)
        throw IOError("cannot open file " + filename);
    if(fseek(fp, 0, SEEK_END) != 0)
        throw IOError("cannot seek in file " + filename);
    unsigned long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* s = new char[fsize + 1];
    if(fread(s, 1, fsize, fp) < fsize)
    {
        delete s;
        throw IOError("read less than file size " + filename);
    }
    fclose(fp);
    return s;
}