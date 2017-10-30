#include <iostream>
#include <vector>
#include <string>

//! define: OPT(i,j) = min cost of aligning 
// prefix string {x1,x2,...,xi} and {y1,y2,...,yj}
//! gap penalty: g_cost ,mismatch penalty: m_cost

const int g_cost = 5;
const int M_COST = 1;

//! minimum
template<class T>
 T min(T a,T b,T c)
{
	return  (a<b?a:b) < c ? (a<b?a:b) :c;
}

//! penalty for mismatch
template<class T>
int  m_cost(T &a, T &b)
{
	int cost = 0;	
	if(a == b)
	{
		cost = 0;
	}
	else
	{
		cost = M_COST;
	}
	return cost;
}


int  opt(std::string &x,std::string &y,std::string &out)
{
	int m = x.size();
	int n = y.size();
	//! create 
	int ** M = new int*[m+1];
	for(int i = 0; i <= m; ++i)
	{
		M[i] = new int[n+1];
	}

	//!
	for(int i = 0; i <= m;++i)
	{
		M[i][0] = i*g_cost;
	}
	for(int j = 0; j <= n; ++j)
	{	
		M[0][j] = j*g_cost;
	}

	//!DP
	for(int i = 1; i <= m; ++i)
	{
		for(int j = 1; j <= n; ++j)
		{
			int m = m_cost(x[i-1],y[j-1]);	
			M[i][j] = min(m + M[i-1][j-1], g_cost + M[i-1][j], g_cost + M[i][j-1]);	

		}
	}

	int result = M[m][n];

	std::vector<char>z;

	int i = m, j = n;	
	while(i >= 1 && j>=1)
	{

			//!if x[i-1] matches y[j-1], then temp1 = M[i-1][j-1]
			//!if x[i-1] matches ' ', then temp2 = M[i-1][j]
			//!if ' '  matches y[j-1] ,then temp2 = M[i][j-1]
			//!if 


			int temp2 = M[i][j] - g_cost;
			int temp1 = 0;
			temp1 = M[i][j] - m_cost(x[i-1],y[j-1]); //must i,j>=1
		
			if(temp1 == M[i-1][j-1])
			{
				z.push_back(y[j-1]);
				i --;
				j --;
			}
			else if(temp2 == M[i-1][j]) 
			{
//				z.push_back(' ');
				i--;
			}
			else if(temp2 == M[i][j-1])
			{
				z.push_back(y[j-1]);
				j--;
			}
			else //! it is impossible 
			{
				std::cout<<"check!"<<std::endl;
				return -1;
			}
		
	}
	while(i == 0 && j >= 1)
	{
		z.push_back(y[j-1]);
		j --;
	}


	out.clear();
	for(size_t i = 0;i < z.size(); ++i)
	{	
	
		out.push_back(z[z.size() - 1 - i ]);
	}


	//!free
	for(int i = 0; i <= m; ++i)
	{
		delete [] M[i];
	}
	delete []M;

	return result;
}


int main(int argc, char * argv[])
{

	std::string a =  "RRANTFFT";
	std::string b = "CURRENT";

	std::string c = "";

	int cost = opt(a,b,c);

	std::cout<<"x: "<<a<<std::endl;
	std::cout<<"y: "<<b<<std::endl;

	std::cout<<"cost:"<<cost<<std::endl;
    std::cout<<"recover:"<<c<<std::endl;



	return 0;
}
