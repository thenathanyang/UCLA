// tempMain.cpp

        #include "newMap.h"
        #include <string>
        #include <iostream>
        #include <cassert>
        using namespace std;

        void test()
        {
            Map m;
            assert(m.insert(10, "diez"));
            assert(m.insert(20, "veinte"));
            assert(m.size() == 2);
            ValueType v = "cuarenta y dos";
            assert(!m.get(30, v)  &&  v == "cuarenta y dos");
            assert(m.get(10, v)  &&  v == "diez");
            v = "cuarenta y dos";
            KeyType x = 30;
            assert(m.get(0, x, v)  &&
                   ((x == 10  &&  v == "diez")  ||  (x == 20  &&  v == "veinte")));
            KeyType x2 = 40;
            assert(m.get(1, x2, v)  &&
                   ((x2 == 10  &&  v == "diez")  ||  (x2 == 20  &&  v == "veinte"))  &&
                   x != x2);
        }

        int main()
        {
            test();
            cout << "Passed all tests" << endl;
        }
