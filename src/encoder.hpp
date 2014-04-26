#ifndef _ENCODER_HPP
#define _ENCODER_HPP

#include <unordered_map>
#include <fstream>
#include <string>

#include "binary-tree.hpp"
#include "priority-queue.hpp"
#include "file-content.hpp"

using namespace std;

class encoder
{
	private:
		//calculating the freq
		unordered_map<char,int> gen_freq(const string &);

		//making the pq and initialize the binary tree with a collection of leaves with pointers stored at pq
		pqueue<char> gen_pq(const unordered_map<char,int> &);

		//making the huffman tree out of the pq - also store the code of each symbol
        binary_tree_node<char>* build_huffman_tree(const unordered_map<char,int> &);

		//recursive fnction that preorder traverses the huffman tree and generates the codes - called by gen_codes
        void rec_preorder_gen_codes(const binary_tree_node<char> * p, string & s,unordered_map<char,string> & codes_ht);

		//generate all the codes out of the huffman tree
        unordered_map<char,string> gen_codes(const binary_tree<char> & huffman_tree);

		//encode the input file using the codes
        void encode_input_file(const string &, const unordered_map<char,string> &, file_content &);

		//preorder traverses the huffman tree and puts the 0/1 chars and leaf data in encoded_huffman_tree and leaves_left_right
        void rec_preorder_traversal_encode(binary_tree_node<char> * p, file_content &);

		//encode the huhffman tree
        void encode_huffman_tree(const binary_tree<char> & huffman_tree, file_content &);

	public:
		encoder();
		void compress(const string);
};

#endif // _ENCODER_H
