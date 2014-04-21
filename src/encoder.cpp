#include <iostream>
#include "encoder.h"

using namespace std;

encoder::encoder() {}

int streamSize(const string & infilename)
{
    ifstream infile(infilename);
    if(!infile)
    {
        cerr<<"Error in openning file\n";
        return -1;
    }
    streampos fbegin = infile.tellg();
    infile.seekg(0, ios::end);
    streampos fend = infile.tellg();
    infile.close();
    return fend - fbegin;
}


void encoder::compress(const string infilename)
{
    file_content outfile_content;
	unordered_map<char,int> freq_ht;
    binary_tree<char> huffman_tree;
	unordered_map<char,string> codes_ht;

	freq_ht = gen_freq(infilename);
    huffman_tree.setRoot(build_huffman_tree(freq_ht));
	codes_ht = gen_codes(huffman_tree);
	encode_input_file(infilename, codes_ht, outfile_content);
	encode_huffman_tree(huffman_tree, outfile_content);
	outfile_content.write_content(string(infilename+".zp"));
}

unordered_map<char,int> encoder::gen_freq(const string & infilename)
{
    unordered_map<char,int> freq_ht;

    ifstream infile(infilename);
    if(!infile)
    {
        cerr<<"Error in openning file\n";
        return freq_ht;
    }
	while(infile.good())
    {
        char ch = infile.get();
        if(infile.good())
            freq_ht[ch]++;
    }
    infile.close();
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

    //this is a special case, where the input file has only 1 char
    //it's a special case because we need to make sure huffman tree, besides its root, has a leaf corresponding to the only char
    if(pq.isEmpty())
    {
        binary_tree_node<char> * root = new binary_tree_node<char>;
        root->setLeft(leftChild.getNode());
        return root;
    }
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
	    if(p->getLeft())
        {
            s.push_back('0');
            rec_preorder_gen_codes(p->getLeft(),s,codes_ht);
            s.pop_back();
        }
        if(p->getRight())
        {
            s.push_back('1');
            rec_preorder_gen_codes(p->getRight(),s,codes_ht);
            s.pop_back();
        }
	}
}

unordered_map<char,string> encoder::gen_codes(const binary_tree<char> & huffman_tree)
{
    unordered_map<char,string> codes_ht;
	string s("");
	if(!huffman_tree.isEmpty())
        rec_preorder_gen_codes(huffman_tree.getRoot(),s,codes_ht);
	return codes_ht;
}

void encoder::encode_input_file(const string & infilename, const unordered_map<char,string> & codes_ht, file_content & outfile_content)
{
    ifstream infile(infilename);
    if(!infile)
    {
        cerr<<"Error in openning file\n";
        return;
    }
    outfile_content.encoded_file = "";
	while(infile.good())
	{
		char ch = infile.get();
		if(infile.good())
            outfile_content.encoded_file.append(codes_ht.at(ch));
	}
	outfile_content.encoded_filesize = outfile_content.encoded_file.length();
	infile.close();
}

void encoder::rec_preorder_traversal_encode(binary_tree_node<char> * p, file_content & outfile_content)
{
	if(!p->getLeft() && !p->getRight())
	{
		outfile_content.encoded_huffman_tree.append("00");
		outfile_content.leaves.append(1,p->getData());
	}
	else
	{
		if(p->getLeft())
		{
			outfile_content.encoded_huffman_tree.append("1");
			rec_preorder_traversal_encode(p->getLeft(), outfile_content);
		}
		else
			outfile_content.encoded_huffman_tree.append("0");
		if(p->getRight())
		{
			outfile_content.encoded_huffman_tree.append("1");
			rec_preorder_traversal_encode(p->getRight(), outfile_content);
		}
		else
			outfile_content.encoded_huffman_tree.append("0");
	}
}

void encoder::encode_huffman_tree(const binary_tree<char> & huffman_tree, file_content & outfile_content)
{
	if(!huffman_tree.isEmpty())
    {
        outfile_content.encoded_huffman_tree = "1";
		rec_preorder_traversal_encode(huffman_tree.getRoot(), outfile_content);
    }
	outfile_content.huffman_tree_size = outfile_content.encoded_huffman_tree.length();
	outfile_content.leaves_size = outfile_content.leaves.length();
}

