#ifndef __ARGPARSE_H__
#define __ARGPARSE_H__

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <exception>
#include <ctype.h>
#include <limits.h>

#include <anytype.h>

class ArgumentException: public std::exception
{
public:
    ArgumentException(const std::string& msg) {
        this->msg = std::string("ArgumentParser error: ") + msg;
    }
    virtual const char* what() const throw()
    {
        return msg.c_str();
    }
    std::string msg;
};

class ArgumentParser;
class Argument
{
    friend class ArgumentParser;
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
    size_t _nargs_min;
    size_t _nargs_max;
    AnyType _default_value;
    AnyType _min_value;
    AnyType _max_value;
    std::vector<AnyType> _values;
    std::vector<AnyType> _choices;
    //ArgumentHandler* handler;
private:
    Argument(const Argument& a) {}
    Argument& operator=(const Argument& arg) { return *this; }
public:
    Argument(const std::string& name,
             const std::string& type = "str"
            ): _name(name), _type(type),
        _given(false), _flag(false),
        _nargs_min(1), _nargs_max(1) {
    }
    
    ~Argument() {}
    
    operator bool() const { return _given; }
    
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
    DECLARE_PROPERTY(flag, bool, _flag);
    DECLARE_PROPERTY(required, bool, _required);
    DECLARE_PROPERTY(positional, bool, _positional);
    DECLARE_PROPERTY(nargs_min, size_t, _nargs_min);
    DECLARE_PROPERTY(nargs_max, size_t, _nargs_max);
#undef DECLARE_PROPERTY
    // default_value
    template <typename ValType>
    Argument& default_value(const ValType& value) {
        _default_value = AnyType::FromType(_type).Store(value);
        return *this;
    }
    const AnyType& default_value() const {
        return _default_value;
    }
    // min_value
    template <typename ValType>
    Argument& min_value(const ValType& value) {
        _min_value = AnyType::FromType(_type).Store(value);
        return *this;
    }
    const AnyType& min_value() const {
        return _min_value;
    }
    // max_value
    template <typename ValType>
    Argument& max_value(const ValType& value) {
        _max_value = AnyType::FromType(_type).Store(value);
        return *this;
    }
    template <typename ValType>
    const AnyType& max_value() const {
        return _max_value;
    }
    // range
    template <typename ValType>
    Argument& range(const ValType& minval, const ValType& maxval) {
        _min_value = AnyType::FromType(_type).Store(minval);
        _max_value = AnyType::FromType(_type).Store(maxval);
        return *this;
    }
#if 0
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
#endif
    Argument& nargs(size_t value) {
        _nargs_min = value; 
        _nargs_max = value;
        return *this;
    }
    Argument& nargs(size_t minval, size_t maxval) {
        _nargs_min = minval; 
        _nargs_max = maxval; 
        return *this; 
    }
#if 0
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
#endif
    template <typename ValType>
    Argument& add_choice(const ValType& value) {
        AnyType a = AnyType::FromType(_type);
        a.Store(value);
        std::cout << "add_choice(" << _type << "): " << a << std::endl;
        _choices.push_back(a);
        return *this;
    }
    
    Argument& add_choice(const char* value) {
        AnyType a = AnyType::FromType(_type);
        a.Store(value);
        _choices.push_back(a);
        std::cout << "add_choice(" << _type << "): " << a << std::endl;
        return* this;
    }
    
    template <typename ValType>
    Argument& choices(const std::vector<ValType>& values) { 
        _choices.resize(values.size());
        for(size_t i = 0; i < values.size(); i ++)
            _choices[i] = AnyType::FromType(_type).Store(values[i]);
        return *this;
    }
    
    size_t nvalues() const { return _values.size(); }
    template <typename ValType>
    std::vector<ValType> values() const {
        std::vector<ValType> result(_values.size());
        for(size_t i = 0; i < result.size(); i ++)
            result[i] = _values[i].Cast<ValType>();
        return result;
    }
    template <typename ValType>
    ValType value() const { 
        return value<ValType>(0);
    }
    template <typename ValType>
    ValType value(size_t i) const {
        if(i >= _values.size())
            throw ArgumentException("value index out of bound in option " + _name);
        return _values[i].Cast<ValType>();
    }
    
    Argument& set_value(const AnyType& value) {
        if(_values.size() == 0)
            _values.resize(1);
        _values[0] = value;
        return *this;
    }
    
    Argument& set_value(const std::string& s) {
        set_value(AnyType::Parse(s, _type));
        return *this;
    }
    
    Argument& add_value(const AnyType& value) {
        _values.push_back(value);
        return *this;
    }
    
    Argument& add_value(const std::string& s) {
        add_value(AnyType::Parse(s, _type));
        return *this;
    }
    
    template <typename ValType>
    void check_values(const std::vector<ValType>& values) const {
        
        if((!_min_value.Empty()) && (!_max_value.Empty()))
        {
            for(size_t i = 0; i < values.size(); i ++)
            {
                if((values[i] < _min_value) || (values[i] > _max_value))
                    throw ArgumentException(std::string("value of ") + _name + " out of range");
            }
        }
        if(_choices.size() > 0)
        {
            for(size_t i = 0; i < values.size(); i ++)
            {
                if(std::find(_choices.begin(), _choices.end(), values[i]) == _choices.end())
                    throw ArgumentException(std::string("value of ") + _name + " not in choices");
            }
        }
    }
    
    void Validate() const {
        if((_long_arg.size() > 0) && (_long_arg[0] == '-'))
            throw ArgumentException(std::string("long option '") + _name + "'should not start with -");
        if((_short_arg.size() > 0) && (_short_arg[0] == '-'))
            throw ArgumentException(std::string("short option '") + _name + "' should not start with -");
        if(_short_arg.size() > 1)
            throw ArgumentException(std::string("short option '") + _name + "' should not be longer than one character");
        if(_required && (!_default_value.Empty()))
            throw ArgumentException(std::string("required option '") + _name + "' should not be given a default value");
        if(_nargs_min > _nargs_max)
            throw ArgumentException(std::string("nargs_min should not be greater than nargs_max in option ") + _name);
        if(_nargs_min < 0)
            throw ArgumentException(std::string("nargs_min should be non-negative in option ") + _name);
        if(_positional && (_nargs_min <= 0))
            throw ArgumentException(std::string("nargs_min should be at least 1 for positional ") + _name);
        if(!_positional && (_nargs_max > 1))
            throw ArgumentException(std::string("option '") + _name + "' should not have more than one value");
        // check type
        AnyType proto = AnyType::FromType(_type);
        if(!_min_value.Empty() && (_min_value.Type() != proto.Type()))
            throw ArgumentException(std::string("min_value is incompatible with the given type in option") + _name);
        if(!_max_value.Empty() && (_max_value.Type() != proto.Type()))
            throw ArgumentException(std::string("max_value is incompatible with the given type in option") + _name);
    }
    
    void ValidateValues() const {
        // check required
        if(_required && (_values.size() == 0))
            throw ArgumentException(std::string("option '") + _name + "' requires arguments");
        // check nargs
        if(_values.size() < _nargs_min)
            throw ArgumentException(std::string("insufficient arguments for ") + _name);
        if(_values.size() > _nargs_max)
            throw ArgumentException(std::string("extra arguments for ") + _name);
        // check range
        for(size_t i = 0; i < _values.size(); i ++)
        {
            if((!_min_value.Empty() && (_values[i] < _min_value)) ||
               (!_max_value.Empty() && (_values[i] > _max_value)) )
                throw ArgumentException(std::string("value of ") + _name + " out of range");
        }
        // check choice
        if(_choices.size() > 0)
        {
            for(size_t i = 0; i < _values.size(); i ++)
            {
                bool found = false;
                for(size_t j = 0; j < _choices.size(); j ++)
                {
                    if(_choices[j] == _values[i])
                    {
                        found = true;
                        break;
                    }
                }
                if(!found)
                //if(std::find(_choices.begin(), _choices.end(), AnyType(_values[i])) == _choices.end())
                    throw ArgumentException(std::string("value of ") + _name + " not in choices");
            }
        }
        
    }
};

template <typename ValType>
struct NullArgumentHandler
{
    bool operator()(const Argument& arg) { return true; }
};

class ArgumentParser
{
    typedef std::map<std::string, Argument*>::iterator mapiter;
    typedef std::map<std::string, Argument*>::const_iterator mapiter_c;
public:
    enum {NARGS_INF = INT_MAX};
    ArgumentParser(const std::string& description = "");
    ~ArgumentParser();
    
    inline ArgumentParser& description(const std::string& value) { _description = value; return *this; }
    inline ArgumentParser& progname(const std::string& value) { _progname = value; return *this; }
    inline ArgumentParser& max_width(int value) { _max_width = value; return *this; }
    inline ArgumentParser& max_option_width(int value) { _max_option_width = value; return *this; }

    Argument& add_positional(const std::string& name,
                             const std::string& type = "str");
    Argument& add_argument(const std::string& name,
                           const std::string& type = "str");
    Argument& add_flag(const std::string& name);
    Argument& get_argument(const std::string& name);
    
    void parse_args(int argc, char** argv);
    Argument& operator[](const std::string& name) { return get_argument(name); }
    void help() const;
    void summary() const;
    
    void validate_args();
    void validate_values();
private:
    std::map<std::string, Argument*> _arguments;
    std::map<std::string, Argument*> _argmap;
    std::list<Argument*> _positionals;
    std::list<std::string> _names;
    std::string _description;
    bool _parsed;
    int _max_option_width;
    int _max_width;
    std::vector<bool> valid_long;
    std::vector<bool> valid_short;
    std::string _progname;
};

ArgumentParser::ArgumentParser(const std::string& description)
    : _description(description), _parsed(false), 
    _max_option_width(30), _max_width(50) {
         // initialize valid characters
        valid_long.resize(128);
        for(char c = 'a'; c < 'z'; c ++)
            valid_long[int(c)] = true;
        for(char c = 'A'; c < 'Z'; c ++)
            valid_long[int(c)] = true;
        for(char c = '0'; c < '9'; c ++)
            valid_long[int(c)] = true;
        valid_long[int('_')] = true;
        valid_long[int('.')] = true;
        
        add_flag("help").short_arg("h").help("display help");
    }

ArgumentParser::~ArgumentParser()
{
    for(std::map<std::string, Argument*>::iterator it = _arguments.begin();
        it != _arguments.end();
        ++ it)
        delete it->second;
}

Argument& ArgumentParser::add_positional(const std::string& name, const std::string& type)
{
    if(_arguments.find(name) != _arguments.end())
        throw ArgumentException(std::string("argument ") + name + " is duplicated");
    Argument* arg = new Argument(name, type);
    arg->positional(true);
    _arguments[name] = arg;
    _names.push_back(name);
    return *arg;
}

Argument& ArgumentParser::add_argument(const std::string& name, const std::string& type)
{
    if(_arguments.find(name) != _arguments.end())
        throw ArgumentException(std::string("argument ") + name + " is duplicated");
    Argument* arg = new Argument(name, type);
    arg->long_arg(name);
    _arguments[name] = arg;
    _names.push_back(name);
    return *arg;
}

Argument& ArgumentParser::add_flag(const std::string& name)
{
    if(_arguments.find(name) != _arguments.end())
        throw ArgumentException(std::string("argument ") + name + " is duplicated");
    Argument* arg = new Argument(name, "bool");
    arg->long_arg(name);
    arg->set_value(AnyType(false));
    arg->nargs(1);
    _arguments[name] = arg;
    _names.push_back(name);
    return *arg;
}

Argument& ArgumentParser::get_argument(const std::string& name)
{
    std::map<std::string, Argument*>::iterator it = _arguments.find(name);
    if(it == _arguments.end())
        throw ArgumentException(std::string("argument '" ) + name + "' is not found");
    return *(it->second);
}

void ArgumentParser::validate_args()
{
    int varnarg_positionals = 0;
    for(std::list<std::string>::iterator it = _names.begin(); it != _names.end(); ++it)
    {
        Argument* arg = _arguments[*it];
        arg->Validate();
        if(arg->positional())
        {
            // positionals are required
            arg->required(true);
            _positionals.push_back(arg);
            if((arg->nargs_min() != arg->nargs_max())
                || (arg->nargs_max() == NARGS_INF)
                || (arg->nargs_min() == NARGS_INF))
                varnarg_positionals ++;
            if(varnarg_positionals > 1)
                throw ArgumentException("number of variable narg positionals should be no more than one");
        }
        else
        {
            if((arg->nargs_max() == NARGS_INF) || (arg->nargs_min() == NARGS_INF))
                throw ArgumentException(std::string("optional argument ") + arg->name() + "cannot be of variable narg");
            if((arg->long_arg().size() <= 0) && (arg->short_arg().size() <= 0))
                throw ArgumentException("at least one of long option or short option should be non-empty");
            if(arg->short_arg().size() > 0)
            {
                std::string short_arg = std::string("-") + arg->short_arg();
                if(_argmap.find(short_arg) != _argmap.end())
                    throw ArgumentException(std::string("short option ") + arg->short_arg() + " is duplicated");
                _argmap[short_arg] = arg;
            }
            if(arg->long_arg().size() > 0)
            {
                std::string long_arg = std::string("--")+ arg->long_arg();
                if(_argmap.find(long_arg) != _argmap.end())
                    throw ArgumentException(std::string("long option ") + arg->long_arg() + " is duplicated");
                _argmap[long_arg] = arg;
            }
        }
    }
}

void ArgumentParser::validate_values()
{
    for(mapiter it = _arguments.begin(); it != _arguments.end(); ++ it)
    {
        Argument* arg = it->second;
        if(arg->given() && (arg->nvalues() == 0))
            throw ArgumentException(std::string("option '") + arg->name() + "' requires arguments");
        // load default values
        if((arg->nvalues() == 0) && (!arg->default_value().Empty()))
            arg->set_value(arg->default_value());
        // validate
        if(arg->required())
            arg->ValidateValues();
    }
}

void ArgumentParser::parse_args(int argc, char** argv)
{
    // find positionals and create argument map
    validate_args();
    if(_progname.size() <= 0)
        _progname = argv[0];
    int argi = 1;
    // parse options
    Argument* arg = NULL;
    std::list<Argument*>::iterator pos_arg = _positionals.begin();
    while(argi < argc)
    {
        std::string s(argv[argi]);
        // parse option names
        if((s.size() > 0) && (s[0] == '-'))
        {
            std::string opt = s;
            // arg=value assignment
            size_t pos = s.find('=');
            if(pos != std::string::npos)
                opt = s.substr(0, pos);
            mapiter it = _argmap.find(opt);
            if(it == _argmap.end())
                throw ArgumentException(std::string("option '") + opt + "' is not found");
            arg = it->second;
            if(arg->given())
                throw ArgumentException(std::string("option '") + opt + "' is duplicated");
            if(arg->flag())
                arg->set_value(AnyType(true));
            arg->given(true);
            // arg=value assignment
            if(pos != std::string::npos)
            {
                arg->set_value(s.substr(pos + 1, s.size() - pos - 1));
                // end of this option
                arg = NULL;
            }
        }
        else
        {
            // parse option values
            if(arg != NULL)
            {
                if(arg->nvalues() < arg->nargs_max())
                {
                    arg->add_value(s);
                    
                }
                else
                {
                    // end of this option
                    arg = NULL;
                    argi --;
                }
            }
            // parse positional values
            else
            {
                if(pos_arg == _positionals.end())
                    break;
                if((*pos_arg)->nvalues() < (*pos_arg)->nargs_max())
                    (*pos_arg)->add_value(s);
                else
                {
                    ++ pos_arg;
                    argi --;
                }
            }
        }
        argi ++;
    }
    /*
    // parse positionals
    for(std::list<Argument*>::iterator it = _positionals.begin(); it != _positionals.end(); ++ it)
    {
        arg = *it;
        for(int i = 0; i < arg->nargs_max(); i ++)
        {
            if(argi >= argc)
                break;
            arg->add_value(argv[argi]);
            argi ++;
        }
        if(argi >= argc)
            break;
    }
    */
    if(_arguments["help"]->given())
    {
        help();
        exit(1);
    }
    if(argi < argc)
        throw ArgumentException(std::string("extra arguments given"));
    
    validate_values();
}

void ArgumentParser::help() const
{
    std::cout << "Usage: " << _progname << " [options] ";
    for(std::list<Argument*>::const_iterator it = _positionals.begin(); it != _positionals.end(); ++ it)
        std::cout << " " << (*it)->name();
    std::cout << "\n" << std::endl;
    std::cout << _description << "\n" << std::endl;
    std::cout << "Positional arguments:" << std::endl;
    for(std::list<Argument*>::const_iterator it = _positionals.begin();
        it != _positionals.end(); ++ it)
    {
        Argument* arg = *it;
        int pos = 0;
        std::cout << "  " << arg->name();
        pos = 2 + arg->name().size();
        for(int i = pos; i < _max_option_width; i ++)
            std::cout << ' ';
        std::cout << arg->help();
        std::cout << std::endl;
    }
    std::cout << "Optional arguments:" << std::endl;
    for(std::map<std::string, Argument*>::const_iterator it = _arguments.begin();
        it != _arguments.end(); ++it)
    {
        Argument* arg = it->second;
        if(arg->positional())
            continue;
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
        if(arg->help().size() > 0)
            std::cout << arg->help() << ". ";
        if(arg->required())
            std::cout << "Required. ";
        else
            std::cout << "Optional. ";
        if(!arg->default_value().Empty())
            std::cout << "Default is " << arg->default_value() << ". ";
        if(arg->_choices.size() > 0)
        {
            std::cout << "Choices:";
            for(size_t i = 0; i < arg->_choices.size(); i ++)
                std::cout << " " << arg->_choices[i];
            std::cout << ". ";
        }
        
        std::cout << std::endl;
    }
}

void ArgumentParser::summary() const
{
    std::cout << "All Arguments" << std::endl;
    std::cout << "Name\tType\tLong\tShort\tNArgs\tValue\tGiven\tHelp" << std::endl;
    for(std::map<std::string, Argument*>::const_iterator it = _arguments.begin(); it != _arguments.end(); ++it)
    {
        Argument* arg = it->second;
        std::string value;
        if(arg->nvalues() > 0)
            value = arg->value<std::string>();
        std::cout << arg->name()
            << "\t" << arg->type()
            << "\t" << arg->long_arg()
            << "\t" << arg->short_arg()
            << "\t" << arg->nargs_min() << "," << arg->nargs_max()
            << "\t" << value
            << "\t" << arg->given()
            << "\t" << arg->help()
            << std::endl;
    }
    std::cout << "Positionals:";
    for(std::list<Argument*>::const_iterator it = _positionals.begin(); it != _positionals.end(); ++it)
        std::cout << " " << (*it)->name();
    std::cout << std::endl;
    std::cout << "Options:";
    for(std::map<std::string, Argument*>::const_iterator it = _argmap.begin(); it != _argmap.end(); ++it)
        std::cout << " " << it->first;
    std::cout << std::endl;
}


#endif
