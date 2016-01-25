#ifndef __STRINGUTILS_H__
#define __STRINGUTILS_H__
#include <vector>
#include <string>

std::vector<std::string>& Split(const std::string& s,
        std::vector<std::string>& v,
        char delimiter='\t')
{
    std::istringstream is(s);
    std::string tok;
    v.resize(0);
    while(std::getline(is, tok, delimiter))
        v.push_back(tok);
    return v;
}

std::vector<std::string> Split(const std::string& s,
        char delimiter='\t')
{
    std::istringstream is(s);
    std::string tok;
    std::vector<std::string> v;
    while(std::getline(is, tok, delimiter))
        v.push_back(tok);
    return v;
}

#endif
