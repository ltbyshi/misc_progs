#ifndef __ARGPARSE_H__
#define __ARGPARSE_H__

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <exception>
#include <ctype.h>
#include <stdint.h>
#include "anytype.h"

class ArgumentException: public std::exception
{
    ArgumentException(const std::string& msg) {
        this->msg = std::string("ArgumentParser error: ") + msg;
    }
    virtual const char* what() const throw()
    {
        return msg.c_str();
    }
    std::string msg;
};


class Argument
{
private:
    std::string _name;
    std::string _type;
    std::string _metavar;
    std::string _long_arg;
    std::string _short_arg;
    std::string _argstring;
    std::string _help;
    bool _given;
    bool _flag;
    bool _required;
    bool _positional;
    int _nargs_min;
    int _nargs_max;
    AnyType _default_value;
    AnyType _min_value;
    AnyType _max_value;
    std::vector<AnyType> _values;
    std::vector<AnyType> _choices;
    //ArgumentHandler* handler;
private:
    Argument(const Argument& a) {}
    Argument& operator=(const Argument& arg) {}
public:
    Argument(const std::string& name,
             const std::string& type = "str",
             const std::string& short_arg = "",
             const std::string& help = ""
            ): _name(name), _type(type), _given(false), _flag(false),
        _nargs_min(1), _nargs_max(1) {
        this->short_arg(short_arg);
    }
    
    ~Argument() {}
    
#define DECLARE_PROPERTY(name, type, var) \
    Argument& name(const type& value) { var = value; return *this; } \
    const type& name() const { return var; }
    DECLARE_PROPERTY(name, std::string, _name);
    DECLARE_PROPERTY(type, std::string, _type);
    DECLARE_PROPERTY(long_arg, std::string, _long_arg);
    DECLARE_PROPERTY(short_arg, std::string, _short_arg);
    DECLARE_PROPERTY(metavar, std::string, _metavar);
    DECLARE_PROPERTY(help, std::string, _help);
    DECLARE_PROPERTY(given, bool, _given);
    DECLARE_PROPERTY(required, bool, _required);
    DECLARE_PROPERTY(positional, bool, _positional);
    DECLARE_PROPERTY(nargs_min, int, _nargs_min);
    DECLARE_PROPERTY(nargs_max, int, _nargs_max);
    DECLARE_PROPERTY(default_value, AnyType, _default_value);
    DECLARE_PROPERTY(min_value, AnyType, _min_value);
    DECLARE_PROPERTY(max_value, AnyType, _max_value);
#undef DECLARE_PROPERTY
    
    Argument& name(const std::string& value) { _name = value; return *this; }
    const std::string& name() const { return _name; }
    
    Argument& type(const std::string& value) { _type = value; return *this; }
    const std::string& type() const { return _type; }
    
    Argument& long_arg(const std::string& value) {
        if(values.size() > 0)
            if(value[0] == '-')
                throw ArgumentException("long option should not start with -");
        _long_arg = value;
        return *this; 
    }
    const std::string& long_arg() const { return _long_arg; }
    
    Argument& short_arg(const std::string& value) {
        if(value.size() > 1)
            throw ArgumentException("short option is longer than one character");
        if(value.size() > 0)
            if(value[0] == '-')
                throw ArgumentException("short option should not start with -");
        _short_arg = value; 
        return *this; 
    }
    const std::string& short_arg() const { return _short_arg; }
    
    Argument& help(const std::string& value) { _help = value; return *this; }
    const std::string& help() const { return _help; }
    
    Argument& required(bool value) { _required = value; return *this; }
    bool required() const { return _required; }
    
    Argument& flag(bool value) const { _flag = value; return *this; }
    bool flag() const { return _flag; }
    
    Argument& given(bool value) const { _given = value; return *this; }
    bool given() const { return _given; }
    operator bool() const { return _given; }
    
    Argument& positional(bool value) const { _positional = value; return *this; }
    bool positional() const { return _positional; }
    
    Argument& nargs(int value) { _nargs_min = value; _nargs_max = value; return *this; }
    Argument& nargs(int minval, int maxval) { _nargs_min = minval; _nargs = maxval; return *this; }
    int nargs_min() const { return _nargs_min; }
    int nargs_max() const { return _nargs_max; }
    
    template <typename ValType>
    Argument& default_value(const ValType& value) { _default_value = new ValType(value); return *this; }
    template <typename ValType>
    const ValType& default_value() const { return (*(const ValType*)_default_value); }
    
    template <typename ValType>
    Argument& choice(const ValType& value) {
        _choices.push_back(new ValType(value));
        return *this;
    }
    
    template <typename ValType>
    Argument& choices(const std::vector<ValType>& values) { 
        _choices.resize(values.size());
        for(size_t i = 0; i < values.size(); i ++)
            _choices[i] = new ValType(values[i]);
        return *this;
    }
    template <typename ValType>
    std::vector<ValType> choices() {
        std::vector<ValType>& values(_choices.size());
        for(size_t i = 0; i < values.size(); i ++)
            values[i] = *((const ValType*)_choices[i]);
        return values;
    }
    
    template <typename ValType>
    Argument& max_value(const ValType& value) { _max_value = new ValType(value); return *this; }
    template <typename ValType>
    const ValType& max_value() const { return *_max_value; }
    bool has_max_value() const { return (_max_value != NULL); }
    
    template <typename ValType>
    Argument& min_value(const ValType& value) { _min_value = new ValType(value); return *this; }
    template <typename ValType>
    const ValType& min_value() const { return *_min_value; }
    bool has_min_value() const { return (_min_value != NULL); }
    
    template <typename ValType>
    Argument& range(const ValType& minval, const ValType& maxval) {
        _min_value = new ValType(minval);
        _max_value = new ValType(maxval);
        return *this;
    }
    
    template <typename ValType>
    std::vector<ValType> values() const {
        std::vector<ValType> result(_values.size());
        for(size_t i = 0; i < result.size(); i ++)
            result[i] = _values[i].Cast<ValType>();
        return result;
    }
    template <typename ValType>
    ValType value() const { 
        if(_values.size() <= 0)
            throw ArgumentException("no values given for " + _name);
        return _values[0].Cast<ValType>();
    }
    
    template <typename ValType>
    void check_values(const std::vector<ValType>& values) const {
        if((!_min_value.Empty()) && (!_max_value.Empty()))
        {
            for(size_t i = 0; i < values.size(); i ++)
            {
                if((values < *_min_value) || (values > *_max_value))
                    throw ArgumentException("value of " + _name + " out of range");
            }
        }
        if(_choices.size() > 0)
        {
            for(size_t i = 0; i < values.size(); i ++)
            {
                if(std::find(_choices.begin(), _choices.end(), values[i]) == _choices.end())
                    throw ArgumentException("value of " _name + " not in choices");
            }
        }
    }
};

template <typename ValType>
struct NullArgumentHandler
{
    bool operator()(const Argument<ValType>& arg) { return true; }
};

class ArgumentParser
{
public:
    enum {NARGS_INF = INT_MAX};
    ArgumentParser(const std::string& description = "")
        : _description(description) {}
        
    ~ArgumentParser() {
        for(std::map<std::string, Argument*>::iterator it = arguments.begin();
            it != options.end();
            ++ it)
            delete *it;
    }
    
    ArgumentParser& description(const std::string& value)
    Argument& add_positional(const std::string& name);
    Argument& add_argument(const std::string& name);
    bool check_option(const std::string& name,
                      const std::string& long_arg,
                      const std::string& short_arg) const;
    Argument& get_argument(const std::string& name);
    
    void parse_args(int argc, char** argv);
    Argument& operator[](const std::string& name) { return get_argument(name); }
    void help(const std::string& progname) const;
private:
    std::map<std::string, Argument*> _arguments;
    std::map<std::string, Argument*> _argmap;
    std::list<Argument*> _positionals;
    std::list<std::string> _names;
    std::string _description;
    bool _parsed;
    int _max_option_width;
    int _max_width;
};

ArgumentParser::ArgumentParser(const std::string& description)
    : _description(description), _parsed(false), 
    _max_option_width(30), _max_width(50) {}

ArgumentParser::~ArgumentParser()
{
    for(std::map<std::string, Argument*>::iterator it = options.begin();
        it != options.end();
        ++ it)
        delete *it;
}

Argument& ArgumentParser::add_argument(const std::string& name)
{
    if(_arguments.find(name) != _arguments.end())
        throw ArgumentException(std::string("argument ") + name + " is duplicated");
    _arguments[name] = new Argument(name);
    _arguments[name]->long_option(name);
    _names.push_back(name);
    return *(_arguments[name]);
}

Argument& ArgumentParser::get_argument(const std::string& name)
{
    std::map<std::string, Argument*>::iterator it = _arguments.end();
    if(it == _arguments.end())
        throw ArgumentException(std::string("argument" ) + name + " is not found");
    return *(*it);
}

void ArgumentParser::parse_args(int argc, char** argv)
{
    // find positionals and create argument map
    if(!_parsed)
    {
        int varnarg_positionals = 0;
        for(std::list<std::string>::iterator it = _names.begin(); it != _names.end(); ++it)
        {
            Argument* arg = _arguments[*it];
            if(arg->nargs_min() > arg->nargs_max())
                throw ArgumentException("nargs_min should not be larger than nargs_max");
            if(arg->positional)
            {
                _positionals.push_back(arg);
                if((arg->nargs_min() != args->nargs_max())
                    || (arg->nargs_max() == NARGS_INF)
                    || (arg->nargs_min() == NARGS_INF))
                    varnarg_positionals ++;
                if(varnarg_positionals > 1)
                    throw ArgumentException("number of variable narg positionals should be no more than one");
            }
            else
            {
                if((arg->nargs_max() == NARGS_INF) || (arg->nargs_min() == NARGS_INF))
                    throw ArgumentException("optional argument " + _name + "cannot be of variable narg");
                if((arg->long_arg.size() <= 0) && (arg->short_arg.size() <= 0))
                    throw ArgumentException("at least one of long option or short option should be non-empty");
                if(arg->short_arg.size() > 0)
                {
                    std::string short_arg = std::string("-") + arg->short_arg;
                    if(_argmap.find(short_arg) != _argmap.end())
                        throw ArgumentException("short option " + short_arg + " is duplicated");
                    _argmap[short_arg] = arg;
                }
                if(arg->long_arg.size() > 0)
                {
                    std::string long_arg = std::string("--")+ arg->short_arg;
                    if(_argmap.find(long_arg) != _argmap.end())
                        throw ArgumentException("long option " + short_arg + " is duplicated");
                    _argmap[long_arg] = arg;
                }
            }
        }
    }
    std::string progname = argv[0];
    for(int i = 1; i < argc; i ++)
    {
        std::string argstring(argv[i]);
        
    }
}

void ArgumentParser::help(const std::string& progname) const
{
    std::cout << "Usage: " << progname << "[options] ";
    for(std::list<Argument*>::iterator it = _positionals.begin(); it != _positionals.end(); ++ it)
        std::cout << " " << (*it)->name;
    std::cout << "\n" << std::endl;
    std::cout << _description << "\n" << std::endl;
    std::cout << "Optional arguments:" << std::endl;
    for(std::map<std::string, Argument*>::iterator it = _arguments.begin();
        it != _arguments.end(); ++it)
    {
        Argument* arg = *it;
        std::cout << "  ";
        if(arg->short_arg().size() > 0)
            std::cout << "-" << arg->short_arg();
        else
            std::cout << "  ";
        int pos = 4;
        if(arg->long_arg().size() > 0)
        {
            std::cout << ", --" << arg->long_arg();
            pos += 4 + arg->long_arg().size();
        }
        for(int i = pos; i < _max_option_width; i ++)
            std::cout << ' ';
        std::cout << arg->help();
        std::cout << std::endl;
    }
}

void ArgumentParser::check_option(const std::string& name,
                      const std::string& long_arg,
                      const std::string& short_arg)
{
    if((long_arg.size() <= 0) && (short_arg.size() <= 0))
        throw ArgumentException("at least one of long option or short option should be non-empty");
    if(long_arg.size() > 0)
    {
        if(argmap.find(long_arg) != argmap.end())
            throw ArgumentException(std::string("option ") + long_arg + " is duplicated");
        else if(long_arg[0] == '-')
            throw ArgumentException("long option should not start with -");
    }
    if(short_arg.size() > 0)
    {
        if(argmap.find(short_arg) != argmap.end()))
            throw ArgumentException("option " + short_arg + " is duplicated");
        else if(short_arg[0] == '-')
            throw ArgumentException(" short option should not start with -");  
    }
    if(name.size() <= 0)
        throw ArgumentException("option name is empty");
    else
    {
        if(name[0] == '-')
            throw ArgumentException("option name should not start with -");
        else if(options.find(name) != options.end())
            throw ArgumentException(std::string("option name ")s + name + " is duplicated");
    }
}

#endif
