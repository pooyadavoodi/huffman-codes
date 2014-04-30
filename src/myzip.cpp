#include <iostream>
#include "encoder.hpp"
#include "decoder.hpp"

int main(int argc, char * argv[])
{
	if(argc == 2)
	{
		std::cout<<"compressing...\n";
		huffman_comp::encoder e;
		e.compress(argv[1]);
		std::cout<<"Done!\n";
		return 0;
	}
	else if(argc == 3)
	{
	    if(std::string(argv[1]).compare("-d") != 0)
	    {
	        std::cerr<<"Error in arguments\n";
	        return 1;
	    }
		std::cout<<"Decompressing...\n";
		huffman_comp::decoder d;
		d.decompress(argv[2]);
		std::cout<<"Done!\n";
		return 0;
	}
	//error in arguments
    std::cerr<<"no\n";
	return 1;
}



















