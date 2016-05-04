// testnewMap.cpp

#include "newMap.h"
#include <iostream>
#include <cassert>
#include <string>
#include <cstdlib>
#include <algorithm>
using namespace std;

int main()
{	
    Map a(1000);   // a can hold at most 1000 key/value pairs
    Map b(5);      // b can hold at most 5 key/value pairs
    Map c;         // c can hold at most DEFAULT_MAX_ITEMS key/value pairs
    KeyType k[6] = { "ab", "cd", "ef", "gh", "hi", "jk" };
    ValueType v  = 4.5;

      // No failures inserting pairs with 5 distinct keys into b
    for (int n = 0; n < 5; n++)
        assert(b.insert(k[n], v));

      // Failure if we try to insert a pair with a sixth distinct key into b
    assert(!b.insert(k[5], v));

      // When two Maps' contents are swapped, their capacities are swapped
      // as well:
    a.swap(b);
    assert(!a.insert(k[5], v)  &&  b.insert(k[5], v));

    c.swap(a);
    assert(!c.insert(k[5], v)  &&  a.insert(k[5], v));




    Map m;  // maps strings to doubles
    assert(m.empty());
    ValueType x = -1234.5;
    assert( !m.get("abc", x)  &&  x == -1234.5); // v unchanged by get failure
    m.insert("xyz", 9876.5);
    assert(m.size() == 1);
    //cerr << m.size() << endl;
    KeyType j = "hello";
    //cerr << m.get(0, j, x) << " " << j << " " << x << endl;
    assert(m.get(0, j, x)  &&  j == "xyz"  &&  x == 9876.5);

    Map n;
    assert(n.empty());
    assert(n.size() == 0);		// Should return 0 b/c map is empty

    for (int k = 0; k < 198; k++)
		n.insert("blehh" + to_string(k), rand() % 20); 

	assert(n.size() == 198);
	assert(! n.insert("blehh23", 243.1));				// Should fail b/c "blehh23" already exists
	assert(! n.insert("blehh24", 523.4));			// Should fail b/c "blehh24" already exists
	assert(n.insert("blehh198", 245.2));
	assert(n.insertOrUpdate("blehh199", 123.4));
	assert(! n.insert("blehh200", 214.5));				// Should fail b/c max size already reached
	assert(! n.insertOrUpdate("blehh201", 6234.4));		// Should fail b/c max size already reached
	assert(n.size() == 200);
	assert(n.update("blehh12", 1212.12));
	assert(n.insertOrUpdate("blehh45", 4545.45));
	assert(n.insertOrUpdate("blehh24", 2424.24));	
	assert(! n.update("blehh400", 432.1));				// Should fail b/c "blehh400" doesn't eixst
	assert(n.contains("blehh56"));
	assert(! n.contains("blehh500"));					// Should fail b/c "blehh500" doesn't exist

	ValueType testValue1 = 0;
	ValueType testValue2 = 0;
	ValueType testValue3 = 0;

	KeyType testKey1;
	KeyType testKey2;

	assert(! n.get("blehh600", testValue1));			// Should fail b/c "blehh600" doesn't eixst
	assert(n.get("blehh198", testValue1) && testValue1 == 245.2);
	assert(! n.get(-3, testKey1, testValue2));		// Should fail b/c i = -3 < 0
	assert(! n.get(200, testKey1, testValue2));		// Should fail b/c i = 200 = size
	assert(! n.get(245, testKey1, testValue2));		// Should fail b/c i = 245 > size
	assert(n.get(199, testKey1, testValue2) && testKey1 == "blehh199" && testValue2 == 123.4);
	assert(n.get(12, testKey2, testValue3) && testKey2 == "blehh12" && testValue3 == 1212.12);

	n.swap(m);

	assert(m.size() == 200);
	assert(! m.insert("blehh200", 214.5));				// Should fail b/c max size already reached
	assert(! m.insertOrUpdate("blehh201", 6234.4));		// Should fail b/c max size already reached
	assert(! m.update("blehh400", 432.1));				// Should fail b/c "blehh400" doesn't eixst
	assert(m.contains("blehh56") && m.contains("blehh84"));
	assert(! m.contains("blehh500"));					// Should fail b/c "blehh500" doesn't exist

	assert(n.get(0, j, x)  &&  j == "xyz"  &&  x == 9876.5);

	assert(! m.erase("blehh700"));		// Should fail b/c "blehh700" doesn't exist
	assert(m.erase("blehh66"));
	assert(m.size() == 199);

	Map d = m;	// copy constructor
	
	assert(d.size() == 199);
	assert(d.insert("blehh200", 214.5));				// Should fail b/c max size already reached
	assert(d.size() == 200);
	assert(! d.insert("blehh201", 2175.4));				// Should fail b/c max size already reached
	assert(! d.insertOrUpdate("blehh202", 6234.4));		// Should fail b/c max size already reached
	assert(! d.update("blehh400", 432.1));				// Should fail b/c "blehh400" doesn't eixst
	assert(d.contains("blehh56") && d.contains("blehh84"));
	assert(! d.contains("blehh500"));					// Should fail b/c "blehh500" doesn't exist
	assert(! d.contains("blehh66"));					// Should fail b/c "blehh66" doesn't exist anymore



	d = n;	// assignment operator

	assert(d.size() == 1);
	assert(d.get(0, j, x)  &&  j == "xyz"  &&  x == 9876.5);


    cout << "Passed all tests" << endl;
}


