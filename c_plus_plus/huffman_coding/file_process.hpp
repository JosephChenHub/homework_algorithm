#ifndef __FILE_PROCESS_H
#define __FILE_PROCESS_H

#include <iostream>
#include <fstream>
#include <algorithm> //use stl sort
#include <map>  	 //use map
#include <vector>
#include <memory>
#include <iomanip>

#define DEBUG_ON 0  //print some info.

const int CODE_WIDTH = 128;

/*=================================================================
 compressed file info:
	char: \alpha--224,\beta--225  \pi 227
	header: \alpha,\beta,header_size(uint16_t), char+weight,char+weight,..., \beta,\alpha
 	char---1byte weight---[3][2][1][0] 4 bytes
 	tail : \pi, + extra 0 , checksum, \pi (extra=0,since 227=b11100011)
	checksum (type:char) is all bytes sum not included the final \pi.
decompressed file:
	scan over bytes, find the header and tail , check file;
	extract huffman tree,
	decode file
==================================================================*/
//! node with weight
struct node{
	char data;
	uint32_t weight;
	struct node * left;
	struct node * right;
};


//! huff_tree
class huff_tree{
public:
	huff_tree()
	{
		_height = 0;
		root = NULL;
	}
	huff_tree(char ch, uint32_t w)
	{
		create(ch,w);
	}
	~huff_tree()
	{
#if DEBUG_ON 
		std::cout<<"destroy add:"<<this<<" root_addr:"<<root<<std::endl;
#endif
		destroy_tree();
	}
	//! create a tree with one node
	void create(char ch, uint32_t w)
	{
		destroy_tree(root);
		root = new node;
		root->left = NULL;
		root->right = NULL;
		root->data = ch;
		root->weight = w; 
		_height = 1;
		update_height(); 
	}
	//!height of the huff_tree
	int height() const 
	{
		return  _height;
	}
	//!merge trees
	void merge_tree(huff_tree &src)
	{
		merge_tree(src.root, root);
		update_height();
	}
	uint32_t root_weight()const
	{
		return root->weight;
	}
	//!destroy this tree
	void destroy_tree()
	{
		destroy_tree(root);
	}
	//! print the tree to screen
	void postorder(node* p, int indent=0)
	{
    	if(p != NULL) 
		{
        	if(p->right)
			 {
            	postorder(p->right, indent+4);
       		 }
        	if (indent) 
			{
            	std::cout << std::setw(indent) << ' ';
        	}
        	if (p->right) std::cout<<" /\n" << std::setw(indent) << ' ';
			if(uint8_t(p->data) == 255)
			{
				std::cout<<p->weight<<"\n";
			}
			else if(p->data == '\n')
			{
				std::cout<<"\\n,"<<p->weight<<"\n";
			}
			else {
        		std::cout<< p->data<<","<<p->weight << "\n ";
			}
        	if(p->left) 
			{
            	std::cout << std::setw(indent) << ' ' <<" \\\n";
            	postorder(p->left, indent+4);
        	}
    	}
	}
	//!depth
	int depth(char data) 
	{
		return depth(data, root);
	}
	int depth(node *p)
	{
		return depth(p,root);
	}
	//! create map
	void create_map(char data, std::vector<bool> &bits)
	{
		int d = depth(data);
		bits.resize(d);
		int result = -1;
		create_map(data, root, bits,result);
	}
	void create_map(std::map<char , std::vector<bool> > &out)
	{
		std::vector<bool> bits;
		bits.reserve(_height);
		create_map(root, bits, out);
	}
	int is_leaf(node * p)
	{
		if(p != NULL && p->left == NULL && p ->right == NULL)
		{
			return 0;
		}
		return -1;
	}
public:
	node *  root;

private:
	int _height;
	//! destroy a tree of root s	
	void destroy_tree(node * s)
	{
		if(s == NULL)
		{
			return;
		}
		destroy_tree(s->left);
		destroy_tree(s->right);
		delete s;
		s = NULL;
	}
	//!height of a node
	int height(node * s)
	{
		if(s == NULL)
		{
			return 0;
		}	
		int temp1 = height(s->left) + 1;
		int temp2 = height(s->right) + 1;
		
		return (temp1 > temp2? temp1:temp2);
	}
	//! depth of node 
	int depth(char data, node *s, int d = 0)
	{
		if(s == NULL)
		{
			return -1;
		}
		if(s->data == data )//leaf
		{
			return d;
		}
		int temp1 = depth(data, s->left, d+1);
		int temp2 = depth(data, s->right, d+1);
		return temp1>temp2?temp1:temp2;
	}
	int depth(node *p, node *s, int d = 0)
	{
		if(s == NULL || p == NULL)
		{
			return -1;
		}
		if(s == p)
		{
			return d;
		}
		int temp1 = depth(p, s->left, d+1);
		int temp2 = depth(p, s->right, d+1);
		return temp1>temp2?temp1:temp2;
	}
	//!if p exists, return 0;else -1
	int search(char data,node *&p , node *&s)
	{
		if(s == NULL)
		{
			return -1;
		}
		if(s->data == data)
		{
			p = s;
			return 0;
		}
		int temp1 = search(data,p, s->left);
		int temp2 = search(data,p, s->right);

		return temp1>temp2?temp1:temp2;	
	}
	//!update height of tree
	void update_height()
	{
		_height = height(root);
	}
	//!merge
	void merge_tree(node * &src, node * &dst)
	{
		if(src == NULL)
		{
			return ;
		}
		else if(dst == NULL) //copy from src
		{
			dst = new node;
			dst->data = src->data;
			dst->weight = src->weight;
			dst->left = NULL;
			dst->right = NULL;

			merge_tree(src->left,dst->left);
			merge_tree(src->right, dst->right);	
			
			return;
		}
		node * new_root = new node;
		new_root->data = 255;
		new_root->weight = src->weight + dst->weight;
		new_root->left = NULL;
		new_root->right = NULL;
			
		merge_tree(src, new_root->left);
		merge_tree(dst, new_root->right);
			
		destroy_tree(dst);

		dst = new node;
		dst = new_root;
	}
	//! create map char data----code
	void create_map(char data,node *s, std::vector<bool> &bits,int &result)
	{
		if(s == NULL)
		{
			return ;
		}
		int d = depth(s);
		if(s->left && result != 0) //
		{
			bits[d] = 0; //!left edge = 0
			if(s->left->data == data)
			{
					result = 0;
			}
			create_map(data,s->left,  bits , result);
		}
		if(s->right && result != 0)
		{
			bits[d] = 1;
			if(s->right->data == data)
			{
				result = 0;
			}
			create_map(data,s->right,bits, result);
		}		
	}
	void create_map(node *s, std::vector<bool> &bits, std::map<char, std::vector<bool> > & table)
	{
		if(s == NULL)
		{
			return;
		}	
		int d = depth(s);
		if(s->left)
		{
			if(bits.size() >= d+1) //len(bits) = depth(s->left)
			{
				while(bits.size() != d+1)
				{
					bits.pop_back();						
				}
				bits[d] = 0;
			} 
			else 
			{
				bits.push_back(0);
			}
			
			if(s->left->left == NULL && s->left->right == NULL) //leaf
			{
				table[s->left->data] = bits; //insert 
			}
			create_map(s->left,bits, table);
		}
		if(s->right)
		{
			if(bits.size() >= d+1) //len(bits) = depth(s->left)
			{
				while(bits.size() != d+1)
				{
					bits.pop_back();						
				}
				bits[d] = 1;
			} 
			else
			{
				bits.push_back(1);
			}
			if(s->right->left == NULL && s->right->right == NULL)
			{
				table[s->right->data] = bits;
			}
			create_map(s->right, bits, table);
		}
	
	}



};


//!Huffman encoding and decoding
class Huffman{
public: 
	Huffman()
	{
	}
	~Huffman()
	{
	}
	void compress(std::string & input_file, std::string &out_file)
	{
		std::cout<<"compressing file..."<<std::endl;
		//!read all bytes from input file
		std::ifstream file(input_file, std::ios::in|std::ios::binary);
		if(!file.is_open())
		{
			std::cout<<"open file failed: "<<input_file<<"!"<<std::endl;			
			return ;
		}
		//!read all characters
		std::vector<char> contents(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
		file.close();
		std::cout<<"read "<<contents.size()<<" bytes from file:"<<input_file<<std::endl;

		std::map<char, std::vector<bool> > table;
		std::map<char,uint32_t> weights;

		create_huff_table(contents, weights, table); //!

		//!encode
		std::vector<bool> out_bits;
		uint16_t header_size = weights.size()*5 + 4 + 2;  //alpha,beta,header size + xxxx + beta + alpha
		char *header = new char[header_size];
	


		out_bits.reserve((contents.size()+weights.size())*8);
		//!write to out_file
		std::ofstream output(out_file, std::ios::out|std::ios::binary);		
		size_t bits_cnt = 0;	
		//!header: \alpha,\beta, char+weight,char+weight,..., \beta,\alpha
		//! char---1byte weight---[3][2][1][0] 4 bytes
		//!tail : \pi, + extra 1, + \pi
		
		header[0] = FIX[0];
		header[1] = FIX[1];
		header[2] = char((header_size >> 8)&0xFF);
		header[3] = char(header_size &0xFF);

		char buffer[4];
		int index = 4;
		for(std::map<char,uint32_t>::iterator it = weights.begin(); it != weights.end(); ++it)
		{
			//char 1bytes + weight 4bytes
			char data = it->first;
			uint32_t w = it->second;
	
		    buffer[0] = char((w >> 24) & 0xFF);
		    buffer[1] = char((w >> 16) & 0xFF);
		    buffer[2] = char((w >>  8) & 0xFF);
		    buffer[3] = char((w      ) & 0xFF);

			header[index++] = data;
			for(int j = 0; j < 4; ++j)
			{
				header[index++] = buffer[j];				
			}		
		}
		header[index++] = FIX[1];
		header[index] = FIX[0];

		//encode 
		for(std::vector<char>::iterator it = contents.begin(); it != contents.end(); ++it)
		{
			std::map<char, std::vector<bool> >::iterator b_it = table.find(char(*it));
			if(b_it != table.end())
			{
				out_bits.insert(out_bits.end(), b_it->second.begin(), b_it->second.end());
			}
			else
			{
				std::cout<<"char:"<<*it<<" not found in table!!!";
				contents.clear();
				out_bits.clear();
				output.close();
				return;
			}
		}
		
		//!append tail---  \pi, extra 1, check sum, \pi 
		for(int j = 7; j >= 0; --j)
		{
			out_bits.push_back((FIX[2]>>j)&0x1);
		}
		int remainder = 8 - out_bits.size()%8;
		for(int i = 0;i < remainder; ++i)
		{
			out_bits.push_back(0);
		}	
		char check_sum = 0;
		for(int i = 0; i < header_size; ++i)
		{
			check_sum |= header[i];
		}
		//!write to file
		output.write(header, header_size);
		char temp = 0;
		for(size_t i = 0; i < out_bits.size(); i = i+8)
		{		
			temp = 0;
			for(int j = 0; j < 8; ++j)
			{
				temp |= (out_bits[i + j] <<(7-j));
			}
			check_sum |= temp;
			output.write(&temp,1);
		}				
		//!write check_sum + \pi
		output.write(&check_sum, 1);
		output.write(&FIX[2],1);	

		bits_cnt = header_size*8 + out_bits.size() + 2*8;

		
#if DEBUG_ON
		std::cout<<"code table:"<<std::endl;
#endif
		int max_depth = 0;
		size_t mini_bits = 0;
		for(std::map<char, std::vector<bool> >::iterator it = table.begin(); it != table.end(); ++it)
		{
#if DEBUG_ON
			if(it->first == '\n')
			{
				std::cout<<"char:\\n"<<" code:";
			}
			else
			{
				std::cout<<"char:"<<it->first<<" code:";
			}
			std::vector<bool>::iterator b_it = it->second.begin();
			for( ; b_it != it->second.end(); ++b_it)
			{
				std::cout<<*b_it;
			}
			std::cout<<std::endl;
#endif
			if(max_depth < it->second.size())
			{
				max_depth = it->second.size();
			}
			mini_bits += 1*8 + it->second.size();
		}
#if DEBUG_ON
		std::cout<<"max_depth:"<<max_depth<<std::endl;
		std::cout<<"bits of char + code:"<<mini_bits<<std::endl;
		std::cout<<"bits of current extra:"<<bits_cnt - out_bits.size()<<std::endl; 
#endif
		std::cout<<"write "<<bits_cnt/8.0<<" bytes to file:"<<out_file <<std::endl;
		std::cout<<"compression ratio:"<< bits_cnt/8.0/contents.size()<<std::endl;

		output.close();
		std::vector<bool>().swap(out_bits);
		std::vector<char>().swap(contents);

		delete [] header;


	}
	void decompress(std::string &input_file, std::string & out_file)
	{
		std::cout<<"decompressing file ..."<<std::endl;
		//!file
		std::ifstream file(input_file, std::ios::in|std::ios::binary);
		if(!file.is_open())
		{
			std::cout<<"open file failed: "<<input_file<<"!"<<std::endl;			
			return ;
		}
		//!read all characters
		std::vector<char> contents(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
		file.close();
		std::cout<<"read "<<contents.size()<<" bytes from file:"<<input_file<<std::endl;
		//!scan to extract header ,tail
		std::map<char,uint32_t> weights;
		size_t header_index = 0;
		uint16_t header_size = 0;
		size_t check_end = 0;
		char read_check_sum = 0;
		int remainder = 0;

		for(size_t i = 0; i < contents.size(); ++i)
		{
			if(contents[i] == FIX[0] && contents[i+1] == FIX[1]) //!maybe header
			{
				header_size = uint16_t(uint8_t(contents[i+2])<<8| uint8_t(contents[i+3]));
				if(contents.size() < header_size)//!wrong 
				{
					return ;
				}
				if(contents[i+header_size -2] == FIX[1] && contents[i+header_size-1] == FIX[0]) //!find the header
				{
					header_index = i;
					break;
				}
			}
			else
			{
				if(i >= contents.size()-3)
				{
					std::cout<<"can not find the header at the of input file!"<<std::endl;
					std::cout<<"please check the completeness of file!"<<input_file<<std::endl;
					return;
				}
			}	
		}	
		for(size_t i = contents.size()-1; i >= 0; --i)
		{
			if(contents[i] == FIX[2]) //may be the final tail
			{
				//FIX , extra 0 , checksum , FIX at most 3bytes+7bits
				read_check_sum = contents[i-1];
				//FIX[2] = 227=b11100011														
				uint16_t temp = uint16_t(uint8_t(contents[i-3])<<8| uint8_t(contents[i-2]));
				std::vector<bool> bits;
				for(int i = 0; i < 8; ++i)
				{
					bool bit = ((temp>>i)&0x1);
					if(bit == 0)
					{
						remainder ++;
					} 
					else
					{
						break;
					}
				}			
				char fix = char((temp>>remainder)&0xff);
				if(fix != FIX[2])
				{
					std::cout<<"can not find the tail at the of input file!"<<std::endl;
					std::cout<<"please check the completeness of file!"<<input_file<<std::endl;
					return;
				}
				check_end = i - 1;
				break;
			}
			else
			{
				if(i <= 6)
				{
					std::cout<<"can not find the tail at the of input file!"<<std::endl;
					std::cout<<"please check the completeness of file!"<<input_file<<std::endl;
					return;
				}  
			}
		}	
		char sum = 0;
		for(int i = 0; i < check_end; ++i)
		{
			sum |= contents[i];
		}
		if(sum != read_check_sum)
		{
			std::cout<<"check sum failed! "<<std::endl;
			std::cout<<"please check the completeness of file!"<<input_file<<std::endl;
			return;
		}
		//!extract weights from header_index, length = header_size
		for(size_t i = header_index + 4; i < header_size - 2; )
		{
			char data = contents[i++];
			uint32_t weight = uint32_t( uint8_t(contents[i++]) << 24| uint8_t(contents[i++]) << 16| \
					uint8_t(contents[i++]) << 8 | uint8_t(contents[i++]));
			weights[data] = weight;
		}
		//!create huffman tree
		huff_tree tree;
		create_huff_tree(weights, tree);
		//! convert to binary vectors
		size_t start_index = header_index + header_size;
		size_t end_index = check_end - 1;
		std::vector<bool> bits_stream;
		bits_stream.reserve(contents.size()*8);
		for(size_t i = start_index; i <= end_index; ++i)
		{
			for(int j = 7; j >= 0; --j)
			{
				bits_stream.push_back( (contents[i] >> j)&0x1); 
			}
		}
		for(int i = 0; i < remainder + 8; ++i)
		{
			bits_stream.pop_back(); 
		}
		//!decode
	    std::vector<char> bytes;
		bytes.reserve(bits_stream.size()/4);

		size_t root_index = 0;
		node * root =  new node;
		root = tree.root;

		for( ; root_index < bits_stream.size(); ++root_index)
		{
			if(bits_stream[root_index] == 0)
			{
				root = root->left;
				if(tree.is_leaf(root) == 0)
				{
					bytes.push_back(root->data);
					root = tree.root;
					continue;
				}  
			}
			else
			{
				root = root->right;
				if(tree.is_leaf(root) == 0)
				{
					bytes.push_back(root->data);
					root = tree.root;
					continue;
				}
			}			
		}
		//!write to file
		std::ofstream output(out_file, std::ios::out|std::ios::binary);		
		char * out_buf = new char[bytes.size()];		
		for(size_t i = 0; i < bytes.size(); ++i)
		{
			out_buf[i] = bytes[i];
		}	
		output.write(out_buf, bytes.size());
		output.close();

		std::cout<<"extract "<<bytes.size()<<" bytes from :"<<input_file \
			<<" output file:"<<out_file<<std::endl;
	
		std::vector<bool> ().swap(bits_stream); //!memory free
		std::vector<char> ().swap(bytes);
		delete [] out_buf;
	}
private:
	int create_huff_table(std::vector<char> &contents, std::map<char,uint32_t> &weights,
    		std::map<char,std::vector<bool> > &table)
	{
		huff_tree out;
		std::vector<uint32_t>count;
		count.resize(CODE_WIDTH);

		//!frequency
		for(std::vector<char>::iterator it = contents.begin(); it != contents.end(); ++it)
		{
			if(*it > CODE_WIDTH-1)
			{
				std::cout<<"check char:"<<*it<<" more than"<< CODE_WIDTH-1<<" in ASCII"<<std::endl;
				return -1;
			}
			count[*it]++;	
		}
		for(int i = 0; i < count.size(); ++i)
		{
			if(count[i] != 0) 	//count[i] is the weight, i the char
			{
				weights[char(i)] = count[i]; //key--char, val--weight
			}
		}
		std::vector<uint32_t>().swap(count);
	
		//!create huff tree
		create_huff_tree(weights, out);
#if DEBUG_ON
		out.postorder(out.root); 
#endif	
		//!create map
      	out.create_map(table);

		return 0;
	}

	//!merge two huff_tree,b is the result
	void merge_huff_tree(huff_tree & a, huff_tree &b)
	{
		b.merge_tree(a);	
	}
	//!create huffman tree, input--cnt, output--weights,out	
	void create_huff_tree(std::map<char,uint32_t> &weights, huff_tree & out)
	{
		
		std::vector<std::unique_ptr<huff_tree> > trees; //using unique_ptr, for std::sort
		for(size_t i = 0; i < weights.size(); ++i)
		{
			trees.emplace_back(new huff_tree);
		}
		size_t i = 0;
		for(std::map<char,uint32_t>::iterator it = weights.begin(); it != weights.end(); ++it)
		{
			trees[i]->create(it->first, it->second);
			i++;
		} 		
		std::sort(trees.begin(), trees.end(), comp_huff_tree); //!descending order

		while(trees.size() >= 2)
		{
			size_t l = trees.size();
			merge_huff_tree(*trees[l-1], *trees[l-2]);//merge trees[l-1] + trees[l-2] 
			trees.pop_back();
			std::sort(trees.begin(), trees.end(), comp_huff_tree); //sort
		}
		out.destroy_tree();
		merge_huff_tree(*trees[0], out);

		std::vector<std::unique_ptr<huff_tree> >().swap(trees);
	}
private:
	const char FIX[3] ={ char(224),char(225),char(227)};

	inline static bool comp_huff_tree(const std::unique_ptr<huff_tree>  &l, const std::unique_ptr<huff_tree> &r) 
	{
		if(l->root->weight == r->root->weight)
		{
				if(l->height() == r->height() && l->height() == 1)
				{
					return l->root->data < r->root->data;
				}
				else
				{
					return l->height() > r->height();
				}
		}
		else
		{
			return l->root->weight > r->root->weight;
		}
	}

};




#endif
