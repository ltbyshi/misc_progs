#include <iostream>
#include <string>
#include <iomanip>
#include <cstring>
using namespace std;

void PrintUnicode(wchar_t symbol)
{
    wcout << L"wchar_t(" << std::hex << static_cast<unsigned int>(symbol) << L"): " << symbol << endl;
}

void PrintUnicode(const char* s)
{
    char buf[4] = {};
    size_t length = strlen(s);
    strncpy(buf, s, 4);
    cout << "char(";
    for(size_t i = 0;i < length; i ++)
        cout << std::hex << static_cast<unsigned int>(buf[i]);
    cout << ", " << length << "): " << s << endl;
}

static unsigned int symbol_range[][2] = {
    {0x2580, 0x259F}, /* blocks */
    {0x25A0, 0x25FF}, /* geometric */
    {0x2460, 0x24FF}, /* enclosed */
    {0x2300, 0x23FF}, /* misc technical */
    {0x2190, 0x21FF}, /* arrows */
    {0x2150, 0x218B}, /* number forms */
    {0x0374, 0x03FF}, /* Greek and Coptic */
    {0x3041, 0x3096}, /* hiragana */
    {0x30A0, 0x30FF} /* katakana */
};

int main()
{
    const unsigned int nranges = sizeof(symbol_range)/sizeof(symbol_range[0]);

    /*
    for(unsigned int r = 0; r < nranges; r ++)
    {
        for(unsigned int i = symbol_range[r][0]; i <= symbol_range[r][1]; i ++)
            PrintUnicode(static_cast<wchar_t>(i));
    }
    */
    // PrintUnicode(L'あ');
    PrintUnicode("\u3042");
    PrintUnicode("あ");
    return 0;
}
