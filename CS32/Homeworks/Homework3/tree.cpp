// tree.cpp

#include <iostream>
using namespace std;

void p(const double* a, int n) {
  while (n--) {
    cout << *(a++) << ", ";
  }
  cout << endl;
}




  // Return the number of ways that all n2 elements of a2 appear
  // in the n1 element array a1 in the same order (though not
  // necessarily consecutively).  The empty sequence appears in a
  // sequence of length n1 in 1 way, even if n1 is 0.
  // For example, if a1 is the 7 element array
  //	10 50 40 20 50 40 30
  // then for this value of a2     the function must return
  //	10 20 40			1
  //	10 40 30			2
  //	20 10 40			0
  //	50 40 30			3
int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0) return 1;
    if (n1 <= 0) return 0;    // || n2 > n1


    int temp = countIncludes(a1+1, n1-1, a2, n2);
    if (a1[0] == a2[0])
    	return countIncludes(a1+1, n1-1, a2+1, n2-1) + temp;
    return temp;
}

// 10 50 40 20 50 40 30		7			50 40 30		3	(OG)
// 50 40 20 50 40 30			6			50 40 30		3
// 40 20 50 40 30				5			40 30			2																								40 20 50 40 30		5 			50 40 30		3
// 20 50 40 30					4			30				1				20 50 40 30			4			40 30 		2									20 50 40 30			4			50 40 30 		3	
// 50 40 30					3			30				1				50 40 30			3			40 30		2
// 40 30						2			30				1				40 30				2			40 30 		2			30 1		40 30	
// 30							1			30 				1				30					1			30			1
// 1																		1




// int main()
// {
//     double a1[7] = {10, 50, 40, 20, 50, 40, 30};
//     double a2[3] = {10,20,40};
//     double a21[3] = {10,40,30};
//     double a22[3] = {20,10,40};
//     double a23[3] = {50,40,30};
//     double a3[0];
//     double a4[1] = {10};
//     // cout << a2[0] << endl;
//     // cout << a3[0] << endl;
//     cout << countIncludes(a1, 7, a2, 3) << endl;   // 1
//     cout << countIncludes(a1, 7, a21, 3) << endl;   // 2
//     cout << countIncludes(a1, 7, a22, 3) << endl;   // 0
//     cout << countIncludes(a1, 7, a23, 3) << endl;   // 3
//     cout << countIncludes(a3, 0, a4, 1) << endl;   // 0
// }



  // Exchange two doubles
void exchange(double& x, double& y)
{
    double t = x;
    x = y;
    y = t;
}

  // Rearrange the elements of the array so that all the elements
  // whose value is > splitter come before all the other elements,
  // and all the elements whose value is < splitter come after all
      // the other elements.  Upon return, firstNotGreater is set to the
  // index of the first element in the rearranged array that is
  // <= splitter, or n if there is no such element, and firstLess is
  // set to the index of the first element that is < splitter, or n
  // if there is no such element.
  // In other words, upon return from the function, the array is a
  // permutation of its original value such that
  //   * for 0 <= i < firstNotGreater, a[i] > splitter
  //   * for firstNotGreater <= i < firstLess, a[i] == splitter
  //   * for firstLess <= i < n, a[i] < splitter
  // All the elements > splitter end up in no particular order.
  // All the elements < splitter end up in no particular order.
void split(double a[], int n, double splitter, int& firstNotGreater, int& firstLess)
{
    if (n < 0)
        n = 0;

      // It will always be the case that just before evaluating the loop
      // condition:
      //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
      //  Every element earlier than position firstNotGreater is > splitter
      //  Every element from position firstNotGreater to firstUnknown-1 is
      //    == splitter
      //  Every element from firstUnknown to firstLess-1 is not known yet
      //  Every element at position firstLess or later is < splitter

    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess)
    {
            if (a[firstUnknown] < splitter)
            {
                firstLess--;
                exchange(a[firstUnknown], a[firstLess]);
            }
            else
            {
                if (a[firstUnknown] > splitter)
                {
                    exchange(a[firstNotGreater], a[firstUnknown]);
                    firstNotGreater++;
                }
                firstUnknown++;
            }
    }
}

  // Rearrange the elements of the array so that
  // a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
  // If n <= 1, do nothing.
void order(double a[], int n)
{
	if (n <= 1) return;
	int firstNotGreater, firstLess;
	split(a, n, a[0], firstNotGreater, firstLess);
	order(a,firstNotGreater);
	order(a+firstLess, n-firstLess);
}



#include <cstdlib>
int main()
{
    double a1[7] = {10, 50, 40, 20, 50, 40, 30};
    double a2[3] = {10,20,40};
    double a21[3] = {10,40,30};
    double a22[3] = {20,10,40};
    double a23[3] = {50,40,30};
    double a3[1] = {};
    double a4[1] = {10};
    // cout << a2[0] << endl;
    // cout << a3[0] << endl;
    cout << countIncludes(a1, 7, a2, 3) << endl;   // 1
    cout << countIncludes(a1, 7, a21, 3) << endl;   // 2
    cout << countIncludes(a1, 7, a22, 3) << endl;   // 0
    cout << countIncludes(a1, 7, a23, 3) << endl;   // 3
    cout << countIncludes(a3, 0, a4, 1) << endl;   // 0

    cout << endl;
    cout << endl;
    cout << endl;

  	double arr1[25] = {};
  	for (int i = 0; i < 25; i++)
  		arr1[i] = (rand() % 2 == 0) ? -1 * (rand() % 100) : (rand() % 100);
  	p(arr1, 25);
  	cout << "---" << endl;
  	order(arr1, 25); 
  	p(arr1, 25);
  	cout << endl;
}


























