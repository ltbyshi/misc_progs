#include <iostream>
#include <string>
#include <map>
#include <exception>
#include <ctype.h>
using namespace std;

class ArgumentException: public std::exception
{
    ArgumentException(const std::string& msg): msg(msg) {}
    virtual const char* what() const throw()
    {
        return std::string("ArgumentParser error: ") + msg.c_str();
    }
    std::string msg;
};



template <typename ValType>
class Argument
{
private:
    std::string _name;
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
    void* _default_value;
    void* _min_value;
    void* _max_value;
    vector<std::string> _values;
    vector<void*> _choices;
    //ArgumentHandler* handler;
private:
    Argument(const Argument& a) {}
    Argument& operator=(const Argument& arg) {}
public:
    Argument(const std::string& name, 
             const std::string& short_arg = "",
             const std::string& help = ""
            ): _name(name), _given(false), _flag(false),
        _nargs_min(1), _nargs_max(1), 
        _default_value(NULL), _min_value(NULL), _max_value(NULL) {
        this->short_arg(short_arg);
    }
    
    ~Argument() {
        if(_default_value)
            delete _default_value;
        if(_min_value)
            delete _min_value;
        if(_max_value)
            delete _max_value;
        for(size_t i = 0; i < _choices.size(); i ++)
            delete _choices[i];
    }
    
    Argument& name(const std::string& value) { _name = name; return *this; }
    const std::string name() const { return _name; }
    
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
    
    Argument& positional(bool value) const { _positional = value; return *this; }
    bool positional() const { return _positional; }
    
    Argument& nargs(int value) { _nargs_min = value; _nargs_max = value; return *this; }
    Argument& nargs(int minval, int maxval) { _nargs_min = minval; _nargs = maxval; return *this; }
    int nargs_min() const { return _args_min; }
    int nargs_max() const { return _args_max; }
    
    template <typename ValType>
    Argument& default_value(const ValType& value) { _default_value = new ValType(value); return *this; }
    template <typename ValType>
    const ValType& default_value() const { return (*(const ValType*)_default_value); }
    
    template <typename ValType>
    Argument& choices(const vector<ValType>& values) { 
        _choices.resize(values.size());
        for(size_t i = 0; i < values.size(); i ++)
            _choices[i] = new ValType(values[i]);
        return *this;
    }
    template <typename ValType>
    vector<ValType> choices() {
        vector<ValType>& values(_choices.size());
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
    vector<ValType> values() const {
        vector<ValType> result(_values.size());
        for(size_t i = 0; i < result.size(); i ++)
        {
            std::istringstream is(_values[i]);
            is >> result[i];
        }
        return result;
    }
    template <typename ValType>
    ValType value() const { 
        if(_values.size() <= 0)
            throw ArgumentException("no values given for " + _name);
        ValType result;
        std::istringstream is(_values[0]);
        is >> result;
        return result;
    }
    
    template <typename ValType>
    void check_values() const {
        if((_min_value != NULL) && (_max_value != NULL))
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
        for(std::list<std::string>::iterator it = _names.begin(); it != _names.end(); ++it)
        {
            Argument* arg = _arguments[*it];
            if(arg->positional)
                _positionals.push_back(arg);
            else
            {
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
                if(arg->nargs_min > arg->nargs_max)
                    throw ArgumentException("nargs_min should not be larger than nargs_max");
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

template <typename ValType>
void ArgumentParser::add_argument<ValType>(const std::string& name,
                      const std::string& long_arg,
                      const std::string& short_arg,
                      const std::string& help,
                      const ValType& defval,
                      int nargs = 1,
                      bool flag,
                      bool required = false)
{
    check_option(name, long_arg, short_arg);
    
    Argument<ValType>* arg = new Argument<ValType>;
    arg->name = name;
    arg->short_arg = std::string("-") + short_arg;
    arg->long_arg = std::string("--") + long_arg;
    arg->help = help;
    arg->given = false;
    arg->flag = flag;
    arg->required = required;
    arg_defvalue = defval;
    options[arg->name] = arg;
    if(arg->long_arg.size() > 0)
        argmap[arg->long_arg] = arg;
    if(arg->short_arg.size() > 0)
        argmap[arg->short_arg] = arg;
}

