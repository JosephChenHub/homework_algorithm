#include <iostream>
#include <vector>
#include <algorithm>


std::vector<int> opt(int * src,int n) 
{

	std::vector<int> *result = new std::vector<int>;	
	result->clear();
	result->reserve(n);
	
	//!sorted array
	std::vector<int> arr(n);
	for(int i = 0; i < n; ++i)
	{
		arr[i] = src[i];
	}
	std::sort(arr.begin(),arr.end());

	if(n == 1)
	{
		result->push_back(arr[0]);
		return *result;
	}

	int *len =new int[n];
	int *pre = new int[n];
	int *Len = new int[n];
	for(int i = 0; i < n;++i)
	{
		len[i] = 1;
		pre[i] = 1;
	}
	Len[0] =1;

	for(int i = 1; i < n; ++i)
	{
			
		for(int j = 0; j < i; ++j)
		{
			if(arr[i] % arr[j] ==0)
			{
				Len[i] = Len[j]+1 > Len[i-1] ? Len[j]+1:Len[i-1];				
			}
			if(arr[i] % arr[j] == 0 && len[i] < len[j]+1) //0<=j<i,    
			{
				len[i] = len[j]+1;
				pre[i] = j;
			}
		}

	}
	int max_length = 0,max_index = 0;
	for(int i = 0; i < n; ++i)
	{	
		if(len[i] > max_length)
		{
			max_length = len[i];
			max_index = i;
		}
	}
	int *index = new int[max_length];
	int temp = max_index;
	index[max_length-1] = max_index;

	for(int i = max_length-2; temp >=0 && i >=0; --i )
	{
		index[i] = pre[temp];
		temp = index[i];
	}
	
	for(int i = 0; i < max_length;++i)
	{
		result->push_back(arr[index[i]]);

	}

	delete []len;
	delete []pre;
	delete []index;

	return *result;
}






int main(int argc, char * argv[])
{
		int a[] = {1,4,5,8,10,20,21,42};
		int size = sizeof(a)/sizeof(int);
	
		std::vector<int>result;
		result = opt(a,size);

		std::cout<<"original set:";
		for(int i = 0; i < size; ++i)
		{
			std::cout<<a[i]<< " ";
		}
		std::cout<<std::endl;

		std::cout<<"result:";
		for(int i = 0; i < result.size(); ++i)
		{
			std::cout<<result[i]<<" ";
		}
		std::cout<<std::endl;

		return 0;
}
