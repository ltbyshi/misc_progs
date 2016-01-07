#ifndef __ANYTYPE_H__
#define __ANYTYPE_H__

#include <iostream>
#include <sstream>
#include <typeinfo>
#include <exception>
#include <cxxabi.h>
#include <stdlib.h>

inline std::string Demangle(const char* name) 
{
    int status;
    char* s = abi::__cxa_demangle(name, 0, 0, &status);
    std::string fullname(s);
    free(s);
    return fullname;
}
    
class BadAnyCast: public std::bad_cast
{
public:
    BadAnyCast(const std::string op, 
               const std::type_info& ti1,
               const std::type_info& ti2) {
        std::ostringstream os;
        os << "Unmatched type in '" << Demangle(ti1.name()) 
            << "' " << op << " '" << Demangle(ti2.name()) << "'";
        msg = os.str();
    }
    BadAnyCast(const std::string msg): msg(msg) {}
    
    virtual const char* what() const noexcept {
        return msg.c_str();
    }
private:
    std::string msg;
};

class AnyValueBase
{
public:
    virtual ~AnyValueBase() {}
    virtual AnyValueBase* Copy() const = 0;
    virtual const std::type_info& Type() const = 0;
    virtual std::string TypeStr() const = 0;
    virtual std::string Str() const = 0;
    virtual void FromStr(const std::string& s) = 0;
    virtual std::ostream& Output(std::ostream& os) const = 0;
    virtual std::istream& Input(std::istream& is) = 0;
    // operators
    virtual AnyValueBase* add(const AnyValueBase* b) const = 0;
    //virtual AnyValueBase* sub(const AnyValueBase* b) const = 0;
    virtual bool eq(const AnyValueBase* b) const = 0;
    virtual bool ne(const AnyValueBase* b) const = 0;
    virtual bool gt(const AnyValueBase* b) const = 0;
    virtual bool lt(const AnyValueBase* b) const = 0;
    virtual bool ge(const AnyValueBase* b) const = 0;
    virtual bool le(const AnyValueBase* b) const = 0;
};

class AnyType;
template <typename ValType>
class AnyValue: public AnyValueBase
{
    friend class AnyType;
public:
    AnyValue(const ValType& v): value(v) {}
    
    virtual ~AnyValue() {}
    
    virtual AnyValueBase* Copy() const {
        return new AnyValue(value);
    }
    
    virtual const std::type_info& Type() const {
        return typeid(ValType);
    }
    
    std::string TypeStr() const {
        return Demangle(typeid(ValType).name());
    }
    
    virtual std::string Str() const {
        std::ostringstream os;
        os << value;
        return os.str();
    }
    
    virtual void FromStr(const std::string& s) {
        std::istringstream is(s);
        is >> value;
    }
    
    virtual std::ostream& Output(std::ostream& os) const {
        os << value;
        return os;
    }
    
    virtual std::istream& Input(std::istream& is) {
        is >> value;
        return is;
    }
    
    virtual AnyValueBase* add(const AnyValueBase* b) const{
        if(typeid(value) != b->Type())
            throw BadAnyCast("+", typeid(value), b->Type());
        return new AnyValue<ValType>(value + ((const AnyValue<ValType>*)b)->value);
    }
#define DECLARE_LOGICAL_OP(op, fn) \
    virtual bool fn(const AnyValueBase* b) const { \
        if(typeid(value) != b->Type()) \
            throw BadAnyCast(#op, typeid(value), b->Type()); \
        return (value op ((const AnyValue<ValType>*)b)->value); \
    }
    DECLARE_LOGICAL_OP(==, eq)
    DECLARE_LOGICAL_OP(!=, ne)
    DECLARE_LOGICAL_OP(<,  lt)
    DECLARE_LOGICAL_OP(>,  gt)
    DECLARE_LOGICAL_OP(<=, le)
    DECLARE_LOGICAL_OP(>=, ge)
#undef DECLARE_LOGICAL_OP

    
private:
    ValType value;
};

template <> std::string AnyValue<bool>::Str() const {
    return (value? "true" : "false");
}
template <> std::ostream& AnyValue<bool>::Output(std::ostream& os) const {
    if(value) os << "true";
    else os << "false";
    return os;
}

class AnyType
{
    friend std::ostream& operator<<(std::ostream& os, const AnyType& a);
    friend std::istream& operator>>(std::istream& is, AnyType& a);
public:
    AnyType(): value(NULL) {}
    
    template <typename ValType>
    AnyType(const ValType& v) {
        value = new AnyValue<ValType>(v);
    }
    
    AnyType(const AnyType& a): value(NULL){
        if(a.value)
            value = a.value->Copy();
    }
    
    AnyType(const AnyValueBase& v) {
        value = v.Copy();
    }
    
    ~AnyType() {
        if(value) delete value;
    }
    
    static AnyType FromType(std::string& typestr){
        if((typestr == "str") || (typestr == "string")) return AnyType(std::string(""));
        else if(typestr == "bool") return AnyType((bool)0);
        else if(typestr == "int") return AnyType((int)0);
        else if(typestr == "long") return AnyType((long)0);
        else if(typestr == "float") return AnyType((float)0);
        else if(typestr == "double") return AnyType((double)0);
        else if(typestr == "ldouble") return AnyType((long double)0);
        else if(typestr == "llong") return AnyType((long long)0);
        else throw BadAnyCast(std::string("Invalid type: ") + typestr);
        return AnyType((int)0);
    }
    
    AnyType& operator=(const AnyType& a) {
        if(value) 
            delete value;
        value = NULL;
        if(a.value)
            value = a.value->Copy();
        return *this;
    }
    
    template <typename ValType2>
    ValType2 Cast() const {
        if(!value)
            throw BadAnyCast("No values in AnyType");
        // convert to string
        if(typeid(ValType2) == typeid(std::string))
            return value->Str();
        if(value->Type() != typeid(ValType2))
            throw BadAnyCast("=>", value->Type(), typeid(ValType2));
        return ((const AnyValue<ValType2>*)value)->value;
    }
    
    template <typename ValType>
    AnyType& operator=(const ValType& v) {
        return Store<ValType>(v);
    }
    
 
    AnyType operator+(const AnyType& b) const {
        AnyType c;
        c.value = value->add(b.value);
        return c;
    }
#define DECLARE_LOGICAL_OP(op, fn) \
    bool operator op(const AnyType& b) const { return (value->fn(b.value)); }
    DECLARE_LOGICAL_OP(==, eq)
    DECLARE_LOGICAL_OP(!=, ne)
    DECLARE_LOGICAL_OP(<,  lt)
    DECLARE_LOGICAL_OP(>,  gt)
    DECLARE_LOGICAL_OP(<=, le)
    DECLARE_LOGICAL_OP(>=, ge)
#undef DECLARE_LOGICAL_OP
    
    template <typename ValType>
    AnyType& Store(const ValType& v) {
        if(value)
            delete value;
        value = new AnyValue<ValType>(v);
        return *this;
    }
    
    const std::type_info& Type() const {
        if(value)
            return value->Type();
        else
            return typeid(void);
    }
    
    const std::string TypeStr() const {
        if(value)
            return value->TypeStr();
        else
            return "void";
    }
    
    bool Empty() const {
        return (value == NULL);
    }
private:
    AnyValueBase* value;
};


inline std::ostream& operator<<(std::ostream& os, const AnyType& a) {
    if(a.value)
        return a.value->Output(os);
    // output empty string
    else
        return os;
}

inline std::istream& operator>>(std::istream& is, AnyType& a) {
    // default to string type if empty
    if(!a.value)
        a.value = new AnyValue<std::string>("");
    return a.value->Input(is);
}

#endif
