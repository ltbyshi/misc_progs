#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__
#include <exception>
#include <string>
#include <string.h>
#include <errno.h>

class BasicError: public std::exception {
public:
    BasicError(const std::string& what_arg) : msg(what_arg) {}
    BasicError(const std::string& what_arg, const std::string& name) {
        msg = name + ": " + what_arg; 
    }
    virtual const char* what() const noexcept { return msg.c_str(); }
private:
    std::string msg;
};

class IOError: public std::exception {
public:
    explicit IOError(const std::string& what_arg, bool show_errno = true) {
        msg = std::string("IOError: ") + what_arg;
        if(show_errno)
            msg += std::string(": ") + strerror(errno); 
    }
    virtual const char* what() const noexcept { return msg.c_str(); }
private:
    std::string msg;
};

class JSONError: public std::exception {
public:
    explicit JSONError(const std::string& what_arg) { 
        msg = std::string("JSONError: ") + what_arg; 
    }
    virtual const char* what() const noexcept { return msg.c_str(); }
private:
    std::string msg;
};

class OSError: public std::exception {
public:
    explicit OSError(const std::string& what_arg, bool show_errno = true) { 
        msg = std::string("OSError: ") + what_arg;
        if(show_errno)
            msg += std::string(": ") + strerror(errno); 
    }
    virtual const char* what() const noexcept { return msg.c_str(); }
private:
    std::string msg;
};

#endif
