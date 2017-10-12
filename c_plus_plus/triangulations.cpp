#include <iostream>
#include <sys/time.h> 


size_t count_tri(size_t n)
{
	if(n < 2)
	{
		return 0;
	}
	else if(n == 2) //def count_tri(2)=1
	{
		return 1;
	}
	else if(n == 3) 
	{
		return 1;
	}
	//O(n^2)	
	size_t *temp = new size_t[n+1];
	temp[2] = 1;
	temp[3] = 1;
	for(size_t i = 4; i <= n; ++i)
	{
		for(size_t j = 2; j <= i-1; ++j)
		{
			temp[i] += temp[j]*temp[i+1 - j];
		}
	}
	return temp[n];
}


int main(int argc, char * argv[])
{
	struct timeval start,end;
	
	for(size_t i = 3; i < 20; ++i)
	{
		gettimeofday(&start,NULL);
		size_t result = count_tri(i);
		gettimeofday(&end,NULL);
		double cost_time = (end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec);
		std::cout<<"i:"<<i<<" numbers:"<<result<<" cost_time:"<<cost_time<<" us"<<std::endl;
	}


	return 0;
}
