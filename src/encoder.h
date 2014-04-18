#ifndef _ENCODER_H
#define _ENCODER_H

#include <unordered_map>
#include <fstream>
#include <string>

#include "binary-tree.h"
#include "priority-queue.h"

using namespace std;

class encoder
{
	private:
		//calculating the freq
		unordered_map<char,int> gen_freq(ifstream &);

		//making the pq and initialize the binary tree with a collection of leaves with pointers stored at pq
		pqueue<char> gen_pq(const unordered_map<char,int> &);

		//making the huffman tree out of the pq - also store the code of each symbol
        binary_tree_node<char>* build_huffman_tree(const unordered_map<char,int> &);

		//recursive fnction that preorder traverses the huffman tree and generates the codes - called by gen_codes
        void rec_preorder_gen_codes(const binary_tree_node<char> * p, string & s,unordered_map<char,string> & codes_ht);

		//generate all the codes out of the huffman tree
        unordered_map<char,string> gen_codes(const binary_tree<char> & huffman_tree);

		//encode the input file using the codes
        string encode_input_file(ifstream & infile, const unordered_map<char,string> & codes_ht);

		//preorder traverses the huffman tree and puts the 0/1 chars and leaf data in encoded_huffman_tree and leaves_left_right
        void rec_preorder_traversal_encode(binary_tree_node<char> * p, string & , string &);

		//encode the huhffman tree
        pair<string,string> encode_huffman_tree(const binary_tree<char> & huffman_tree);

        unsigned char str01_to_char(const string::const_iterator & start, const string::const_iterator & end);
		//generates encoded_huffman_tree: consists of 2 parts, 1) structure which is 0/1 chars, 2) leaf symbols which correspond to the input chars
		//the two parts are seperated by a *
        void make_output_file(const string & encoded_file, const pair<string,string> & encoded_huffman_tree_plus_leaves, const string & outfilename);

	public:
		encoder();
		void compress(const string);
};

#endif // _ENCODER_H
