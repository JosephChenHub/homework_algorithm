#include<iostream>
#include<algorithm>
#include<cmath>
#include<ctime>

#define NO_DISTANCE  9999999

struct point{
float x;
float y;
};

class Solution {
public:
	Solution(){
		m_distance = 0;
		size = 0;
		points = NULL;
	}
	Solution(int len) {
		m_distance = 0;
		size = len;
		points = new point[size];		
	}
	Solution(point * i_points, int len) {
		m_distance = 0;
		size = len;
		points = new point[size];
	 	for(int i = 0; i < size; ++i)
		{
			points[i] = i_points[i];

		}	
	}
	~Solution()
	{
		if(size != 0)
		{
			delete []points;
		}
	}
	float closest_pair()
	{
		rearrange(points,size);
		std::cout<<"starting search"<<std::endl;
		std::cout<<size<<std::endl;
		m_distance = mini_dist(points,size,m_A,m_B);
		std::cout<<"minimum distance :"<<m_distance<<std::endl;
		std::cout<<"closest pair points:  ("<<m_A.x<<","<<m_A.y \
			<<"),("<<m_B.x<<","<<m_B.y<<")."<<std::endl;
	}

	void set_points(int len)
	{
		if(size != 0)
		{
			size = 0;
			delete []points;
		}
		size = len;
		points = new point[len];
		srand(unsigned(time(NULL))); //random
		for(int i = 0; i < len; ++i)
		{
			points[i].x = (rand()%20000)/100.0-100;
			points[i].y = (rand()%20000)/100.0-100;
		}
		std::cout<<"randomly created "<<len<<" points!"<<std::endl;	
	}
	float mini_distance() const
	{
		return m_distance;
	}
	void print_points()
	{
		if(size == 0)
		{	
			std::cout<<"No points!"<<std::endl;
		}
		for(int i = 0; i < size; ++i)
		{
			std::cout<<"("<<points[i].x<<","	\
					<<points[i].y<<")"<<std::endl;
		}
	}
private:
	int size;
	point *points;
	point m_A;
	point m_B;
	float m_distance;

	float distance(point a, point b)
	{
		return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
	}

	static int compX(point a, point b)
	{
		return a.x < b.x;
	}

	static int compY(point a, point b)
	{

		return a.y < b.y;
	}

	void rearrange(point *points,int len)
	{
		///sort by x-coordinate	
		std::sort(points,points+len,compX); //pre-sort
	}


	float mini_dist(point *points,int len,point &pA, point &pB)
	{
		if (len == 1)
		{
			return NO_DISTANCE;
		}
		if (len == 2)
		{
			pA = points[0];
			pB = points[1];
			return distance(points[0],points[1]);	

		}
		///sort by x-coordinate     O(nlogn)    
		//std::sort(points,points+len,compX); //pre-sort
		float mid = points[(len-1)/2].x;
		///two parts
		point *left = new point[len];
		point *right = new point[len];

		for(int i = 0; i < len/2; ++i)
		{
			left[i] = points[i];
		}
		for(int j = 0, i = len/2; i < len; ++i)
		{
			right[j++] = points[i];	
		} 
		point a1,b1,a2,b2;	
		float delta = 0;
		float d1 = mini_dist(left,len/2,a1,b1);
		float d2 = mini_dist(right,len-len/2,a2,b2);
		if(d1 < d2)
		{
			delta = d1;
			pA = a1;
			pB = b1;
		}
		else
		{
			delta = d2;
			pA = a2;
			pB = b2;	
		}
	
	
		///consider part between (l-delta,l+delta)
		point *part = new point[len];
		int num = 0; //length of the above part
		for(int i = 0,num = 0; i < len; ++i)
		{
			if(abs(points[i].x - mid) <= delta)
			{	
				part[num++] = points[i];
			}	
	
		}
		///sort by y-coordinate
		std::sort(part,part+num,compY);
		///scan
		for(int i = 0; i < num; ++i)
		{
			for(int j = i+1; j < num && j<= i+11 ;++j)
			{
				float temp = distance(part[i],part[j]);
				if(temp < delta) //update distance
				{
					delta = temp;
					pA = part[i];
					pB = part[j];
				}
		
			}
		}

		return delta;
	}
};


int main()
{

	Solution test;
	test.set_points(21); //randomly create 21 points	
	test.closest_pair(); 
	std::cout<<"original points:"<<std::endl;
	test.print_points();

	return 0;
}
