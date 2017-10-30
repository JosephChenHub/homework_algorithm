#include <iostream>

using namespace std;


int max_length(int * nums,int size)
{

	if(size == 0)
	{
		return 0;
	}
	else if(size == 1)
	{
		return 1;
	}

	int * opt = new int[size];
	for(int i = 0; i < size; ++i)
	{
		opt[i] = 1;
	}

	int end = nums[0]; //maximum of the optimal subset S


	for(int i = 1;i < size; ++i)
	{
		if(nums[i] > end) //add nums[i] to the subset S,update end of S
		{
			opt[i] = opt[i-1] + 1;
			end = nums[i];
		}
		else //nums[i] <= end
		{
			//!find the subset S1 ended with nums[i]	
			int temp = nums[i];		
			for(int j = i-1; j >= 0; --j)
			{
				if(nums[j] < temp) //len(S1)+1
				{
					temp = nums[j];
					opt[i] ++;
				}
			}
			if(opt[i] < opt[i-1]) //len(S1) < len(S) ,S is unchanged 
			{
				opt[i]  = opt[i-1];  
			}
			else	// len(S1) >= len(S), update S, end=nums[i]
			{
				end = nums[i];
			}
		 }
	}

	int result = opt[size-1];
	delete [] opt;

	return result; 
 
}






int main(int argc, char * argv[])
{
	int a[]={3,5,6,2,5,4,19,5,6,7,1};
	int size = sizeof(a)/sizeof(int);

	int result = max_length(a,size);
	cout<<"array:";
	for(int i = 0; i < size; ++i)
	{
		cout<<a[i]<<" ";
	}
	cout<<endl;
	cout<<"length of optimal subset:"<<result<<endl;


	return 0;
}
