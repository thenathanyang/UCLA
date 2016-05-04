#include <iostream>
using namespace std;

// int main()
// {
//     int arr[3] = { 5, 10, 15 };
//     int* ptr = arr;

//     *ptr = 30;          // set arr[0] to 30
//     *(ptr + 1) = 20;    // set arr[1] to 20
//     ptr += 2;
//     ptr[0] = 10;        // set arr[2] to 10

//     ptr = arr;
//     while (ptr < arr + 3)
//     {
//         cout << *ptr << endl;    // print values
//         ptr++;
//     }
// }



void findMax(int arr[], int n, int*& pToMax)
    {
        if (n <= 0) 
            return;      // no items, no maximum!
    
        pToMax = arr;

        for (int i = 1; i < n; i++)
        {
            if (arr[i] > *pToMax)
                pToMax = arr + i;
        }
    }       

    int main()
    {
        int nums[4] = { 5, 3, 15, 6 };
        int* ptr;

        findMax(nums, 4, ptr);
        cout << "The maximum is at address " << ptr << endl;
        cout << "It's at position " << ptr - nums << endl;
        cout << "Its value is " << *ptr << endl;
    }




// void computeCube(int n, int* ncubed)
//     {
//         *ncubed = n * n * n;
//     }

// int main()
//     {
//         int n;
//         int* ptr = &n;      // Wasnt pointing anywhere
        
//         computeCube(5, ptr);
//         cout << "Five cubed is " << *ptr << endl;
//     }


// void computeCube(int n, int* ncubed)
//     {
//         *ncubed = n * n * n;
//     }

// int main()
//     {
//         int n;
//         //int* ptr = &n;      // Wasnt pointing anywhere
        
//         computeCube(5, &n);
//         cout << "Five cubed is " << n << endl;
//     }


      // return true if two C strings are equal


    // bool strequal(const char str1[], const char str2[])
    // {
    //     while (str1 != 0  &&  str2 != 0)
    //     {
    //         if (str1 != str2)  // compare corresponding characters
    //             return false;
    //         str1++;            // advance to the next character
    //         str2++;
    //     }
    //     return str1 == str2;   // both ended at same time?
    // }

    // int main()
    // {
    //     char a[15] = "Noor";
    //     char b[15] = "Noah";

    //     if (strequal(a,b))
    //         cout << "They're the same person!\n";
    // }

// void removeS(char* str);

// int main()
//     {
//         char msg[50] = "She'll be a massless princess. Yesssssssasss";
//         removeS(msg);
//         cout << msg << endl;  // prints   he'll be a male prince.
//     }

// void removeS(char* str)
// {
//     char* ptr = str;
//     while(*ptr != '\0')
//     {
//         if(*ptr == 's' || *ptr == 'S')
//         {
//             while(*ptr != '\0')     // Move all characters following the s or S one position to the left
//             {
//                 *ptr = *(ptr + 1);      
//                 ptr++;
//             }
//             ptr = str;      // Set pointer to beginning of the string
//         }   
//         ptr++;      // Move on to next character in string
//     }
// }



//  bool strequal(const char str1[], const char str2[]);

// int main()
//     {
//         char a[15] = "Noor";
//         char b[15] = "Noah";

//         if (strequal(a,b))
//             cout << "They're the same person!\n";
//         else
//             cout << "No." << endl;;
//     }


//  bool strequal(const char str1[], const char str2[])
// {
//         cout << str1 << endl;
//         cout << str2 << endl;
//         cout << endl;

//         int numCharsInStr1 = 0;
//         while (str1[numCharsInStr1] != 0)
//             numCharsInStr1++;

//         int numCharsInStr2 = 0;
//         while (str2[numCharsInStr2] != 0)
//             numCharsInStr2++;

//         if(numCharsInStr1 != numCharsInStr2)
//             return false;

//         int k = 0;
//         while (str1[k] != 0  &&  str2[k] != 0)
//         {
//             cout << str1[k] << ", " << str2[k] << endl;
//             if (str1[k] != str2[k])  // compare corresponding characters
//                 return false;
//             k++;            // advance to the next character
//         }
//         return true;
//     }

// double mean(const double* scores, int numScores)
// {
//     double tot = 0;
//     for (int k = 0; k < numScores; k++)
//     {
//         tot += *(scores + k);           // CORRECT?!?!
//     }
//     return tot/numScores;

// }

// int main()
// {
//     return 0;
// }



// PROBLEM 4:

// The program prints 3, 4, 79, -1, 9, 22, and 19 with each number of a new line. In the main function, ptr is set to the address maxwell returns. Maxwell returns the address of the larger value of the two arguments. In this case, maxwell is comparing the value at array (or array[0]) to the value at array[2]. The value in array, which is 5, is greater than the value in array[2], which is 4. As a result, it returns the address of array and the ptr is set to that address. The value of ptr is then set from 5 to -1. ptr being increment by two sets the pointer to point to array[2]. ptr[1] sets array[3] to 9. *(array + 1) sets array[1] to 79. The program then prints out 3 because the address of array[5] minus ptr (which is pointing at array[2]) is 3. In other words, array[5] is 3 indices away from array[2]. swap1 does not have any affect on the array because the values were passed in by value. Also, all the variables within swap1 were changed locally, thus not affecting the array in the main function. swap2 was able to change the array in the main function because it changed the values the pointers were pointing at. In this case, the addresses of array and array[2] were passed into swap2 and the values stored in array and array[2] were swapped. As a result, the value in array is set to 4 while the value of array[2] is set to -1. Next, the program prints out the array with each element of the array on a new line. 