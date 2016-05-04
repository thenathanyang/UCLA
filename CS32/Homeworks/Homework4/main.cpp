#include "Map.h"
#include <cassert>
#include <string>
#include <iostream>

using namespace std;

void test()
{
	Map<string, int> msi;
	Map<int, double> mid;
	assert(msi.empty());
	assert(mid.empty());
	assert(msi.size() == 0);
	assert(mid.size() == 0);
	assert(msi.insert("Hello", 10));
	assert(mid.insert(10, 3.5));
	assert(msi.update("Hello", 20));
	assert(mid.update(10, 4.75));
	assert(msi.insertOrUpdate("Goodbye", 30));
	assert(mid.insertOrUpdate(20, 5.25));
	assert(msi.erase("Goodbye"));
	assert(mid.erase(20));
	assert(msi.contains("Hello"));
	assert(mid.contains(10));
	int k;
	assert(msi.get("Hello", k)  &&  k == 20);
	string s;
	assert(msi.get(0, s, k)  &&  s == "Hello"  &&  k == 20);
	double d;
	assert(mid.get(10, d)  &&  d == 4.75);
	assert(mid.get(0, k, d)  && k == 10  &&  d == 4.75);
	Map<string, int> msi2(msi);
	msi2.swap(msi);
	msi2 = msi;
	Map<int, double> mid2(mid);
	mid2.swap(mid);
	mid2 = mid;
	combine(msi2, msi, msi);
	combine(mid2, mid, mid);
	subtract(msi2, msi, msi);
	subtract(mid2, mid, mid);
}

void test2()
{
	Map<int, double> mid;
	Map<string, int> msi;
	assert(msi.empty());
	assert(msi.size() == 0);
	assert(msi.insert("Hello", 10));
	assert(mid.insert(10, 3.5));
	assert(msi.update("Hello", 20));
	assert(mid.update(10, 4.75));
	assert(msi.insertOrUpdate("Goodbye", 30));
	assert(msi.erase("Goodbye"));
	assert(mid.contains(10));
	int k;
	assert(msi.get("Hello", k));
	string s;
	assert(msi.get(0, s, k));
	Map<string, int> msi2(msi);
	msi2.swap(msi);
	msi2 = msi;
	combine(msi,msi2,msi);
	combine(mid,mid,mid);
	subtract(msi,msi2,msi);
	subtract(mid,mid,mid);
}

int main()
{
    test();
	test2();
	cout << "Passed all tests" << endl;
}
