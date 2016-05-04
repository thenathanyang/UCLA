// linear.cpp

#include <iostream>
#include <cassert>
using namespace std;

bool somePredicate(double x)
{
    return x < 0;
}

  // Return true if the somePredicate function returns true for at
  // least one of the array elements, false otherwise.
bool anyTrue(const double a[], int n)
{
	if (n <= 0)
		return false;

	if (somePredicate(a[0]))
		return true;
	return anyTrue(a+1, n-1);
}

  // Return the number of elements in the array for which the
  // somePredicate function returns true.
int countTrue(const double a[], int n)
{
  if (n <= 0)
    return 0;

  int temp = countTrue(a+1, n-1);
  if (somePredicate(a[0]))
    return 1 + temp;
  return temp;
}


  // Return the subscript of the first element in the array for which
  // the somePredicate function returns true.  If there is no such
  // element, return -1.
// int firstTrue(const double a[], int n)
// {
//     if (n <= 0)    // CORRECT?!?!?
//     	return -1;

//     if (somePredicate(a[n-1]) && firstTrue(a, n-1) == -1)
// 		    return n-1;
// 	  return firstTrue(a, n-1);
// }

int firstTrue(const double a[], int n)
{
    if (n <= 0)
      return -1;

    int temp = firstTrue(a, n-1);
    if (somePredicate(a[n-1]) && temp == -1)
        return n-1;
    return temp;
}

  // Return the subscript of the smallest element in the array (i.e.,
  // the one whose value is <= the value of all elements).  If more
  // than one element has the same smallest value, return the smallest
  // subscript of such an element.  If the array has no elements to
  // examine, return -1.
void p(const double* a, int n) {
  while (n--) {
    cout << *(a++) << ", ";
  }
}

int indexOfMin(const double a[], int n)
{
    static int counter = 0;


    cout << (++counter) << ": array=["; p(a,n); cout <<  "]"; cout << endl;// "|||" << n << "|||" << endl;
    if (n == 1) cout << "return 0, reached end of array" << endl;

    if (n <= 0) return -1;
    if (n == 1) return 0;


    int temp = indexOfMin(a+1, n-1) + 1;    // Every time recursion called, pushed back one. That's why +1

    p(a,n); cout << " - Comparing " << a[temp] << " to " << a[0] << ". Returning value " << endl;

    if (a[0] > a[temp])
        return temp;
    return 0;

}

  // If all n2 elements of a2 appear in the n1 element array a1, in
  // the same order (though not necessarily consecutively), then
  // return true; otherwise (i.e., if the array a1 does not include
  // a2 as a not-necessarily-contiguous subsequence), return false.
  // (Of course, if a2 is empty (i.e., n2 is 0), return true.)
  // For example, if a1 is the 7 element array
  //    10 50 40 20 50 40 30
  // then the function should return true if a2 is
  //    50 20 30
  // or
  //    50 40 40
  // and it should return false if a2 is
  //    50 30 20
  // or
  //    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 == 0)
      return true;
    if (n1 < 0 || n2 > n1)
      return false;

    if (a1[0] == a2[0])
       return includes(a1+1, n1-1, a2+1, n2-1);
    return includes(a1+1, n1-1, a2, n2);
}


int main()
{
	double arr[3] = {3, 1, -1};
	double arr2[4] = {3, 1, -1, -3};
  double arr201[6] = {2, -4, 4, -2, -3, -5};
  double arr21[5] = {1, 2, 3, 4, -5};
  double arr22[5] = {-1, 2, 3, 4, -5};
	double arr3[7] = {10, 50, 40, 20, 50, 40, 30};
	double arr4[3] = {10, 40, 30};
  double arr41[3] = {50, 20, 30};
  double arr42[3] = {50, 40, 40};
  double arr43[3] = {50, 30, 20};
  double arr44[3] = {10, 20, 20};
  assert(anyTrue(arr, 3) == 1);
  cout << "Any True: " << anyTrue(arr, 3) << endl;        // 1
  cout << "Count True: " << countTrue(arr,3) << endl;     // 1
  cout << "Count True: " << countTrue(arr2,4) << endl;    // 2
  cout << "Count True: " << countTrue(arr201,6) << endl;    // 4
	cout << "First True: " << firstTrue(arr, 3) << endl;    // 2
	cout << "First True: " << firstTrue(arr2, 4) << endl;   // 2
  cout << "First True: " << firstTrue(arr21, 5) << endl;   // 4
  cout << "First True: " << firstTrue(arr22, 5) << endl;   // 0
	assert(includes(arr3, 7, arr4, 3) == 1);
  assert(includes(arr3, 7, arr41, 3) == 1);
  assert(includes(arr3, 7, arr42, 3) == 1);
  assert(includes(arr3, 7, arr43, 3) == 0);
  assert(includes(arr3, 7, arr44, 3) == 0);
	double arr5[8] = {100, 50, 30, 40, 50, 10, 30, 40};
  double arr6[2] = {1, 2};
  double arr7[3] = {1, 3, 2};
  double arr8[3] = {3, 1, 2};
  double arr9[3] = {3, 2, 1};
	cout << indexOfMin(arr7, 3) << endl;  // 0
  cout << indexOfMin(arr8, 3) << endl;  // 1
  cout << indexOfMin(arr9, 3) << endl;  // 2
  cout << indexOfMin(arr5, 8) << endl;  // 5

  double arr10[5] = {2,3,1,5,4};
  cout << indexOfMin(arr5, 8) << endl; // 5
  cout << "PASSED ALL TESTS" << endl;

}
