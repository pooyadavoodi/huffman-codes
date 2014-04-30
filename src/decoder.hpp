#ifndef _DECODER_HPP
#define _DECODER_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "binary-tree.hpp"
#include "file-content.hpp"

namespace huffman_comp
{

class decoder
{
	private:
        void rec_preorder_make_huffman_tree(binary_tree_node<char> *, const std::string &, size_t &);
        binary_tree_node<char> * make_huffman_tree(const std::string &);

        void rec_preorder_make_codes_ht(binary_tree_node<char> *, const std::string &, size_t &, std::string &, std::unordered_map<std::string,char> &);
        //make hash table mapping codes to chars
        std::unordered_map<std::string,char> make_ht_codes(const binary_tree<char> &, const std::string &);

        std::string next_code(const std::string &, size_t &, const std::unordered_map<std::string,char> &);
        //decode using the hash table
        std::string decode_input_file(const std::string &, size_t, const std::unordered_map<std::string,char> &);

        //write the decoded file
        void make_output_file(const std::string &, const std::string);

    public:
        decoder();
        void decompress(const std::string infilename);
};

} // namespace huffman_comp


#endif //_DECODER_HPP
