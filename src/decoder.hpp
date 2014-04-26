#ifndef _DECODER_HPP
#define _DECODER_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "binary-tree.hpp"
#include "file-content.hpp"

using namespace std;

class decoder
{
	private:
        void rec_preorder_make_huffman_tree(binary_tree_node<char> *, const string &, size_t &);
        binary_tree_node<char> * make_huffman_tree(const string &);

        void rec_preorder_make_codes_ht(binary_tree_node<char> *, const string &, size_t &, string &, unordered_map<string,char> &);
        //make hash table mapping codes to chars
        unordered_map<string,char> make_ht_codes(const binary_tree<char> &, const string &);

        string next_code(const string &, size_t &, const unordered_map<string,char> &);
        //decode using the hash table
        string decode_input_file(const string &, size_t, const unordered_map<string,char> &);

        //write the decoded file
        void make_output_file(const string &, const string);

    public:
        decoder();
        void decompress(const string infilename);
};



#endif //_DECODER_HPP
