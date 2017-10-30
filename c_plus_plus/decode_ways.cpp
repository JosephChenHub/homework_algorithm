#include <iostream>

using namespace std;


int numDecodings(string &s)
{
        int size = s.size();
        
        if(size == 0)
        {
            return 0;
        }
        else if(size == 1)
        {
            if(s[0] == '0')
            {
                return 0;
             } 
            return 1;
        }
        if(s[0] == '0')
        {
            return 0;
        }       
        int *opt = new int [size+1];
        opt[0] = 1;
        opt[1] = 1;
        for(int i = 1;i < size; ++i)
        {
            if(stoi(s.substr(i-1,2)) > 26 || s[i-1] == '0')
            {
                if(s[i] == '0')
                {
                    return 0;
                }
                opt[i+1] = opt[i];
            }
            else  
            {
                if(s[i] == '0')
                {
                    opt[i+1] = opt[i-1];
                }
                else
                {
                    opt[i+1] = opt[i] + opt[i-1];
                }

            }
            
        }
        int result = opt[size];
        delete [] opt;
        
        return result;
}




int main(int argc, char * argv[])
{


		string a = "1224";
		cout<<"numbers:"<<numDecodings(a)<<endl;

		return 0;


}
