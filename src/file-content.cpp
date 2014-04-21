#include "file-content.h"

#include <fstream>
#include <iostream>
#include <assert.h>
#include "bin-string-engine.h"


//read n chars from infile and return it as a string.
//if less than n chars exist from the current location to the end of infile, it's an error
string file_content::read_next(ifstream & infile, int n)
{
    string read_content = "";
    int i;
    for(i=0;(i<n) && (infile.good());i++)
    {
        char ch = infile.get();
//        cout<<"ch: "<<ch<<endl;
        if(infile.good())
            read_content.push_back(ch);
    }
//    cout<<"i= "<<i<<endl;
//    cout<<"n= "<<n<<endl;
//    cout<<"infile.good()= "<<infile.good()<<endl;
    assert(i==n);//error if it can't read all the n bytes
    return read_content;
}

//read from an input file and store data in the object
void file_content::read_content(const string & infilename)
{
    ifstream infile(infilename);
    if(!infile)
    {
        cerr<<"Error in openning file\n";
        return;
    }
    assert(infile.good() == true);
    infile >> huffman_tree_size;
    infile >> leaves_size;
    infile >> encoded_filesize;
    infile.get(); //skipping a char

//    cout<<"read huffman tree\n";
    encoded_huffman_tree = read_next(infile, (huffman_tree_size-1)/8+1);
    infile.get(); //skipping a char
//    cout<<"encoded_huffman_tree= "<<encoded_huffman_tree<<endl;
    encoded_huffman_tree = bin_string_engine::decompress(encoded_huffman_tree, huffman_tree_size);

//    cout<<"read leaves\n";
    leaves = read_next(infile, leaves_size);
    infile.get(); //skipping a char

//    cout<<"read encoded file\n";
    encoded_file = bin_string_engine::decompress(read_next(infile, (encoded_filesize-1)/8+1), encoded_filesize);
    infile.get(); //skipping a char

    while(infile.good())
    {
        char ch = infile.get();
        if(infile.good())
            cout<<ch;
    }
    cout<<endl;

    assert(infile.good() == false);//current pos should be at the eof now
    infile.close();
}


//read data from the object and store it in an output file
void file_content::write_content(const string & outfilename)
{
    ofstream outfile(outfilename);
    if(!outfile)
    {
        cerr<<"Error in openning file\n";
        return;
    }
    assert(outfile.good() == true);
    outfile << huffman_tree_size <<endl;
    outfile << leaves_size <<endl;
    outfile << encoded_filesize <<endl;

    outfile << bin_string_engine::compress(encoded_huffman_tree) <<endl;
    outfile << leaves <<endl;
    outfile << bin_string_engine::compress(encoded_file);

    outfile.close();
}

void file_content::print()
{
    cout<<"huffman_tree_size: "<<huffman_tree_size<<endl;
    cout<<"leaves_size: "<<leaves_size<<endl;
    cout<<"encoded_filesize: "<<encoded_filesize<<endl;
    cout<<"encoded_huffman_tree: "<<encoded_huffman_tree<<endl;
    cout<<"encoded_huffman_tree (compressed): "<<bin_string_engine::compress(encoded_huffman_tree)<<endl;
    cout<<"leaves: "<<leaves<<endl;
    cout<<"encoded_file: "<<encoded_file<<endl;
    cout<<"encoded_file (compressed): "<<bin_string_engine::compress(encoded_file)<<endl;
}















