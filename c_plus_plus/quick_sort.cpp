#include <iostream>
#include <sys/time.h>


template<class T>
void quick_sort(T * arr,const size_t len, T * &out)
{
	if(len == 0)
	{
		return ;
	}
	if(len == 1 )
	{
		out[0] = arr[0];
		return ;
	}
	if(out == NULL)
	{
		out = new T[len];
	}

	size_t pos = len/2;
	T mid = arr[pos]; 
	T *left = new T[len];
	T *right = new T[len];

	size_t m = 0, n = 0;
	for(size_t i = 0; i < len ; ++i)
	{
		if(i == pos)
		{
			continue;
		}
		if(arr[i] < mid) 
		{
			left[m++] = arr[i];
		}
		else
		{
			right[n++] = arr[i];
		}
	}
	
	quick_sort(left, m, left);
	quick_sort(right,n, right);
	//!merge L,mid,R	
	for(size_t i = 0; i < len; ++i)
	{
		 if(i < m)
		{
			out[i] = left[i];
		}
		else if(i == m ) 
		{
			out[i] = mid;
		}
		else 
		{

			out[i] = right[i-m-1];
		}
	}
}



int main(int argc, char * argv[])
{

	
	const size_t size = 100000;
	int *a = new int[size];
	for(size_t i = 0; i < size; ++i)
	{
		a[i] = size - i;
	}
	int *out = new int[size];

	
	std::cout<<std::endl;
        struct timeval start,end;
	gettimeofday(&start,NULL);	

	quick_sort(a,size,out);
		
	gettimeofday(&end,NULL);
	double cost_time = (end.tv_sec - start.tv_sec)*1000 +
                                 (end.tv_usec - start.tv_usec)*0.001; //ms  
	std::cout<<"It took :"<<cost_time<<" milliseconds."<<std::endl;


	return 0;
}
