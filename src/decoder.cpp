#include <assert.h>
#include "decoder.h"

decoder::decoder() {}

void decoder::decompress(const string infilename)
{
    file_content infile_content;
    binary_tree<char> huffman_tree;
	unordered_map<string,char> codes_ht;
    string decoded_file;

    infile_content.read_content(infilename);
    huffman_tree.setRoot(make_huffman_tree(infile_content.encoded_huffman_tree));
    //make hash table mapping codes to chars
    codes_ht = make_ht_codes(huffman_tree, infile_content.leaves);
    //decode using the hash table
    decoded_file = decode_input_file(infile_content.encoded_file, infile_content.encoded_filesize, codes_ht);
    //write the decoded file
    make_output_file(decoded_file, string (infilename+".unzp"));
}

//make huffman tree by reading chars of encoded_huffman_tree
//i denotes the position of the current char we are looking at in encoded_huffman_tree
void decoder::rec_preorder_make_huffman_tree(binary_tree_node<char> * p, const string & encoded_huffman_tree, size_t & i)
{
    if(i<encoded_huffman_tree.length())
    {
        if(encoded_huffman_tree[i++] == '1')
        {
            p->setLeft(new binary_tree_node<char>());
            rec_preorder_make_huffman_tree(p->getLeft(), encoded_huffman_tree, i);
        }
        assert(i<encoded_huffman_tree.length());//there must be a char for right child
        if(encoded_huffman_tree[i++] == '1')
        {
            p->setRight(new binary_tree_node<char>());
            rec_preorder_make_huffman_tree(p->getRight(), encoded_huffman_tree, i);
        }
    }
}

binary_tree_node<char> * decoder::make_huffman_tree(const string & encoded_huffman_tree)
{
    binary_tree_node<char> * root;

    int n = (encoded_huffman_tree.length()-1)/2;//number of nodes
    if(n>0)
    {
        assert(encoded_huffman_tree[0] == '1');//root must exist
        root = new binary_tree_node<char>();
        size_t i = 1;
        rec_preorder_make_huffman_tree(root, encoded_huffman_tree, i);
        assert(i == encoded_huffman_tree.length());
    }
    return root;
}

//traverse huffman_tree and when seeing a leaf, add a a (key,val) to codes_ht, where key: binary sequence from root-to-leaf, val: i-th entry in leaves
void decoder::rec_preorder_make_codes_ht(binary_tree_node<char> * p, const string & leaves, size_t & i, string & s, unordered_map<string,char> & codes_ht)
{
    if(i<leaves.length())
    {
        //if leaf, add it to codes_ht
        if((!p->getLeft()) && (!p->getRight()))
        {
            codes_ht[s] = leaves[i];
            i++;
        }
        else
        {
            if(p->getLeft())
            {
                s.push_back('0');
                rec_preorder_make_codes_ht(p->getLeft(), leaves, i, s, codes_ht);
                s.pop_back();
            }
            if(p->getRight())
            {
                s.push_back('1');
                rec_preorder_make_codes_ht(p->getRight(), leaves, i, s, codes_ht);
                s.pop_back();
            }
        }
    }
}

//make hash table mapping codes to chars
unordered_map<string,char> decoder::make_ht_codes(const binary_tree<char> & huffman_tree, const string & leaves)
{
    unordered_map<string,char> codes_ht;
    string s("");
    size_t i = 0;
    if(!huffman_tree.isEmpty())
        rec_preorder_make_codes_ht(huffman_tree.getRoot(), leaves, i, s, codes_ht);
    return codes_ht;
}

//return the first code that starts at position i and exists in codes_ht
string decoder::next_code(const string & encoded_file, size_t & i, const unordered_map<string,char> & codes_ht)
{
    string s("");
    size_t j;
    for(j=i; j<encoded_file.length(); j++)
    {
        s.append(1,encoded_file[j]);
        if (codes_ht.find(s) != codes_ht.end())
            break;
    }
    assert(j < encoded_file.length());//the last code must be a valid code
    i = j+1;
    return s;
}

//decode using the hash table
string decoder::decode_input_file(const string & encoded_file, size_t encoded_filesize, const unordered_map<string,char> & codes_ht)
{
    string decoded_file("");

    //loop over all the codes
    size_t i=0;
    while(i<encoded_file.length())
        decoded_file.push_back(codes_ht.at(next_code(encoded_file, i, codes_ht)));
    return decoded_file;
}

//write the decoded file
void decoder::make_output_file(const string & decoded_file, const string outfilename)
{
    ofstream outputfile(outfilename);
    if(!outputfile)
    {
        cerr<<"Error in creating the output file\n";
        return;
    }

    for(size_t i=0; i<decoded_file.length(); i++)
        outputfile<<decoded_file[i];
    outputfile.close();
}
















