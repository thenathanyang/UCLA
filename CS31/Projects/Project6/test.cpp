#include <iostream>
    using namespace std;

    int* maxwell(int* a, int* b)
    {
        if (*a > *b)
            return a;
        else
            return b;
    }

    void swap1(int* a, int* b)
    {
        int* temp = a;
        a = b;
        b = temp;
    }

    void swap2(int* a, int* b)
    {
        int temp = *a;
        *a = *b;
        *b = temp;
    }

    int main()
    {
        int array[6] = { 5, 3, 4, 17, 22, 19 };

        cout << array << endl;
        cout << &array[2] << endl;
        cout << array[2] << endl;
        cout << endl;

        int* ptr = maxwell(array, &array[2]);   // Returns &array[2]
        cout << (array > &array[2]) << endl;
        cout << "ptr: " << ptr << endl;
        cout << endl;

        *ptr = -1;          // 5, 3, -1, 17, 22, 19
        cout << array[0] << endl;
        cout << *ptr << endl;

        cout << endl;

        ptr += 2;
        ptr[1] = 9;

        cout << *(array + 1);
        *(array+1) = 79;
        cout << endl;
        cout << endl;

        cout << endl;
        for (int i = 0; i < 6; i++)
            cout << array[i] << endl;
        cout << endl;


        cout << "Hi" << endl;
        cout << &array[5] << endl;
        cout << ptr << endl;
        cout << &array[5] - ptr << endl;        // prints 5 locations apart
        cout << endl;

        //swap1(&array[0], &array[1]);
        swap2(array, &array[2]);

        for (int i = 0; i < 6; i++)
            cout << array[i] << endl;
    }