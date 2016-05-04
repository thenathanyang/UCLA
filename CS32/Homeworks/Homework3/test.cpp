// Example program
#include <iostream>
using namespace std;


int s[4] = {1,5,10,25};

int count(int n, int m)   //n is the total change
{
	if(n < 0 || m < 0)
		return 0;
	if(n == 0)			//there is one way of making 0 dollars
		return 1;
	return count(n-s[m], m) + count(n, m-1);
}

int main()
{
    cout << count(5,3) << endl;
}

// 15,3
// 0	15,2
// 	5,2		15,1
// 	0		10,1	15,0
// 			5,1		14,0
// 			1		13,0
// 					12,0
// 					11,0
// 					10,0
// 					9
// 					8
// 					7
// 					6
// 					5
// 					4
// 					2
// 					1
// 					0



5,3
0	5,2
	0	5,1
		1	5,0		5,-1
			4,0
			3,0
			2,0
			1,0
			0,0 (1)