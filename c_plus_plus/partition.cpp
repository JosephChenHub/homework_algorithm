#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int COST = 1;

int s_cost(string & arr)
{
	int cost = 0;
	int size = arr.size();
	for(int i = 0;i < size/2; ++i)
	{
		if(arr[i] != arr[size-1-i])
		{
			cost += 1;	
		}
	}
	return cost;
}

int cuts(string &s)
{
	if(s.size() == 1)
	{
		return 0;
	}
	int size = s.size();
	int *nums = new int[size];
	nums[0] = 0;
	for(int i = 1; i < s.size(); ++i)
	{
		if(s[s.size()-1] != s[0])
		{
			nums[i] = nums[i-1] + 1;
			s.pop_back();
		}

	}

	return nums[size-1];
}


int main(int argc,char * argv[])
{

	string b = "deabcdcbaf";	
	int size = b.size();
	cout<<"original:"<<b<<endl;
	cout<<"num:"<<cuts(b)<<endl;

	return 0;
}
