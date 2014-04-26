#include <iostream>
#include <assert.h>
#include "bin-string-engine.hpp"

unsigned char bin_string_engine::str01_to_char(const string::const_iterator & strbegin, const string::const_iterator & strend)
{
	unsigned char ch = 0x00;
	uint8_t i=0x80;
	int j=0;
	while((strbegin+j) != strend)
	{
		if(*(strbegin+j) == '1')
			ch = ch | i;
		j++;
		i = i >> 1;
	}
	return ch;
}

//compress str: each char in str is 0/1 which should be compressed in one bit
string bin_string_engine::compress(const string str)
{
    string outstr("");
	size_t i;
	for(i=8; i < str.length(); i+=8)
		outstr.push_back(str01_to_char(str.cbegin()+i-8,str.cbegin()+i));
	outstr.push_back(str01_to_char(str.cbegin()+i-8,str.cend()));
	return outstr;
}

//return the string of length n which is the bit-sequence in ch from left to right
string bin_string_engine::char_to_str01(const unsigned char ch, size_t n)
{
    assert(n<=8);
    string str("");
    uint8_t j = 0x80;
    for(size_t i = 0; i<n; i++)
    {
        str.push_back('0' + ((ch&j)!=0));
        j = j >> 1;
    }
	return str;
}

//n: length of the output string; if str.length is k bytes, then n can be any number in range [(k-1)*8+1, k*8]
//n is essentially used to determine the number of valid bits in the last byte of str
string bin_string_engine::decompress(const string str, size_t n)
{
    string outstr("");
    size_t i;
    assert(((str.length()-1)*8)+1 <= n);
    assert(n <= (str.length()*8));
    for(i=0;i<(n/8); i++)
        outstr.append(char_to_str01(str[i],8));
    //translate the last byte
    outstr.append(char_to_str01(str[i],n%8));
    return outstr;
}












