// linear.cpp

bool somePredicate(double x);

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
int indexOfMin(const double a[], int n)
{
    if (n <= 0) return -1;
    if (n == 1) return 0;

    int temp = indexOfMin(a+1, n-1) + 1;
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
