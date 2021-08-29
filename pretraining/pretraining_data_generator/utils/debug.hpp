#ifndef DEBUG
#define DEBUG

#include <fstream>
#include <codecvt>

void debug(std::wstring str){
    std::wofstream file;
    std::locale utf8(std::locale(), new std::codecvt_utf8_utf16<wchar_t>);
    file.imbue(utf8);
    file.open("test.txt",std::ios_base::app);
    file << str << "\n";
    file.close();
}

#endif