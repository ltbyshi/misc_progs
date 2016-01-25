#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

std::vector<std::string>& Split(const std::string& s,
        std::vector<std::string>& v,
        char delimiter='\t')
{
    std::istringstream is(s);
    std::string tok;
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

template <typename T, char Delimiter=' '>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    for(size_t i = 0; i < v.size() - 1; i ++)
    {
        if(i > 0)
            os << Delimiter;
        os << v[i];
    }
    return os;
}

int main()
{
    string s("one,two,three,four");
    cout << "Split string '" << s << "' by \\t:" << endl;
    cout << Split(s, ',') << endl;
    vector<string> v;
    Split(s, v, ',');
    cout << v << endl;
    return 0;
}
