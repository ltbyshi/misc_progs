#include <stdio.h>
#include <wchar.h>

int main()
{
    wchar_t c = L'\u25A0';
    const wchar_t* s = L"Hello world \u25A0";

    wprintf(L"message %ls: %lc\n", s, c);
    return 0;
}
