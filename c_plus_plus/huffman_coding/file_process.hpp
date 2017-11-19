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
	struct node * left;
	struct node * right;
};
//! tree
class tree{
public:
	tree()
	{
		root = NULL;
		index_root = 0;
	}
	tree(USE_TYPE ch, size_t w)
	{
		create(ch,w);
	}
	~tree()
	{
		std::cout<<"destroy obj:"<<this<<" root:"<<root<<std::endl;
		destroy(root);
	}
	
	void create(USE_TYPE ch, size_t w)
	{
		destroy(root);
		root = new node;
		root->left = NULL;
		root->right = NULL;
		root->data = ch;
		root->weight = w;  
	}
	//! free memory 
	void destroy(node *s)
	{
		if(s == NULL)
		{
			return;
		}
		destroy(s->left);
		s->left = NULL;	
		destroy(s->right);
		s->right = NULL;
		
		delete s;	
		s = NULL;
	}
	//!depth of node
	int depth(node *s)
	{
		return depth(s, root, 0);
	}
	int depth(node *s, node *r, int d = 0) 
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
	//!height of the tree
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
	//!merge two tree,b is out
	void merge_tree(tree &a , tree &b)
	{
		if(a.root == NULL)
		{
			return ;
		}
		else if(b.root == NULL)
		{
			b.root = new node;
			b.root = a.root;
			return;
		}
        node *new_root = new node;

		new_root->left = a.root;
		new_root->right = b.root;
		new_root->data = 0;
		new_root->weight = a.root_weight() + b.root_weight();

		b.root = new_root;
	}
	int create_tree(size_t *cnt, int len, tree & out)
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
		std::vector<tree> m;
		for(std::map<USE_TYPE,size_t>::iterator it = weights.begin(); it != weights.end(); ++it)
		{
			
//			m.push_back(tree(it->first, it->second));
		} 		

//		std::sort(m.begin(), m.end(), comp_tree_weight);
		tree m_out; //



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
	static bool comp_by_val(const std::pair<USE_TYPE,size_t> & l , const std::pair<USE_TYPE,size_t>& r)
	{
		return l.second < r.second;
	}		
	static bool comp_tree_weight(const tree & l,const tree &r)
	{
		return l.root->weight < r.root->weight;
	}

};




#endif
