#include <iostream>
#include "encoder.h"

using namespace std;

encoder::encoder() {}

void encoder::compress(const string infilename)
{
    ifstream infile(infilename);
    if(!infile)
    {
        cerr<<"Error in openning file\n";
        return;
    }

    string outfilename(infilename+".zip");
	unordered_map<char,int> freq_ht;
    binary_tree<char> huffman_tree;
	unordered_map<char,string> codes_ht;
    string encoded_file; //each bit is stored in a char - output converts them to bits and outputs them
    pair<string,string> encoded_huffman_tree_plus_leaves; //each bit of the structure encoding is stored in a char - output converts them to bits and outputs them - leaves of huffman tree from left to right, each leaf is a char

	freq_ht = gen_freq(infile);
    huffman_tree.setRoot(build_huffman_tree(freq_ht));
	codes_ht = gen_codes(huffman_tree);
	encoded_file = encode_input_file(infile, codes_ht);
	encoded_huffman_tree_plus_leaves = encode_huffman_tree(huffman_tree);
	make_output_file(encoded_file, encoded_huffman_tree_plus_leaves,outfilename);
}

unordered_map<char,int> encoder::gen_freq(ifstream & inputfile)
{
    unordered_map<char,int> freq_ht;
	while(inputfile.good())
		freq_ht[inputfile.get()]++;
	return freq_ht;
}

pqueue<char> encoder::gen_pq(const unordered_map<char,int> & freq_ht)
{
    pqueue<char> pq;

	//init the pq and huffman tree - create leaves and store pointers to them at the pq - no change to huffman_tree yet
	for(auto i  = freq_ht.begin() ; i != freq_ht.end(); i++)
	{
	    //pushing all the chars into pq - since each char corresponds to a node in the huffman tree, pq consists of a node pointer and freq
	    //allocation should occur in pq.push

		binary_tree_node<char> * leaf = new binary_tree_node<char>(i->first); //will be deleted in the destructor of huffman tree
        pq.push(i->second,leaf);
	}
	return pq;
}

binary_tree_node<char>* encoder::build_huffman_tree(const unordered_map<char,int> & freq_ht)
{
    //creat leaves and init pq: all the leaves can be accessed through pq
    pqueue<char> pq = gen_pq(freq_ht);

    if(pq.isEmpty())
        return nullptr;
    pq_node<char> leftChild = pq.toppop();
    //at each iteration: pop,push,pop: therefore size of pq decreases by 1
	while(!pq.isEmpty())
	{
        pq_node<char> rightChild = pq.toppop();
        binary_tree_node<char> * parent = new binary_tree_node<char>(leftChild.getNode(), rightChild.getNode());
        pq.push(leftChild.getFreq() + rightChild.getFreq(), parent);
		leftChild = pq.toppop();
	}
    //this will be the root of huffman tree
    return leftChild.getNode();
}

void encoder::rec_preorder_gen_codes(const binary_tree_node<char> * p, string & s,unordered_map<char,string> & codes_ht)
{
	if(!p->getLeft() && !p->getRight())
	{
		codes_ht[p->getData()] = s;
	}
	else
	{
		s.push_back('0');
		rec_preorder_gen_codes(p->getLeft(),s,codes_ht);
		s[s.length()-1] = '1';
		rec_preorder_gen_codes(p->getRight(),s,codes_ht);
		s.erase(s.length()-1,1); //delete the last char
	}
}

unordered_map<char,string> encoder::gen_codes(const binary_tree<char> & huffman_tree)
{
    unordered_map<char,string> codes_ht;
	string s("");
	rec_preorder_gen_codes(huffman_tree.getRoot(),s,codes_ht);
	return codes_ht;
}

string encoder::encode_input_file(ifstream & infile, const unordered_map<char,string> & codes_ht)
{
    string encoded_inputfile = "";
	while(infile.good())
	{
		char ch = infile.get();
		encoded_inputfile.append(codes_ht.at(ch));
	}
	return encoded_inputfile;
}

void encoder::rec_preorder_traversal_encode(binary_tree_node<char> * p, string & encoded_huffman_tree, string & leaves_left_right)
{
	if(!p->getLeft() && !p->getRight())
	{
		encoded_huffman_tree.append("00");
		leaves_left_right.append(1,p->getData());
	}
	else
	{
		if(p->getLeft())
		{
			encoded_huffman_tree.append("1");
			rec_preorder_traversal_encode(p->getLeft(), encoded_huffman_tree, leaves_left_right);
		}
		if(p->getRight())
		{
			encoded_huffman_tree.append("1");
			rec_preorder_traversal_encode(p->getRight(), encoded_huffman_tree, leaves_left_right);
		}
	}
}

pair<string,string> encoder::encode_huffman_tree(const binary_tree<char> & huffman_tree)
{
	string encoded_huffman_tree = "";
	string leaves_left_right = "";
	if(!huffman_tree.isEmpty())
		rec_preorder_traversal_encode(huffman_tree.getRoot(), encoded_huffman_tree, leaves_left_right);
	return pair<string,string>(encoded_huffman_tree,leaves_left_right);
}

unsigned char encoder::str01_to_char(const string::const_iterator & start, const string::const_iterator & end)
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
void encoder::make_output_file(const string & encoded_file, const pair<string,string> & encoded_huffman_tree_plus_leaves, const string & outfilename)
{
    ofstream outputfile(outfilename);
    if(!outputfile)
    {
        cerr<<"Error in creating the output file\n";
        return;
    }

	size_t i=8;
	while(i <= encoded_huffman_tree_plus_leaves.first.length())
	{
		outputfile << str01_to_char(encoded_huffman_tree_plus_leaves.first.cbegin()+i-8,encoded_huffman_tree_plus_leaves.first.cbegin()+i);
		i+=8;
	}
	outputfile << str01_to_char(begin(encoded_huffman_tree_plus_leaves.first)+i-8,end(encoded_huffman_tree_plus_leaves.first));
	outputfile << '*';

	outputfile << encoded_huffman_tree_plus_leaves.second;
	outputfile << '*';

	i=8;
	while(i <= encoded_file.length())
	{
		outputfile << str01_to_char(encoded_file.cbegin()+i-8,encoded_file.cbegin()+i);
		i+=8;
	}
	outputfile << str01_to_char(encoded_file.cbegin()+i-8,encoded_file.cend());
	outputfile.close();
}



