#! /usr/bin/env python
import itertools
from collections import defaultdict

c_types = ['bool', 'short', 'unsigned short', 'int', 'unsigned int',
        'long', 'unsigned long', 'float', 'double', 'long double']
int_types = ['bool', 'short', 'unsigned short', 'int', 'unsigned int',
        'long', 'unsigned long']
float_types = ['float', 'double', 'long double']

typestr = {t:t for t in c_types}
typestr['unsigned short'] = 'ushort'
typestr['unsigned long'] = 'ulong'
typestr['unsigned int'] = 'uint'
typestr['long double'] = 'ldouble'
compatible = defaultdict(defaultdict)
for t1, t2 in itertools.product(c_types, repeat=2):
    if ((t1 in int_types) and (t2 in int_types)) or ((t1 in float_types) and (t2 in float_types)):
        compatible[t1][t2] = True
    else:
        compatible[t1][t2] = False

def Space(n):
    return ''.join(itertools.repeat(' ', n))

def GenAnyTypeFromType(indent=4):
    space = Space(indent)
    print 'class AnyType::FromType'
    print '%sstatic AnyType FromType(const std::string& typestr) const {'%space
    print '%s    if(typestr == "str") return AnyType(std::string(""));'%space
    print '%s    else if(typestr == "string") return AnyType(std::string(""));'%space
    for t in c_types:
        print '%s    else if(typestr == "%s") return AnyType(static_cast<%s>(0));'%(space, typestr[t], t)
    print '%s    else throw BadAnyCast(std::string("Unsupported type: ") + typestr);'%space
    print '%s    return AnyType((int)(0));'%space
    print '%s}'%space
    print ''

def GenAnyTypeWithType(indent=4):
    space = Space(indent)
    print 'class AnyType::AnyType'
    print '%stemplate <typename ValType>'%space
    print '%sAnyType(const std::string& typestr, const ValType& v) {'%space
    print '%s    if(typestr == "str") value = new AnyValue<std::string>;'%space
    print '%s    else if(typestr == "string") value = new AnyValue<std::string>;'%space
    for t in c_types:
        print '%s    else if(typestr == "%s") value = new AnyValue<%s>;'%(space, typestr[t], t)
    print '%s    else throw BadAnyCast(std::string("Unsupported type string: ") + typestr);'%space
    print '%s    value->Store(v);'%space
    print '%s}'%space
    print ''

def GenAnyTypeConstructor(indent=4):
    space = Space(indent)
    print 'class AnyType::AnyType'
    for t in c_types:
        print '%sAnyType(%s val) { value = new AnyValue<%s>(val); }'%(space, t, t)
    print '%sAnyType(const std::string& val) { value = new AnyValue<std::string>(val); }'%space
    print ''

def GenAnyValueCast(indent=4):
    space = Space(indent)
    print 'class AnyValueBase::Cast'
    for t in c_types:
        print '%svirtual %s Cast(%s unused) const = 0;'%(space, t, t)
    print '%svirtual std::string Cast(const std::string& unused) const = 0;'%space;
    print ''
    print 'class AnyValue::Cast'
    for t in c_types:
        print '%svirtual %s Cast(%s unused) const { return static_cast<%s>(value); }'%(space, t, t, t)
    print '%svirtual std::string Cast(const std::string& unused) const { return ValueToString<ValType>(value); }'%(space)
    print ''
    print 'class AnyValue<std::string>::Cast'
    for t in c_types:
        print 'template<> %s AnyValue<std::string>::Cast(%s unused) const { return ValueFromString<%s>(value); }'%(t, t, t)
    print 'template<> std::string AnyValue<std::string>::Cast(const std::string& unused) const { return value; }'
    print ''

def GenAnyValueBaseStore(indent=4):
    space = Space(indent)
    print 'class AnyValueBase::Store'
    for t in c_types:
        print '%svirtual void Store(%s val) = 0;'%(space, t)
    print '%svirtual void Store(const std::string& val) = 0;'%space
    print ''

def GenAnyValueStore(indent=4):
    space = Space(indent)
    print 'class AnyValue::Store'
    for t in c_types:
        print '%svirtual void Store(%s val) { value = static_cast<ValType>(val); }'%(space, t)
    print '%svirtual void Store(const std::string& val) { FromString(val); }'%space
    print ''
    print 'class AnyValue<std::string>::Store'
    for t in c_types:
        print 'template<> void AnyValue<std::string>::Store(%s val) { value = ValueToString(val); }'%(t)
    print 'template<> void AnyValue<std::string>::Store(const std::string& val) { value = val; }'
    print ''

def GenAnyTypeStore(indent=4):
    space = Space(indent)
    print 'class AnyType::Store'
    for t in c_types:
        print '%sAnyType& Store(%s val) { if(value) value->Store(val); else value = new AnyValue<%s>(val); return *this; }'%(space, t, t)
    print '%sAnyType& Store(const std::string& val) { if(value) value->Store(val); else value = new AnyValue<std::string>(val); return *this; }'%(space)
    print ''

def GenAnyValueIsNumeric(indent=4):
    space = Space(indent)
    print 'class AnyValue::Numeric'
    print '%svirtual bool IsNumeric() const {'%space
    print '%s    return ('%space
    for i in xrange(len(c_types)):
        orstr = ' ||'
        if i == len(c_types) - 1:
            orstr = ''
        print '%s        (typeid(ValType) == typeid(%s))%s'%(space, c_types[i], orstr)

    print '%s    );'%space
    print '%s}'%space
    print ''
    
GenAnyValueCast()
GenAnyValueBaseStore()
GenAnyValueStore()
GenAnyValueIsNumeric()
#GenAnyTypeWithType()
GenAnyTypeFromType()
GenAnyTypeConstructor()
GenAnyTypeStore()

