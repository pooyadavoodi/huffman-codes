#include <iostream>
#include "encoder.h"
#include "decoder.h"

using namespace std;

int main(int argc, char * argv[])
{
	if(argc == 2)
	{
		cout<<"compressing...\n";
		encoder e;
		e.compress(argv[1]);
		cout<<"Done!\n";
		return 0;
	}
	else if(argc == 3)
	{
	    if(string(argv[1]).compare("-d") != 0)
	    {
	        cerr<<"Error in arguments\n";
	        return 1;
	    }
		cout<<"Decompressing...\n";
		decoder d;
		d.decompress(argv[2]);
		cout<<"Done!\n";
		return 0;
	}
	//error in arguments
    cerr<<"no\n";
	return 1;
}



















