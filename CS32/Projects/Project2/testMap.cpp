// Nathan's CS 32 Project 2 Test Cases

// testMap.cpp

#include "Map.h"

#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	Map m;
	ValueType v = -1234.5;

	assert(m.empty());								// Test empty
	assert(m.size() == 0);							// Test size
	assert(m.insert("blehh", 123.4));				// Test to see if inserted successfully
	assert(! m.empty());
	assert(m.size() == 1);
	assert(m.contains("blehh"));					// Test contains
	assert(! m.insert("blehh", 432.1));				// Test to see if unable to insert
	assert(! m.update("blehh500", 500.5));			// Test to see if fail to update
	assert(! m.get("abc", v)  &&  v == -1234.5); 	// Test to see if unable to get value; v unchanged by get failure
	assert(m.get("blehh", v) && v == 123.4);		// Test to see if able to get value successfully
	assert(m.insert("blehh1", 111.1));
	assert(m.size() == 2);
	assert(m.contains("blehh1"));
	assert(m.update("blehh", 5432.1));				// Test update
	assert(m.get("blehh", v) && v == 5432.1);		// Test to see if update succeeded


	KeyType testKey1;
	KeyType testKey2;

	ValueType testValue1 = 0;
	ValueType testValue2 = 0;

	assert(m.get(0, testKey1, testValue1) && testKey1 == "blehh" && testValue1 == 5432.1);			// Test to see if able to get key and value successfully
	assert(m.get(1, testKey2, testValue2) && testKey2 == "blehh1" && testValue2 == 111.1);
	assert(! m.get(200, testKey2, testValue2));														// Test to see if unable to get key and value 


	ValueType testValue3 = 0;

	Map n;
	assert(n.empty());
	assert(n.size() == 0);
	assert(n.insertOrUpdate("merp", 123.4));		// Test insertOrUpdate (insert aspect)
	assert(! n.empty());
	assert(n.size() == 1);
	assert(n.contains("merp"));
	assert(! n.insert("merp", 432.1));
	assert(! n.update("merp500", 500.5));
	assert(n.insertOrUpdate("merp", -123.4));		// Test inserOrUpdate (update aspect)
	assert(n.get("merp", testValue3) && testValue3 == -123.4);			// Test to see if successfully updated

	n.swap(m);										// Test swap

	assert(m.contains("merp"));						// Test to see if m and n correctly swapped
	assert(m.size() == 1);
	assert(n.contains("blehh"));
	assert(n.contains("blehh1"));
	assert(n.size() == 2);

	assert(n.insert("blehh2", 222.2));
	assert(n.insert("blehh3", 333.3));
	assert(n.insert("blehh4", 444.4));
	assert(n.size() == 5);


	Map testMap1(n);								// Test copy constructor
	assert(testMap1.contains("blehh"));
	assert(testMap1.contains("blehh4"));
	assert(testMap1.size() == 5);

	testMap1 = m;									// Test assignment operator
	assert(testMap1.contains("merp"));
	assert(testMap1.size() == 1);

	Map a(n);
	assert(a.erase("blehh"));						// Test erase at the front/head
	assert(! a.contains("blehh"));
	assert(a.contains("blehh1"));
	assert(a.size() == 4);

	Map b(n);										// Test erase in the middle
	assert(b.erase("blehh2"));
	assert(! b.contains("blehh2"));
	assert(b.contains("blehh1"));
	assert(b.contains("blehh4"));
	assert(b.size() == 4);

	Map c(n);										// Test erase in the middle
	assert(c.erase("blehh1"));
	assert(! c.contains("blehh1"));
	assert(c.contains("blehh"));
	assert(c.size() == 4);

	Map d(n);										// Test erase at the back/tail
	assert(d.erase("blehh4"));
	assert(! d.contains("blehh4"));
	assert(d.contains("blehh3"));
	assert(d.size() == 4);

	Map e(m);										// Test erase in an one-element map situation
	assert(e.size() == 1);
	assert(e.erase("merp"));
	assert(! e.contains("merp"));
	assert(e.empty());

	Map f;											// Test erase in an empty map situation
	assert(f.size() == 0);
	assert(! f.contains("sup"));
	assert(! f.erase("sup"));



	// Testing combine and subtract

	Map names1;
	assert(names1.insert("Fred", 123));
	assert(names1.insert("Ethel", 456));
	assert(names1.insert("Lucy", 789));
	assert(names1.size() == 3);

	Map names2;
	assert(names2.insert("Lucy", 789));
	assert(names2.insert("Ricky", 321));

	Map names3;
	assert(names3.insert("Lucy", 654));
	assert(names3.insert("Ricky", 321));

	Map names4;
	assert(names4.insert("Fred", 123));
	assert(names4.insert("Ethel", 456));
	assert(names4.insert("Lucy", 789));

	Map names5;
	assert(names5.insert("Lucy", 789));
	assert(names5.insert("Ricky", 321));
	assert(names5.insert("Ethel", 654));

	Map namesResult1;									// Test combine
	assert(namesResult1.insert("random", 40));			// namesResult1 is not empty. After combine, result the result should only include keys from the combined maps.
	assert(namesResult1.insert("random1", 40.1));
	assert(namesResult1.size() == 2);
	assert(combine(names1, names2, namesResult1));		// Combine, only include one key/value pair with the "Lucy" key, and return true
	cout << "namesResult1:" << endl; 					// "Lucy" key in both maps have the same value
	namesResult1.dump();

	cout << endl;

	Map namesResult2;									// Test combine
	assert(! combine(names1, names3, namesResult2));	// Don't include the key/value pair with the "Lucy" key and return false
	cout << "namesResult2:" << endl; 					// "Lucy" key in both maps have different values
	namesResult2.dump();

	cout << endl;

	Map namesResult3;									// Test subtract
	assert(namesResult3.insert("random", 24.5));		// Subtract the maps and only include the key/value pair with the "Fred" key
	subtract(names4, names5, namesResult3);				// All the other keys are in both maps
	cout << "namesResult3:" << endl; 					
	namesResult3.dump();



	Map g;
	assert(g.empty());
	assert(g.insert("ehh", 111.1));
	assert(g.insert("ehh1", 111.2));
	assert(g.insert("ehh2", 111.3));
	assert(! g.insert("ehh2", 111.03));
	assert(g.insert("ehh3", 111.4));
	assert(g.size() == 4);
	
	Map h;
	assert(h.empty());							
	assert(h.insert("uhh", 222.1));	 
	assert(h.insert("ehh1", 111.2));
	assert(h.insert("ehh2", 222.03));
	assert(h.insert("uhh1", 222.2));
	assert(h.insert("uhh2", 222.3));
	assert(h.size() == 5);

	Map i;
	assert(i.empty());					
	assert(! combine(g, h, i));			// Test to see if return false when keys with different values are attempted to be combined



	cout << endl;
	cout << "==========================" << endl;
	cout << "||                      ||" << endl;
	cout << "||   PASSED ALL TESTS   ||" << endl;
	cout << "||                      ||" << endl;
	cout << "==========================" << endl;
	cout << endl;
}



