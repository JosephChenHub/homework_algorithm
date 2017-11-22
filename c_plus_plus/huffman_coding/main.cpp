/*==================================
compile command:
	g++ main.cpp -o out -std=c++11
c++11 is required!
==================================*/
#include <iostream>

#include "file_process.hpp"

using namespace std;

int main(int argc, char * argv[])
{

	Huffman huff_coding;

	string input1  = "graph.txt";
	string output1 = "compress_graph.bin";
	string recover1 = "recover_graph.txt";
	string input2  = "Aesop_Fables.txt";
	string output2 = "compress_Aesop_Fables.bin";
	string recover2 = "recover_Aesop_Fables.txt";

	huff_coding.compress(input1, output1);
	huff_coding.compress(input2, output2);

	huff_coding.decompress(output1, recover1);
	huff_coding.decompress(output2, recover2);



	return 0;
}
