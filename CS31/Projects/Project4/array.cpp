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
    string h[7] = { "bernie", "hillary", "donald", "jeb", "", "carly", "ben" };
    string empty[0];
    assert(lookup(h, 7, "carly") == 5);
    assert(lookup(h, 7, "donald") == 2);
    assert(lookup(h, 7, "bernie") == 0);
    assert(lookup(h, 2, "donald") == -1);
    assert(lookup(h, 7, "hillsborough") == -1);
    assert(positionOfMax(h, 7) == 3);
    assert(positionOfMax(h, -5) == -1);
    assert(positionOfMax(empty, 0) == -1);

    string g[4] = { "bernie", "hillary", "jeb", "carly" };
    assert(differ(h, 4, g, 4) == 2);
    assert(appendToAll(g, 4, "?") == 4 && g[0] == "bernie?" && g[3] == "carly?");
    assert(rotateLeft(g, 4, 1) == 1 && g[1] == "jeb?" && g[3] == "hillary?");

    string e[4] = { "donald", "jeb", "", "carly" };
    assert(subsequence(h, 7, e, 4) == 2);

    string d[5] = { "hillary", "hillary", "hillary", "ben", "ben" };
    assert(countRuns(d, 5) == 2);
	
    string f[3] = { "jeb", "donald", "marco" };
    assert(lookupAny(h, 7, f, 3) == 2);
    assert(flip(f, 3) == 3 && f[0] == "marco" && f[2] == "jeb");
	
    assert(split(h, 7, "carly") == 3);

	string repeats[10] = { "millbrae", "millbrae", "millbrae", "millbrae", "burlingame", "burlingame", "cupertino", "cupertino", "cupertino", "belmont" };
	assert(countRuns(repeats, 10) == 4);

	
    cout << "All tests succeeded" << endl;
    cerr << endl;

    string cities[7] = { "millbrae", "burlingame", "sunnyvale", "cupertino", "belmont", "berkeley", "oakland" };

    string cities3[6] = {"millbrae", "burlingame", "sunnyvale", "cupertino", "belmont", "oakland"};
	string cities4[5] = {"millbrae", "burlingame", "sunnyvale", "oakland", "berkeley"};

	int q = differ(cities3, 2, cities4, 1);
	cerr << "q: " << q << endl;
	cerr << endl;

	string cities5[6] = {"millbrae", "burlingame", "sunnyvale", "cupertino", "belmont", "berkeley"};
	string cities6[3] = {"sunnyvale", "cupertino", "belmont"};
	string cities7[6] = {"millbrae", "berkeley"};

	int qw = subsequence(cities5, -6, cities6, 3);
	cerr << "qw: " << qw << endl;
	int as = subsequence(cities5, 4, cities7, 2);
	cerr << "as: " << as << endl;
	cerr << endl;

	string cities8[5] = { "hillsborough", "arcadia", "chicago", "millbrae", "cupertino" };
	string cities9[2] = {"boston", "fremont"};

	int asd = lookupAny(cities, 7, cities8, 5);
	cerr << "asd: " << asd << endl;
	int wasd = lookupAny(cities, 7, cities9, 2);
	cerr << "wasd: " << wasd << endl;
	cerr << endl;

	int wasdq = split(cities, 7, "woodside");
	cerr << "wasdq: " << wasdq << endl;

}




/*
int main()
{
	string people[5] = { "hillary", "jeb", "rand", "ben", "john" };
	int j = appendToAll(people, 5, "!!!");
	cerr << "j: " << j << endl;

	cerr << endl;
	cerr << "===========================" << endl;
	cerr << endl;
	
	string running[5] = { "carly", "mike", "ted", "bernie", "jeb" };
	int m = rotateLeft(running, 5, 1);
	cerr << "m: " << m << endl;

	cerr << endl;
	cerr << "===========================" << endl;
	cerr << endl;

	string d[13] = {"ben", "chris", "marco", "marco", "donald", "donald", "donald", "marco", "marco", "hi", "hi", "sup", "sup"};
	int p = countRuns(d, 13);
	cerr << "p: " << p << endl;

	cerr << endl;
	cerr << "===========================" << endl;
	cerr << endl;

	string folks[6] = { "chris", "marco", "", "ben", "donald", "john" };
	int q = flip(folks, 6);
	cerr << "q: " << q << endl;

	cerr << endl;
	cerr << "===========================" << endl;
	cerr << endl;

	string folks2[6] = { "chris", "marco", "", "ben", "donald", "john" };
	string group[5] = { "chris", "marco", "john", "", "carly" };
	int r = differ(folks2, 6, group, 5);  //  returns 2
	cerr << "r: " << r << endl;
	int s = differ(folks2, 2, group, 1);  //  returns 1
	cerr << "s: " << s << endl;

	cerr << endl;
	cerr << "===========================" << endl;
	cerr << endl;

	string names[10] = { "ted", "sup", "hillary", "rand", "bernie", "mike", "jeb" };
	string names1[10] = { "hillary", "rand", "bernie" };
	int t = subsequence(names, 6, names1, 3);  // returns 2
	cerr << "t: " << t << endl;
	string names2[10] = { "ted", "bernie" };
	int u = subsequence(names, 5, names2, 2);  // returns -1
	cerr << "u: " << u << endl;

	cerr << endl;
	cerr << "===========================" << endl;
	cerr << endl;

	string namess[10] = { "ted", "rand", "hillary", "bernie", "mike", "jeb" };
	string set1[10] = { "carly", "mike", "bernie", "hillary" };
	int v = lookupAny(namess, 6, set1, 4);  // returns 2 (a1 has "hillary" there)
	cerr << "v: " << v << endl;
	string set2[10] = { "ben", "donald" };
	int w = lookupAny(namess, 6, set2, 2);  // returns -1 (a1 has none)
	cerr << "w: " << w << endl;

	cerr << endl;
	cerr << "===========================" << endl;
	cerr << endl;

	string cand[6] = { "bernie", "hillary", "donald", "marco", "carly", "ben" };
	int x = split(cand, 6, "chris");  //  returns 3
	cerr << "x: " << x << endl;
	cerr << endl;
	string cand2[5] = { "donald", "hillary", "alpha", "jeb", "ben" };
	int y = split(cand2, 5, "donald");  //  returns 2
	cerr << "y: " << y << endl;
}
*/




int appendToAll(string a[], int n, string value)
{
	if(n < 0)
		return -1;
	for(int k = 0; k < n; k++)
	{
		a[k] += value;
		cerr << "Array value at " << k << ": " << a[k] << endl;
	}
	return n;
}


int lookup(const string a[], int n, string target)
{
	for(int k = 0; k < n; k++)
	{
		if(a[k] == target)
			return k;
	}
	return -1;
}

int positionOfMax(const string a[], int n)
{
	for(int k = 0; k < n; k++)
	{
		for (int i = 0; i < n; i++)
		{
			if(a[k] < a[i])
				break;
			if(i == n-1)
				return k;
		}
	}
	return -1;
}


int rotateLeft(string a[], int n, int pos)
{
	if(n < 0 || pos < 0)
		return -1;
	string copy = a[pos];
	for(int k = pos; k < n - 1; k++)
	{
		a[k] = a[k + 1];
	}
	a[n-1] = copy;

	for(int k = 0; k < n; k++)		// FOR TESTING!!!!
	{
		cerr << "Array value at " << k << ": " << a[k] << endl;
	}

	return pos;
}


int countRuns(const string a[], int n)
{
	if(n < 0)
		return -1;
	int consecSeq = 0;
	for(int k = 0; k < n; k++)
	{
		if(a[k] != a[k+1])
			consecSeq++;;
	}
	return consecSeq;
}

int flip(string a[], int n)
{
	if(n < 0)
		return -1;
	for(int k = 0; k < n/2; k++)
	{
		string copy = a[k];
		a[k] = a[n - 1 - k];
		a[n - 1 - k] = copy;
	}

	for (int i = 0; i < n; i++) 	// FOR TESTING!!!!
	{
		cerr << "ARRAY value at " << i << ": " << a[i] << endl;
	}
	return n;
}

int differ(const string a1[], int n1, const string a2[], int n2)	// NEED TESTING
{
	
	if(n1 < 0 || n2 < 0)
		return -1;
	
	int n = n1;
	if(n2 < n1) n = n2;		// Compare using array with smallest number of elements
	for(int k = 0; k < n; k++) 
	{
		cerr << a1[k] << " ?!= " << a2[k] << endl;	// FOR TESTING!!!
		if(a1[k] != a2[k])
			return k;
	}
	return n;
}

int subsequence(const string a1[], int n1, const string a2[], int n2)	// CORRECT?
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
	return pos;
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
	for(int x = 0; x < n; x++)
		cerr << "Original Array in Split: Element in " << x << ": " << a[x] << endl;
	cerr << endl;

	if(n < 0)
		return -1;

	// sort the array alphabetically
	for(int k = 0; k < n; k++)
	{
		int j = k;
		for(int i = k; i < n; i++)
			if(a[i] < a[j])
				j = i;

		string copy = a[j];
		a[j] = a[k];
		a[k] = copy;
	}

	for(int x = 0; x < n; x++)
		cerr << "Final Array in Split: Element in " << x << ": " << a[x] << endl;
	cerr << endl;

	for(int h = 0; h < n; h++)
	{
		if(a[h] >= splitter)
			return h;
	}
	
	return n;
}
