#include <iostream>
#include "encoder.h"
//#include "decoder.h"

using namespace std;

int main(int argc, char * argv[])
{
	if(argc == 2)
	{
		cout<<"yes\n";
		encoder e;
		e.compress(argv[1]);
		return 0;
	}
	cout<<"no\n";
	return 1;
}



















