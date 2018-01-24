/********************************************
compile environments: Linux, GCC
command:
g++  count_inversionsc.cpp -o test
*********************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>  //Linux

const int FACTOR = 3;

/// brute force  O(n^2)
template<class T>
size_t brute_getInvCount(T arr[],size_t n)
{
	size_t cnt = 0;
	for(size_t i = 0; i < n -1; ++i)
	{
		for(size_t j = i +1; j < n; ++j)
		{
			if(arr[i] > FACTOR*arr[j])
			{
				cnt ++;
			}
		}
	}
	return cnt;
}

///A,B are sorted,return sorted list	O(n)
template<class T>
size_t merge_count(T * A, size_t len_a, T * B, size_t len_b,T * &list)
{
	if(list == NULL)
	{
		list =new T[len_a + len_b];
	}
	/*
	if(len_a == 0)
	{
		for(int i = 0; i < len_b; ++i)
		{
			list[i] = B[i];
		}
		return 0;
	}
	if(len_b == 0)
	{
		for(int i = 0; i < len_a; ++i)
		{		
			list[i] = A[i];

		}
		return 0;
	}
	*/

	size_t inv = 0;
	//count O(n)
	for(int i = 0, j = 0; i < len_b && j < len_a; )
	{
		if(A[j] > FACTOR*B[i])
		{
			inv += len_a - j;
			i ++;
		}
		else
		{
			j++;
		}
	}
	//sort  O(n)
	size_t m = 0, n = 0;
	for(size_t k = 0, i = 0; i < len_a+len_b ; ++i)
	{
		if(m >= len_a)
		{
			list[k++] = B[n++];
		}
		else if(n >= len_b)
		{
			list[k++] = A[m++];
		}
		else if(A[m] <= B[n])
		{
			list[k++] = A[m++];
		}
		else
		{
			list[k++] = B[n++];
		}
	}
		
	return inv;
}

template<class T>
size_t sort_count(T * list,size_t len,T * &out)
{

	if(out == NULL)
	{
		out = new T[len];
	}
	if( len == 1)
	{
		out =list;
		return 0;
	}	

	///divide list into two parts
	T * left = new T[len/2];
	T * right = new T[len-len/2];

	size_t r1 = sort_count(list,len/2,left);
	size_t r2 = sort_count(list+len/2,len - len/2,right);
	
	///combine
	size_t r = merge_count(left,len/2,right, len-len/2, out);


	return  r1 + r2 + r;
	
}

int main(int argc,char * argv[])
{

	int *data = new int[100000];
	size_t size = 0;

	
	std::ifstream file;
	file.open("./Q8.txt",std::ios::in);
	if(! file.is_open())
	{
		std::cout<<"Open file failed!"<<std::endl;
		return -1;

	}
	std::string s;
	while(! file.eof())
	{
		file>>data[size++];
	}
	file.close();
	size--;
	std::cout<<"read "<<size<<" numbers"<<std::endl;


	int *list = new int[100000];
	//test time
	struct timeval start,end;
	//! 23.923ms,22.338ms, 24.542ms,24.916ms   ~ 25ms
	gettimeofday(&start,NULL);
	size_t r = sort_count(data,size,list);
	gettimeofday(&end,NULL);
	std::cout<<"Merge and Count -- number of inversions:"<<r<<std::endl;
	double cost_time = (end.tv_sec - start.tv_sec)*1000 + 
				(end.tv_usec - start.tv_usec)*0.001; //ms 

	std::cout<<"It took :"<<cost_time<<" milliseconds."<<std::endl;

	//! brute force cost 20045ms,20039ms,19993ms,20019ms ~ 20s
	gettimeofday(&start,NULL);
	size_t r2 = brute_getInvCount(data,size);
	gettimeofday(&end,NULL);
	
	std::cout<<"Brute force -- number of inversions:"<<r2<<std::endl;
	double cost_time2 = (end.tv_sec - start.tv_sec)*1000 + 
				(end.tv_usec - start.tv_usec)*0.001; //ms 
	std::cout<<"It took :"<<cost_time2<<" milliseconds."<<std::endl;



	return 0;
}
