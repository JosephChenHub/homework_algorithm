#ifndef __FILE_PROCESS_H
#define __FILE_PROCESS_H

#include <iostream>
#include <fstream>
#include <algorithm> //use sort
#include <map>  //use map
#include <vector>
#include <memory>
#include <iomanip>

#define DEBUG_ON 0 //print some info.
typedef char USE_TYPE;
const int CODE_WIDTH = 128;


//! node with weight
struct node{
	USE_TYPE data;
	size_t weight;
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
	huff_tree(USE_TYPE ch, size_t w)
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
	void create(USE_TYPE ch, size_t w)
	{
		destroy_tree(root);
		root = new node;
		root->left = NULL;
		root->right = NULL;
		root->data = ch;
		root->weight = w; 
		_height = 1; 
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
	int root_weight()const
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
        	std::cout<< p->data<<","<<p->weight << "\n ";
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
		create_map(data, root, bits);

	}
	void create_map(char data,node *s, std::vector<bool> &bits)
	{
		if(s == NULL)
		{
			return ;
		}
		int d = depth(s);
		if(s->left) //not NULL
		{
			bits[d] = 0; //!left edge = 0
			if(s->left->data == data)
			{
					return ;
			}
			create_map(data,s->left,  bits);
		}
		if(s->right)
		{
			bits[d] = 1;
			if(s->right->data == data)
			{
				return;
			}
			create_map(data,s->right,bits);
		}
		
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
		if(s->data == data )//&& s->left == NULL && s->right == NULL) //!no child
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
		new_root->data = 0;
		new_root->weight = src->weight + dst->weight;
		new_root->left = NULL;
		new_root->right = NULL;
		merge_tree(src, new_root->left);
		merge_tree(dst, new_root->right);
		
		destroy_tree(dst);

		dst = new node;
		dst = new_root;
	}



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
		count = NULL;
	}
	void compress(std::string &file_name)
	{


	}
	void decompress(std::string &file_name)
	{


	}
	int create_huff_table(std::string &file_name, huff_tree &out)
	{
		std::ifstream file;
		file.open(file_name, std::ios::in);
		if(!file.is_open())
		{
			std::cout<<"open file failed: "<<file_name<<"!"<<std::endl;			
			return -1;
		}
		USE_TYPE ch;
		while(! file.eof())
		{
			file>>ch;
			//!count
			count[ch] ++;		
		}
		file.close();
		//!
		create_huff_tree(count, CODE_WIDTH, out);
				
	}
	void read_huff_table(std::string &file_name)
	{

	}
	//!merge two huff_tree,b is the result
	void merge_huff_tree(huff_tree & a, huff_tree &b)
	{
		b.merge_tree(a);	
	}
	
	void create_huff_tree(size_t *cnt, int len, huff_tree & out)
	{
		for(int i = 0; i < len && len <= CODE_WIDTH ; ++i)
		{
			if(cnt[i] != 0) //cnt[i] is the weight, i the USE_TYPE
			{
				weights[USE_TYPE(i)] = cnt[i]; //key--USE_TYPE, val--weight
			}
		}
		std::vector<std::unique_ptr<huff_tree> > trees; //using unique_ptr, for std::sort
		for(int i = 0; i < weights.size(); ++i)
		{
			trees.emplace_back(new huff_tree);
		}
		int i = 0;
		for(std::map<USE_TYPE,size_t>::iterator it = weights.begin(); it != weights.end(); ++it)
		{
			
			trees[i++]->create(it->first, it->second);
		} 		
		
#if DEBUG_ON
		for(int i = 0; i < trees.size(); ++i)
		{
			std::cout<<"tree: trees["<<i<<"] root_addr:"<<trees[i]->root<<" data:"<< trees[i]->root->data<<" weight:"<<trees[i]->root->weight<<std::endl;
		}
		std::cout<<"after sort"<<std::endl;
#endif 
		std::sort(trees.begin(), trees.end(), comp_huff_tree); //!descending order
#if DEBUG_ON
		for(int i = 0; i < trees.size(); ++i)
		{
			std::cout<<"tree: trees["<<i<<"] root_addr:"<<trees[i]->root<<" data:"<< trees[i]->root->data<<" weight:"<<trees[i]->root->weight<<std::endl;

		}
#endif
		while(trees.size() >= 2)
		{
			merge_huff_tree(*trees[trees.size()-1], *trees[trees.size()-2]);
			trees.pop_back();
			std::sort(trees.begin(), trees.end(), comp_huff_tree); //sort
		}
		out.destroy_tree();
		merge_huff_tree(*trees[0], out);
	}
private:
	size_t *count;
	std::map<USE_TYPE, size_t> weights;

	inline static bool comp_huff_tree(const std::unique_ptr<huff_tree>  &l, const std::unique_ptr<huff_tree> &r) 
	{
		if(l->root->weight == r->root->weight)
		{
			return l->height() > r->height();
		}
		return l->root->weight > r->root->weight;
	}

};




#endif
