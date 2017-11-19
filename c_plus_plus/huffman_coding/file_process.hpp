#ifndef __FILE_PROCESS_H
#define __FILE_PROCESS_H

#include <iostream>
#include <fstream>
#include <algorithm> //use sort
#include <map>  //use map
#include <vector>
#include <memory> 

#define  USE_TYPE char
using namespace std;

//! node with weight
struct node{
	char data;
	size_t weight;
	struct shared_ptr<node> left;
	struct shared_ptr<node>  right;
};


//! huff_tree
class huff_tree{
public:
	huff_tree()
	{
	}
	huff_tree(USE_TYPE ch, size_t w)
	{
		create(ch,w);
	}
	~huff_tree()
	{
//		std::cout<<"destroy obj:"<<this<<" root:"<<root<<std::endl;
		destroy(root);
	}
/*
	huff_tree & operator=(huff_tree &other)
	{
		if(this != &other)
		{
			destroy(root);
			root = make_shared<node>();
			root = other.root;
		}
		return *this;
	}
*/	
	void create(USE_TYPE ch, size_t w)
	{
		destroy(root);
		root = make_shared<node>();
		root->left = NULL;
		root->right = NULL;
		root->data = ch;
		root->weight = w;  
	}
	//! free memory 
	void destroy(shared_ptr<node> &s)
	{
		if(s == NULL)
		{
			return;
		}
		destroy(s->left);
		s->left = NULL;	
		destroy(s->right);
		s->right = NULL;
		
		s.reset();
		s = NULL;
	}
	//!depth of node
	int depth(shared_ptr<node> & s)
	{
	   depth(s,root,0);
	}
	int depth(shared_ptr<node> & s, shared_ptr<node> &r, int d = 0) 
	{
		if(r == NULL)
		{
			return 0;
		}			
		if(r == s)
		{
			return d;
		}		
		int temp1 = depth(s, r->left, d+1);
 
		int temp2 = depth(s, r->right,d+1);		
			
		return temp1>temp2?temp1:temp2;
	}
	//!height of a node
	int height(shared_ptr<node> &s)
	{
		if(s == NULL)
		{
			return 0;
		}	
			
		int temp1 = height(s->left) + 1;
		int temp2 = height(s->right) + 1;
		
		return (temp1 > temp2? temp1:temp2);
	}
	//!height of the huff_tree
	int height() 
	{
		return  height(root);
	}
	size_t root_weight() const
	{
		return root->weight;
	}
public:
	shared_ptr<node>  root;//!prevent deleting twice
private:

};


//!Huffman encoding and decoding
class Huffman{
public: 
	Huffman()
	{
		count = new size_t[CODE_WIDTH];
	}
	~Huffman()
	{
		delete count;

	}
	void compress(std::string &file_name)
	{


	}
	void decompress(std::string &file_name)
	{


	}
	int createTable(std::string &file_name)
	{
		std::ifstream file;
		file.open(file_name, std::ios::in);
		if(!file.is_open())
		{
			std::cout<<"open file failed: "<<file_name<<"!"<<std::endl;			
			return -1;
		}
		char ch;
		while(! file.eof())
		{
			file>>ch;
			//!count
			count[ch] ++;		
		}
		file.close();
		//!
		
	}
	void readTable(std::string &file_name)
	{

	}
	//!merge two huff_tree,b is out
	void merge_huff_tree(huff_tree &a , huff_tree &b)
	{
		if(a.root == NULL)
		{
			return ;
		}
		else if(b.root == NULL)
		{
			b.root = make_shared<node>();
			b.root = a.root;
			return;
		}
        shared_ptr<node> new_root = make_shared<node>();
		new_root->left = make_shared<node>();
		new_root->right = make_shared<node>();

		new_root->left = a.root;
		new_root->right = b.root;
		new_root->data = 0;
		new_root->weight = a.root_weight() + b.root_weight();

		b.root = new_root;
	}
	
	int create_huff_tree(size_t *cnt, int len, huff_tree & out)
	{
		
		for(int i = 0; i < len && len <= CODE_WIDTH ; ++i)
		{
			if(cnt[i] != 0) //cnt[i] is the weight, i the char
			{
				weights[USE_TYPE(i)] = cnt[i]; //key--char, val--weight
			}

		}
		//!sort by value
//		sort_map(weights);	
		std::vector<huff_tree> m;
		m.resize(weights.size());
		int i = 0;
		for(std::map<USE_TYPE,size_t>::iterator it = weights.begin(); it != weights.end(); ++it)
		{
			
			m[i++].create(it->first, it->second);
		} 		

		std::sort(m.begin(), m.end(),comp_huff_tree); //!descending order
		size_t l = m.size();
		while(l >= 2)
		{
			merge_huff_tree(m[l-1], m[l-2]);
			//m.pop_back(); //remove the minimum
			std::sort(m.begin(), m.begin() + l, comp_huff_tree); //sort
			l --; 
		}
		out = m[0];
		int j = 0;

	}
	void sort_map(std::map<USE_TYPE,size_t> & w)
	{
		vec_weights.clear();
		vec_weights.resize(w.size());
		int i = 0;
		for(std::map<USE_TYPE,size_t>::iterator it = w.begin(); it != w.end(); ++it)
		{
			vec_weights[i++] = std::make_pair(it->first, it->second);
		}

		std::sort(vec_weights.begin(), vec_weights.end(),comp_by_val);		
		
	}
private:
	size_t *count;
	const int CODE_WIDTH = 128;
	std::map<USE_TYPE, size_t> weights;
	std::vector<std::pair<USE_TYPE,size_t> > vec_weights;

	inline static bool comp_by_val(const std::pair<USE_TYPE,size_t> & l , const std::pair<USE_TYPE,size_t>& r)
	{
		return l.second < r.second;
	}		
	inline static bool comp_huff_tree(const huff_tree  &l,const huff_tree &r)
	{
		return l.root->weight > r.root->weight;
	}

};




#endif
