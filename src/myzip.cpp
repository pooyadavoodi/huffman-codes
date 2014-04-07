#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <stack>
#include <stdexcept>
#include <list>
#include <queue>
#include <sstream>
#include <map>
#include <set>
#include <climits>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

template <class T>
class binary_tree_node
{
	public:
		binary_tree_node<T> * left;
		binary_tree_node<T> * right;
		T data;

		binary_tree_node()
		{
			left = right = NULL;
		}
};

template <class T>
class binary_tree
{
	public:
		binary_tree_node<T> * root;
};

class encoder
{
	private:
		class pq_node
		{
			public:
				char ch;
				int freq;
				binary_tree_node<char> * node;
		};
		class maximum
		{
			public:
				bool operator()(const pq_node & a, const pq_node & b)
				{
					if(a.freq >= b.freq)
						return true;
					return false;
				}
		};
		class minimum
		{
			public:
				bool operator()(const pq_node & a, const pq_node & b)
				{
					if(a.freq <= b.freq)
						return true;
					return false;
				}
		};
		ifstream inputfile;
		ofstream outputfile;
		string encoded_file; //each bit is stored in a char - output converts them to bits and outputs them
		string encoded_huffman_tree; //each bit of the structure encoding is stored in a char - output converts them to bits and outputs them
		string leaves_left_right; //leaves of huffman tree from left to right, each leaf is a char
		binary_tree<char> huffman_tree;
		string input_filename;
		string output_filename;
		int inputfile_size;
		priority_queue<pq_node,vector<pq_node>,minimum> pq;

		//calculating the freq
		void gen_freq(unordered_map<char,int> & freq_ht);

		//making the pq and initialize the binary tree with a collection of leaves with pointers stored at pq
		void gen_pq(const unordered_map<char,int> & freq_ht);

		//making the huffman tree out of the pq - also store the code of each symbol
		void build_huffman_tree();

		//recursive fnction that preorder traverses the huffman tree and generates the codes - called by gen_codes
		void rec_preorder_gen_codes(binary_tree_node<char> * p, string & s,unordered_map<char,string> & codes_ht);

		//generate all the codes out of the huffman tree
		void gen_codes(unordered_map<char,string> & codes_ht);

		//encode the input file using the codes
		void encode_inputfile(const unordered_map<char,pair<int,uint8_t> > & codes_ht);

		//preorder traverses the huffman tree and puts the 0/1 chars and leaf data in encoded_huffman_tree and leaves_left_right
		void rec_preorder_traversal_encode(binary_tree_node<char> * p);

		//encode the huhffman tree
		void encode(const unordered_map<char,string> & codes_ht);

		//generates encoded_huffman_tree: consists of 2 parts, 1) structure which is 0/1 chars, 2) leaf symbols which correspond to the input chars
		//the two parts are seperated by a *
		void encode_huffman_tree();

	public:
		//sets the input file
		encoder(const char * input_filename);

		//main file that calls everything compression function
		void compress();

		//outputs the huffman tree and the encoded input file to the output file - encoding the huffman tree is done here
		void output();
};

encoder::encoder(const char * filename) : inputfile(filename)
{
	inputfile_size=0;
	input_filename = filename;
	output_filename = input_filename;
	output_filename.append(".zip");
}

void encoder::gen_freq(unordered_map<char,int> & freq_ht)
{
	while(inputfile.good())
	{
		freq_ht[inputfile.get()]++;
		inputfile_size++;
	}
}

void encoder::gen_pq(const unordered_map<char,int> & freq_ht)
{
	//init the pq and huffman tree - create leaves and store pointers to them at the pq - no change to huffman_tree yet
	for(auto i  = freq_ht.begin() ; i != freq_ht.end(); i++)
	{
		binary_tree_node<char> * leaf = new binary_tree_node<char>;
		leaf->data = i->first;

		pq_node p;
		p.ch = i->first;
		p.freq = i->second;
		p.node = leaf;
		pq.push(p);
	}
}

void encoder::build_huffman_tree()
{
	while(!pq.empty())
	{
		pq_node first_node = pq.top();
		pq.pop();
		if(pq.empty())
		{
			huffman_tree.root = first_node.node;
		}
		else
		{
			pq_node second_node = pq.top();
			pq.pop();
			pq_node parent;
			parent.freq = first_node.freq + second_node.freq;
			parent.node = new binary_tree_node<char>;
			parent.node->left = first_node.node;
			parent.node->right = second_node.node;
			pq.push(parent);
		}
	}
}

void encoder::rec_preorder_gen_codes(binary_tree_node<char> * p, string & s,unordered_map<char,string> & codes_ht)
{
	if(!p->left && !p->right)
	{
		codes_ht[p->data] = s;
	}
	else
	{
		s.push_back('0');
		rec_preorder_gen_codes(p->left,s,codes_ht);
		s[s.length()-1] = '1';
		rec_preorder_gen_codes(p->right,s,codes_ht);
		s.erase(s.length()-1,1); //delete the last char
	}
}

void encoder::gen_codes(unordered_map<char,string> & codes_ht)
{
	string s("");
	rec_preorder_gen_codes(huffman_tree.root,s,codes_ht);
}

void encoder::encode(const unordered_map<char,string> & codes_ht)
{
	while(inputfile.good())
	{
		char ch = inputfile.get();
		encoded_file.append(codes_ht.at(ch));
	}
}

void encoder::rec_preorder_traversal_encode(binary_tree_node<char> * p)
{
	if(!p->left && !p->right)
	{
		encoded_huffman_tree.append("00");
		leaves_left_right.append(1,p->data);
	}
	else
	{
		if(p->left)
		{
			encoded_huffman_tree.append("1");
			rec_preorder_traversal_encode(p->left);
		}
		if(p->right)
		{
			encoded_huffman_tree.append("1");
			rec_preorder_traversal_encode(p->right);
		}
	}
}

void encoder::encode_huffman_tree()
{
	encoded_huffman_tree = "";
	leaves_left_right = "";
	if(huffman_tree.root != NULL)
	{
		rec_preorder_traversal_encode(huffman_tree.root);
	}
}


void encoder::compress()
{
	unordered_map<char,int> freq_ht;
	unordered_map<char,string> codes_ht;

	gen_freq(freq_ht);
	gen_pq(freq_ht);
	build_huffman_tree();
	gen_codes(codes_ht);
	encode(codes_ht);
	encode_huffman_tree();
}


unsigned char str01_to_char(const string::iterator & start, const string::iterator & end)
{
	unsigned char ch = 0x00;
	uint8_t i=0x80;
	int j=0;
	while((start+j) != end)
	{
		if((*start) == '1')
			ch = ch | i;
		j++;
		i = i >> 1;
	}
	return ch;
}

//the main job is to convert chars to bits. then output to file
void encoder::output()
{
	outputfile.open(output_filename);
	outputfile<<inputfile_size;
	size_t i=8;
	while(i <= encoded_huffman_tree.length())
	{
		outputfile << str01_to_char(encoded_huffman_tree.begin()+i-8,encoded_huffman_tree.begin()+i);
		i+=8;
	}
	outputfile << str01_to_char(begin(encoded_huffman_tree)+i-8,end(encoded_huffman_tree));
	outputfile << '*';

	outputfile << leaves_left_right;
	outputfile << '*';

	i=8;
	while(i <= encoded_file.length())
	{
		outputfile << str01_to_char(begin(encoded_file)+i-8,begin(encoded_file)+i);
		i+=8;
	}
	outputfile << str01_to_char(begin(encoded_file)+i-8,end(encoded_file));
	outputfile.close();
}


int main(int argc, char * argv[])
{
	if(argc == 2)
	{
		cout<<"yes\n";
		encoder e(argv[1]);
		e.compress();
		e.output();
		return 0;
	}
	cout<<"no\n";
	return 1;
}



















