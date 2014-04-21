#ifndef _BIN_STRING_ENGINE_H
#define _BIN_STRING_ENGINE_H

#include <string>

using namespace std;

class bin_string_engine
{
public:
    static unsigned char str01_to_char(const string::const_iterator &, const string::const_iterator &);
    static string char_to_str01(const unsigned char, size_t);
public:
    static string compress(const string);
    static string decompress(const string, size_t);
};

#endif // _BIN_STRING_ENGINE_H
