#include <exception>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
using namespace std;
#include <errno.h>
#include <rapidjson/document.h>     // rapidjson's DOM-style API
using rapidjson::Document;
using rapidjson::Value;
using rapidjson::SizeType;

class IOError: public std::exception {
public:
    explicit IOError(const std::string& what_arg) { msg = std::string("IOError: ") + what_arg + ": " + strerror(errno); }
    virtual const char* what() const noexcept { return msg.c_str(); }
private:
    std::string msg;
};

class JSONError: public std::exception {
public:
    explicit JSONError(const std::string& what_arg) { msg = std::string("JSONError: ") + what_arg; }
    virtual const char* what() const noexcept { return msg.c_str(); }
private:
    std::string msg;
};

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

#define ASSERT_JSON(expr) if(!expr) throw JSONError(#expr);

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s jsonfile\n", argv[0]);
        exit(1);
    }
    std::string jsonfile = argv[1];
    char* json = ReadFileAsString(jsonfile);
    
    Document document;
    if(document.ParseInsitu(json).HasParseError())
        throw JSONError("failed to parse JSON file " + jsonfile);
    ASSERT_JSON(document.HasMember("map"));
    ASSERT_JSON(document["map"].IsArray());
    cout << "map: " << document["map"].Size() << endl;
    ASSERT_JSON(document["outer"].IsArray());
    const Value& outer = document["outer"];
    cout << "outer: " << document["outer"].Size() << endl;
    for(SizeType i = 0; i < outer.Size(); i ++)
    {
        ASSERT_JSON(outer[i].HasMember("train"));
        ASSERT_JSON(outer[i]["train"].IsArray());
        cout << "outer[" << i << "]->train: " << outer[i]["train"].Size() << endl;
        ASSERT_JSON(outer[i].HasMember("test"));
        ASSERT_JSON(outer[i]["test"].IsArray());
        cout << "outer[" << i << "]->test: " << outer[i]["test"].Size() << endl;
        ASSERT_JSON(outer[i].HasMember("inner"));
        ASSERT_JSON(outer[i]["inner"].IsArray());
        cout << "outer[" << i << "]->inner: " << outer[i]["inner"].Size() << endl;
        const Value& inner = outer[i]["inner"];
        for(SizeType j = 0; j < inner.Size(); j ++)
        {
            ASSERT_JSON(inner[j].HasMember("train"));
            ASSERT_JSON(inner[j]["train"].IsArray());
            cout << "outer[" << i << "]->inner[" << j << "]->train: " << inner[j]["train"].Size() << endl;
            ASSERT_JSON(inner[j].HasMember("test"));
            ASSERT_JSON(inner[j]["test"].IsArray());
            cout << "outer[" << i << "]->inner[" << j << "]->test: " << inner[j]["test"].Size() << endl;
        }
    }
        
    return 0;
}

