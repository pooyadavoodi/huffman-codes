#ifndef _BIN_STRING_ENGINE_HPP
#define _BIN_STRING_ENGINE_HPP

#include <string>

namespace huffman_comp
{

class bin_string_engine
{
public:
    static unsigned char str01_to_char(const std::string::const_iterator &, const std::string::const_iterator &);
    static std::string char_to_str01(const unsigned char, size_t);
public:
    static std::string compress(const std::string);
    static std::string decompress(const std::string, size_t);
};

#endif // _BIN_STRING_ENGINE_HPP


} // namespace huffman_comp
