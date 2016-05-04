// testWeightMap.cpp

#include "WeightMap.h"
#include <iostream>
#include <cassert>
#include <string>
#include <cstdlib>
#include <algorithm>
using namespace std;

int main()
{
	WeightMap program;

	//assert(program.enroll("Person1", 1234.5));
	//assert(program.enroll("Person2", 2345.6));
	//assert(program.enroll("Person3", 3456.7));

	//double randWeight = 5555.5;

	//string j[200];
	//char v[3];
	for (int k = 0; k < 199; k++)
		program.enroll("blehh" + to_string(k), rand() % 20); 

	// for (int k = 0; k < 200; k++)
	// 	assert(program.enroll(j[k], rand() % 20));

	cout << "Should return 199: " << endl;
	cout << program.size() << endl;					// Should return 199

	cout << endl;

	cout << program.weight("blehh3") << endl;
	cout << program.weight("blehh58") << endl;
	cout << program.weight("blehh128") << endl;
	cout << program.weight("blehh198") << endl;

	cout << endl;

	cout << "Should return -1 b/c 'Person1' doesn't exist: " << endl;
	cout << program.weight("Person1") << endl;		// Should return -1 b/c "Person1" doesn't exist

	cout << endl;

	cout << program.weight("blehh3") << endl;
	program.adjustWeight("blehh3", 32.1);
	cout << program.weight("blehh3") << endl;

	cout << endl;

	cout << program.weight("blehh58") << endl;
	program.adjustWeight("blehh58", -1);
	cout << program.weight("blehh58") << endl;

	cout << endl;
	cout << endl;
	cout << endl;

	program.print();

	cout << endl;
	cout << endl;
	cout << endl;

	assert(! program.enroll("blehh22", 123));			// Should fail b/c "blehh22" exists
	assert(! program.enroll("blehh199", -123));			// Should fail b/c startWeight is negative
	assert(program.enroll("blehh199", 123));
	cout << "Should return 200: " << endl;
	cout << program.size() << endl;						// Should return 200
	assert(! program.enroll("blehh200", 123));			// Should fail b/c "blehh200" is 201 > DEFAULT_MAX_ITEMS
	assert(! program.adjustWeight("Person2", 123));		// Should fail b/c "Person2" doesn't exist
	assert(! program.adjustWeight("blehh45", -500));	// Should fail b/c weight() + amt is negative	
	assert(! program.adjustWeight("blehh128", -200));	// Should fail b/c weight() + amt is negative

	cout << endl;
	cout << endl;
	cout << endl;

	program.print();

	cout << endl;
	cout << endl;
	cout << endl;

	cout << "Passed all tests" << endl;
}

