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
    
template <typename ValType>
inline std::string ValueToString(ValType val)
{
    std::ostringstream os;
    os << val;
    return os.str();
}

template <typename ValType>
ValType ValueFromString(const std::string& s) {
    ValType v;
    std::istringstream is(s);
    is >> v;
    return v;
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
    virtual std::string ToString() const = 0;
    virtual void FromString(const std::string& s) = 0;
    virtual std::ostream& Output(std::ostream& os) const = 0;
    virtual std::istream& Input(std::istream& is) = 0;
    // conversion
    virtual bool Cast(bool unused) const = 0;
    virtual short Cast(short unused) const = 0;
    virtual unsigned short Cast(unsigned short unused) const = 0;
    virtual int Cast(int unused) const = 0;
    virtual unsigned int Cast(unsigned int unused) const = 0;
    virtual long Cast(long unused) const = 0;
    virtual unsigned long Cast(unsigned long unused) const = 0;
    virtual float Cast(float unused) const = 0;
    virtual double Cast(double unused) const = 0;
    virtual long double Cast(long double unused) const = 0;
    virtual std::string Cast(const std::string& unused) const = 0;
    // replace existing values
    virtual void Store(bool val) = 0;
    virtual void Store(short val) = 0;
    virtual void Store(unsigned short val) = 0;
    virtual void Store(int val) = 0;
    virtual void Store(unsigned int val) = 0;
    virtual void Store(long val) = 0;
    virtual void Store(unsigned long val) = 0;
    virtual void Store(float val) = 0;
    virtual void Store(double val) = 0;
    virtual void Store(long double val) = 0;
    virtual void Store(const std::string& val) = 0;
    
    virtual bool IsNumeric() const = 0;
    
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
    AnyValue() {}
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
    
    virtual std::string ToString() const {
        std::ostringstream os;
        os << value;
        return os.str();
    }
    
    virtual void FromString(const std::string& s) {
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
    
    // conversion
    virtual bool Cast(bool unused) const { return static_cast<bool>(value); }
    virtual short Cast(short unused) const { return static_cast<short>(value); }
    virtual unsigned short Cast(unsigned short unused) const { return static_cast<unsigned short>(value); }
    virtual int Cast(int unused) const { return static_cast<int>(value); }
    virtual unsigned int Cast(unsigned int unused) const { return static_cast<unsigned int>(value); }
    virtual long Cast(long unused) const { return static_cast<long>(value); }
    virtual unsigned long Cast(unsigned long unused) const { return static_cast<unsigned long>(value); }
    virtual float Cast(float unused) const { return static_cast<float>(value); }
    virtual double Cast(double unused) const { return static_cast<double>(value); }
    virtual long double Cast(long double unused) const { return static_cast<long double>(value); }
    virtual std::string Cast(const std::string& unused) const { return ValueToString<ValType>(value); }
    // store
    virtual void Store(bool val) { value = static_cast<ValType>(val); }
    virtual void Store(short val) { value = static_cast<ValType>(val); }
    virtual void Store(unsigned short val) { value = static_cast<ValType>(val); }
    virtual void Store(int val) { value = static_cast<ValType>(val); }
    virtual void Store(unsigned int val) { value = static_cast<ValType>(val); }
    virtual void Store(long val) { value = static_cast<ValType>(val); }
    virtual void Store(unsigned long val) { value = static_cast<ValType>(val); }
    virtual void Store(float val) { value = static_cast<ValType>(val); }
    virtual void Store(double val) { value = static_cast<ValType>(val); }
    virtual void Store(long double val) { value = static_cast<ValType>(val); }
    virtual void Store(const std::string& val) { FromString(val); }
    
    virtual bool IsNumeric() const {
        return (
            (typeid(ValType) == typeid(bool)) ||
            (typeid(ValType) == typeid(short)) ||
            (typeid(ValType) == typeid(unsigned short)) ||
            (typeid(ValType) == typeid(int)) ||
            (typeid(ValType) == typeid(unsigned int)) ||
            (typeid(ValType) == typeid(long)) ||
            (typeid(ValType) == typeid(unsigned long)) ||
            (typeid(ValType) == typeid(float)) ||
            (typeid(ValType) == typeid(double)) ||
            (typeid(ValType) == typeid(long double))
        );
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

template <> std::string AnyValue<bool>::ToString() const {
    return (value? "true" : "false");
}
template <> std::ostream& AnyValue<bool>::Output(std::ostream& os) const {
    if(value) os << "true";
    else os << "false";
    return os;
}
// cast
template<> bool AnyValue<std::string>::Cast(bool unused) const { return ValueFromString<bool>(value); }
template<> short AnyValue<std::string>::Cast(short unused) const { return ValueFromString<short>(value); }
template<> unsigned short AnyValue<std::string>::Cast(unsigned short unused) const { return ValueFromString<unsigned short>(value); }
template<> int AnyValue<std::string>::Cast(int unused) const { return ValueFromString<int>(value); }
template<> unsigned int AnyValue<std::string>::Cast(unsigned int unused) const { return ValueFromString<unsigned int>(value); }
template<> long AnyValue<std::string>::Cast(long unused) const { return ValueFromString<long>(value); }
template<> unsigned long AnyValue<std::string>::Cast(unsigned long unused) const { return ValueFromString<unsigned long>(value); }
template<> float AnyValue<std::string>::Cast(float unused) const { return ValueFromString<float>(value); }
template<> double AnyValue<std::string>::Cast(double unused) const { return ValueFromString<double>(value); }
template<> long double AnyValue<std::string>::Cast(long double unused) const { return ValueFromString<long double>(value); }
template<> std::string AnyValue<std::string>::Cast(const std::string& unused) const { return value; }
// store
template<> void AnyValue<std::string>::Store(bool val) { value = ValueToString(val); }
template<> void AnyValue<std::string>::Store(short val) { value = ValueToString(val); }
template<> void AnyValue<std::string>::Store(unsigned short val) { value = ValueToString(val); }
template<> void AnyValue<std::string>::Store(int val) { value = ValueToString(val); }
template<> void AnyValue<std::string>::Store(unsigned int val) { value = ValueToString(val); }
template<> void AnyValue<std::string>::Store(long val) { value = ValueToString(val); }
template<> void AnyValue<std::string>::Store(unsigned long val) { value = ValueToString(val); }
template<> void AnyValue<std::string>::Store(float val) { value = ValueToString(val); }
template<> void AnyValue<std::string>::Store(double val) { value = ValueToString(val); }
template<> void AnyValue<std::string>::Store(long double val) { value = ValueToString(val); }
template<> void AnyValue<std::string>::Store(const std::string& val) { value = val; }

class AnyType
{
    friend std::ostream& operator<<(std::ostream& os, const AnyType& a);
    friend std::istream& operator>>(std::istream& is, AnyType& a);
public:
    AnyType(): value(NULL) {}
    
    AnyType(bool val) { value = new AnyValue<bool>(val); }
    AnyType(short val) { value = new AnyValue<short>(val); }
    AnyType(unsigned short val) { value = new AnyValue<unsigned short>(val); }
    AnyType(int val) { value = new AnyValue<int>(val); }
    AnyType(unsigned int val) { value = new AnyValue<unsigned int>(val); }
    AnyType(long val) { value = new AnyValue<long>(val); }
    AnyType(unsigned long val) { value = new AnyValue<unsigned long>(val); }
    AnyType(float val) { value = new AnyValue<float>(val); }
    AnyType(double val) { value = new AnyValue<double>(val); }
    AnyType(long double val) { value = new AnyValue<long double>(val); }
    AnyType(const std::string& val) { value = new AnyValue<std::string>(val); }
    
    AnyType(const AnyType& a): value(NULL){
        if(a.value)
            value = a.value->Copy();
    }
    
    AnyType(const AnyValueBase& v) {
        value = v.Copy();
    }
    
#if 0
    template <typename ValType>
    AnyType(const std::string& typestr, const ValType& v) {
        if(typestr == "str") value = new AnyValue<std::string>(v);
        if(typestr == "string") value = new AnyValue<std::string>(v);
        if(typestr == "bool") value = new AnyValue<bool>(static_cast<bool>(v));
        if(typestr == "short") value = new AnyValue<short>(static_cast<short>(v));
        if(typestr == "ushort") value = new AnyValue<unsigned short>(static_cast<unsigned short>(v));
        if(typestr == "int") value = new AnyValue<int>(static_cast<int>(v));
        if(typestr == "uint") value = new AnyValue<unsigned int>(static_cast<unsigned int>(v));
        if(typestr == "long") value = new AnyValue<long>(static_cast<long>(v));
        if(typestr == "ulong") value = new AnyValue<unsigned long>(static_cast<unsigned long>(v));
        if(typestr == "float") value = new AnyValue<float>(static_cast<float>(v));
        if(typestr == "double") value = new AnyValue<double>(static_cast<double>(v));
        if(typestr == "ldouble") value = new AnyValue<long double>(static_cast<long double>(v));
        else throw BadAnyCast(std::string("Unsupported type string: ") + typestr);
    }
#endif
    ~AnyType() {
        if(value) delete value;
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
        return value->Cast(ValType2(0));
    }
    
    template <typename ValType>
    AnyType& operator=(const ValType& v) {
        if(value)
            delete value;
        else
            value = new AnyValue<ValType>(v);
        return *this;
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
    
    static AnyType Parse(const std::string& s, const std::string& typestr) {
        if((typestr == "str") || (typestr == "string"))
            return AnyType(s);
        else
        {
            AnyType a = AnyType::FromType(typestr);
            std::istringstream is(s);
            a.value->Input(is);
            if(!is)
                throw BadAnyCast("cannot parse string " + s + " as " + typestr);
            return a;
        }
    }
    static AnyType FromType(const std::string& typestr) {
        if(typestr == "str") return AnyType(std::string(""));
        else if(typestr == "string") return AnyType(std::string(""));
        else if(typestr == "bool") return AnyType(static_cast<bool>(0));
        else if(typestr == "short") return AnyType(static_cast<short>(0));
        else if(typestr == "ushort") return AnyType(static_cast<unsigned short>(0));
        else if(typestr == "int") return AnyType(static_cast<int>(0));
        else if(typestr == "uint") return AnyType(static_cast<unsigned int>(0));
        else if(typestr == "long") return AnyType(static_cast<long>(0));
        else if(typestr == "ulong") return AnyType(static_cast<unsigned long>(0));
        else if(typestr == "float") return AnyType(static_cast<float>(0));
        else if(typestr == "double") return AnyType(static_cast<double>(0));
        else if(typestr == "ldouble") return AnyType(static_cast<long double>(0));
        else throw BadAnyCast(std::string("Unsupported type: ") + typestr);
        return AnyType((int)(0));
    }

    AnyType& Store(bool val) { if(value) value->Store(val); else value = new AnyValue<bool>(val); return *this; }
    AnyType& Store(short val) { if(value) value->Store(val); else value = new AnyValue<short>(val); return *this; }
    AnyType& Store(unsigned short val) { if(value) value->Store(val); else value = new AnyValue<unsigned short>(val); return *this; }
    AnyType& Store(int val) { if(value) value->Store(val); else value = new AnyValue<int>(val); return *this; }
    AnyType& Store(unsigned int val) { if(value) value->Store(val); else value = new AnyValue<unsigned int>(val); return *this; }
    AnyType& Store(long val) { if(value) value->Store(val); else value = new AnyValue<long>(val); return *this; }
    AnyType& Store(unsigned long val) { if(value) value->Store(val); else value = new AnyValue<unsigned long>(val); return *this; }
    AnyType& Store(float val) { if(value) value->Store(val); else value = new AnyValue<float>(val); return *this; }
    AnyType& Store(double val) { if(value) value->Store(val); else value = new AnyValue<double>(val); return *this; }
    AnyType& Store(long double val) { if(value) value->Store(val); else value = new AnyValue<long double>(val); return *this; }
    AnyType& Store(const std::string& val) { if(value) value->Store(val); else value = new AnyValue<std::string>(val); return *this; }
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
