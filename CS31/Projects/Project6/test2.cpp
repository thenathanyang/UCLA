#include <iostream>
    using namespace std;

    int* getPtrToArray(int& m)
    {
        int anArray[100];
        for (int j = 0; j < 100; j++)
            anArray[j] = 100-j;
        m = 100;
        return anArray;
    }

    void f()
    {
        int junk[100];
        for (int k = 0; k < 100; k++)
            junk[k] = 123400000 + k;
    }

    int main()
    {
        int n;
        int* ptr = getPtrToArray(n);
        cout << n << endl;

        f();
        for (int i = 0; i < 3; i++)
            cout << ptr[i] << ' ';
        for (int i = n-3; i < n; i++)
            cout << ptr[i] << ' ';
        cout << endl;
    }