#include <iostream>
#include <string>
#include <vector>

// Dynamic Programming Solution for Palindrome Partitioning Problem
#include <stdio.h>
#include <string.h>
#include <limits.h>

int min (int a, int b) { return (a < b)? a : b; }

// Returns the minimum number of cuts needed to partition a string
// such that every part is a palindrome
int minPalPartion(char *str)
{
	// Get the length of the string
	int n = strlen(str);

	/* Create two arrays to build the solution in bottom up manner
	C[i] = Minimum number of cuts needed for palindrome partitioning
				of substring str[0..i]
	P[i][j] = true if substring str[i..j] is palindrome, else false
	Note that C[i] is 0 if P[0][i] is true */
	int C[n];
	bool P[n][n];

	// Every substring of length 1 is a palindrome
	for (int i=0; i<n; i++)
	{
		P[i][i] = true;
		C[i] = 0;
	}

	/* L is substring length. Build the solution in bottom up manner by
	considering all substrings of length starting from 2 to n. */
	for (int L = 2; L <= n; L++) //! L from 2 to n
	{
		for (int i = 0; i < n-L+1; i++) //! i --start index
		{
			int j = i+L-1; //ending index

			// If L is 2, then we just need to compare two characters. Else
			// need to check two corner characters and value of P[i+1][j-1]
			if (L == 2)
			{
				P[i][j] = (str[i] == str[j]);
			}
			else
			{
				P[i][j] = (str[i] == str[j]) && P[i+1][j-1];
			}
		}
	}

	for (int i = 0; i < n; i++)
	{
		if (P[0][i] == true) //s[0...i] is palindrome
			C[i] = 0;
		else
		{
			C[i] = INT_MAX;
			for(int j = 0; j <= i-1; j++) //j from 0 to i-1
			{
				if(P[j+1][i] == true && C[i] > 1+C[j])
					C[i] = 1 + C[j];
			}
		}
	}

	// Return the min cut value for complete string. i.e., str[0..n-1]
	return C[n-1];
}

// Driver program to test above function
int main()
{
	char str[] = "ababbbabbababa";
	std::cout<<"Min cuts needed for Palindrome Partitioning is " <<
		minPalPartion(str)<<std::endl;

	return 0;
}

