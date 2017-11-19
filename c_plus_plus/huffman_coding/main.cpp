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


	tree test1('a',123);
	cout<<"test1 addr:"<<&test1<<" test1.root　addr:"<<test1.root<<endl;

	tree test2;
	cout<<"initial test2 addr:"<<&test2<<" test2.root　addr:"<<test2.root<<endl;

	huff_coding.merge_tree(test1,test2);	
	cout<<"after copy test2 addr:"<<&test2<<" test2.root　addr:"<<test2.root<<endl;

//	huff_coding.create_tree(cnt,len, out);






		

	return 0;
}
