#include "file-content.hpp"

#include <fstream>
#include <iostream>
#include <assert.h>
#include "bin-string-engine.hpp"

namespace huffman_comp
{


//read n chars from infile and return it as a string.
//if less than n chars exist from the current location to the end of infile, it's an error
std::string file_content::read_next(std::ifstream & infile, int n)
{
    std::string read_content = "";
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
void file_content::read_content(const std::string & infilename)
{
    std::ifstream infile(infilename);
    if(!infile)
    {
        std::cerr<<"Error in openning file\n";
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
            std::cout<<ch;
    }
    std::cout<<std::endl;

    assert(infile.good() == false);//current pos should be at the eof now
    infile.close();
}


//read data from the object and store it in an output file
void file_content::write_content(const std::string & outfilename)
{
    std::ofstream outfile(outfilename);
    if(!outfile)
    {
        std::cerr<<"Error in openning file\n";
        return;
    }
    assert(outfile.good() == true);
    outfile << huffman_tree_size <<std::endl;
    outfile << leaves_size <<std::endl;
    outfile << encoded_filesize <<std::endl;

    outfile << bin_string_engine::compress(encoded_huffman_tree) <<std::endl;
    outfile << leaves <<std::endl;
    outfile << bin_string_engine::compress(encoded_file);

    outfile.close();
}

void file_content::print()
{
    std::cout<<"huffman_tree_size: "<<huffman_tree_size<<std::endl;
    std::cout<<"leaves_size: "<<leaves_size<<std::endl;
    std::cout<<"encoded_filesize: "<<encoded_filesize<<std::endl;
    std::cout<<"encoded_huffman_tree: "<<encoded_huffman_tree<<std::endl;
    std::cout<<"encoded_huffman_tree (compressed): "<<bin_string_engine::compress(encoded_huffman_tree)<<std::endl;
    std::cout<<"leaves: "<<leaves<<std::endl;
    std::cout<<"encoded_file: "<<encoded_file<<std::endl;
    std::cout<<"encoded_file (compressed): "<<bin_string_engine::compress(encoded_file)<<std::endl;
}


} // namespace huffman_comp













