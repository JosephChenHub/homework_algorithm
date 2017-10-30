#include <iostream>
#include <vector>

using namespace std;


int rob(int * arr,int n)
{
	if(n == 0)
	{
		return 0;
	}
	else if(n == 1)
	{
		return arr[0];
	}
	else if(n == 2)
	{
		return arr[0] > arr[1]?arr[0]:arr[1];
	}
	int *sum = new int[n];
	sum[0] = arr[0];
	sum[1] = arr[0] > arr[1]? arr[0]:arr[1];


	for(int i = 2; i < n; ++i)
	{
		sum[i] = sum[i-1]> (sum[i-2] + arr[i]) ? sum[i-1]:(sum[i-2] + arr[i]);
	}
	int result = sum[n-1];
	delete [] sum;

	return result;


}





int main(int argc, char * argv[])
{
		int a[] = { 30,24,20,500,67,8};
		int size = sizeof(a)/sizeof(int);


		int money = rob(a,size);

		int total = 0;
		cout<<"houses:";
		for(int i = 0; i < size; ++i)
		{
			total += a[i];
			cout<<a[i]<<" ";
		}
		cout<<endl;
		

		cout<<"total money:"<<total<<endl;
		cout<<"you can rob:"<<money<<endl;
		

		return 0;
}
