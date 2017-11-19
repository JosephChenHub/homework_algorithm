#include <iostream>

#include "file_process.hpp"

using namespace std;

int main(int argc, char * argv[])
{

	Huffman huff_coding;

	string file_name = "../../graph.txt";

	int len = 128;
	size_t *cnt = new size_t[len];
	cnt['g'] = 3;
	cnt['o'] = 3;
	cnt[' '] = 2;
	cnt['e'] = 1;
	cnt['s'] = 1;
	cnt['h'] = 1;
	cnt['p'] = 1;
	cnt['r'] = 1;

	
	

	huff_tree out;
	huff_coding.create_huff_tree(cnt,len, out);





		

	return 0;
}
