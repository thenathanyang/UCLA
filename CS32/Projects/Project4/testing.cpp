#include <iostream>
#include "DiskMultiMap.h"
using namespace std;

// int main_old()
// {
// 	DiskMultiMap x;

// 	x.createNew("myhashtable.dat",100); // empty, with 100 buckets
// 	x.openExisting("myhashtable.dat");
// 	x.insert("hmm.exe", "pfft.exe", "m52902");
// 	cout << "------ " << endl; 
// 	x.insert("hmm.exe", "pfft.exe", "m52902"); 
// 	cout << "------ " << endl;
// 	x.insert("hmm.exe", "pfft.exe", "m10001"); 
// 	cout << "------ " << endl;
// 	x.insert("hmm.exe", "pfft.exe", "m10001"); 
// 	cout << "------ " << endl;
// 	x.close();
// 	x.openExisting("myhashtable.dat");
// 	x.insert("blah.exe", "bletch.exe", "m0003");
// 	cout << "------ " << endl;
// 	cout << "ABOUT TO ERASE HEREEEEEEEEEEEEEEEEEEEEEEEEEEE" << endl;
// 	x.erase("hmm.exe", "pfft.exe", "m52902");
// 	cout << "------ " << endl;
// 	x.insert("merp.exe", "bleh.exe", "m1337");
// 	cout << "------ " << endl;

// 	x.createNew("second.dat",100);
// 	x.openExisting("second.dat");
// 	x.insert("blehh.exe", "merpp.exe", "m1738");
// 	cout << "------ " << endl;
// 	x.insert("hmm.exe", "pfft.exe", "m10001"); 
// 	cout << "------ " << endl;
// 	x.close();
// 	x.openExisting("myhashtable.dat");

// 	DiskMultiMap::Iterator it = x.search("hmm.exe"); 
// 	if (it.isValid())
// 	{
// 		cout << "I found at least 1 item with a key of hmm.exe" << endl; 
// 		do
// 		{
// 			MultiMapTuple m = *it; // get the association
// 			cout << "The key is: " << m.key << endl;
// 			cout << "The value is: " << m.value << endl; 
// 			cout << "The context is: " << m.context << endl; 
// 			cout << endl;

// 			++it; // advance iterator to the next matching item 
// 		} while (it.isValid());
// 	}
// 	else
// 		cout << "Nope." << endl;
// }

// NEED TO FIX INSERTING INTO OLD DELETED POSITIONS. 
// FIRST DEL NODE POSITION NOT UPDATING?!?!?!?