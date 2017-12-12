#include <iostream>
#include <utility>

//!greedy 

int main(int argc, char * argv[])
{

//	int c[] = {1,5,10,25};
	int c[] = {1,10,21,34,70,100,350,1225,1500};
	


	int s[9] = {0};

	int x = 140;
	int raw = x;
	int k = 8;
	while(x > 0)
	{
		if(k < 0)
		{
			std::cout<<"no solution!"<<std::endl;	
			return -1;
		}
		if(c[k] <= x)
		{
			x = x- c[k];
			s[k]++;
		}
		else
		{
			k--;
		}

	}


	return 0;

}
