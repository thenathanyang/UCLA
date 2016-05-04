#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int countRuns(const string a[], int n);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int split(string a[], int n, string splitter);


int main()
{
	return 0;
}


int appendToAll(string a[], int n, string value)
{
	if(n < 0)			// Returns -1 if the specified number (n) of elements in the array is negative
		return -1;
	for(int k = 0; k < n; k++)		// Appends value to value to each element in a[]
	{
		a[k] += value;
	}
	return n;
}


int lookup(const string a[], int n, string target)
{
	for(int k = 0; k < n; k++)
	{
		if(a[k] == target)			// Returns the smallest position number of an element that is equal to target
			return k;
	}
	return -1;		// Returns -1 if there is no element equal to target and/or n is negative
}

int positionOfMax(const string a[], int n)
{
	for(int k = 0; k < n; k++)
	{
		for (int i = 0; i < n; i++)
		{
			if(a[k] < a[i])		// Moves onto next number if a number is greater than the number being compared to is found
				break;
			if(i == n-1)		// If there is no other number greater than the number being compared to, retur the position of that number		
				return k;
		}
	}
	return -1;		// Returns -1 if the array has no elements
}


int rotateLeft(string a[], int n, int pos)
{
	if(n < 0 || pos < 0)
		return -1;
	string copy = a[pos];		// Makes a copy of the element that is to be moved
	for(int k = pos; k < n - 1; k++)
	{
		a[k] = a[k + 1];		// Moves all the elements right of specified position one position to the left
	}
	a[n-1] = copy;		// Puts the copied element intoo the position of the array

	return pos;		// Returns the original position of the element that was moved
}


int countRuns(const string a[], int n)
{
	if(n < 0)
		return -1;
	int consecSeq = 0;
	for(int k = 0; k < n; k++)
	{
		if(a[k] != a[k+1])		// If an element and the next element are not identical, increase the count of consecutive sequences by 1
			consecSeq++;;
	}
	return consecSeq;		// Returns number of sequences with consecutive identical elements
}

int flip(string a[], int n)
{
	if(n < 0)
		return -1;
	for(int k = 0; k < n/2; k++)
	{							// Swaps two elements from opposite sides of an array
		string copy = a[k];		// Makes a copy of an element in the first half of the array
		a[k] = a[n - 1 - k];	// Sets that element to an element from the second half of the array
		a[n - 1 - k] = copy;	// Sets the element from the second half of the array to the copy of the element from the first half of the array
	}

	return n;
}

int differ(const string a1[], int n1, const string a2[], int n2)
{
	int n = n1;
	if(n2 < n1) n = n2;			// Compare using array with smallest number of elements
	for(int k = 0; k < n; k++) 
	{
		if(a1[k] != a2[k])		// When the elements in both arrays do not equal anymore, return the position of the first element that is different
			return k;
	}
	return -1;		// Returns -1 if n1 or n2 are negative
}

int subsequence(const string a1[], int n1, const string a2[], int n2)
{
	if (n2 > n1 || n1 < 0 || n2 < 0)
		return -1;
	int pos = 0;
	for(int k = 0; k < n1; k++)		// Find where first element in a2 appears in a1
	{
		if(a2[0] == a1[k])
		{
			pos = k;
			break;
		}

	}
	for (int i = 0; i < n2; i++)			// Checks to make sure all elements in a2 are in a1
	{
		if (a2[i] != a1[pos + i])		// Return -1 if a1 does not contain a2
			return -1;
	}
	return pos;		// Return the position in a1 where a2 begins
}

int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
	for(int k = 0; k < n1; k++)			// See if an element in a1 is in a2. If so, return that position
	{
		for(int i = 0; i < n2; i++)
			if(a1[k] == a2[i])
				return k;
	}
	return -1;							// Return -1 if no element of a1 is in a2
}

int split(string a[], int n, string splitter)
{
	if(n < 0)
		return -1;

	for(int k = 0; k < n; k++)		// Sort the array alphabetically
	{
		int j = k;
		for(int i = k; i < n; i++)
			if(a[i] < a[j])
				j = i;

		string copy = a[j];
		a[j] = a[k];
		a[k] = copy;
	}

	for(int h = 0; h < n; h++)		// Return the position of the first element that, after the rearrangement, is not less than splitter
	{
		if(a[h] >= splitter)
			return h;
	}
	
	return n;		// Return n if there are no elements that, after the rearrangement, are not less than splitter
}

