#include <iostream>
#include <string>
using namespace std;

int main()
{
    int unitsSent;
    string title;
    double basePrice;
    string premItem;
    double first400;
    double amtEarned;


    // Ask for how many units sent
    cout << "Units sent: ";
    cin >> unitsSent;
    cin.ignore(10000, '\n');
    
    // Ask for title of movie
    cout << "Title: ";
    getline(cin, title);
    
    // Ask for base price of the movie
    cout << "Base price: ";
    cin >> basePrice;
    cin.ignore(10000, '\n');
    
    // Ask for whether or not movie is premium
    cout << "Premium item? (y/n): ";
    getline(cin, premItem);

    
    cout << "---" << endl;
    if (unitsSent < 0)      // Output if user enters a negative number for units sent
    {
        cout << "The number of units sent must be nonnegative." << endl;
        return 1;   // Non-zero return value means program couldn't do its job
    }
    else if (title == "")   // Output if user does not enter a title
    {
        cout << "You must enter a title." << endl;
        return 1;
    }
    else if (basePrice < 0) // Output if user enters a negative number for base price
    {
        cout << "The base price must be nonnegative." << endl;
        return 1;
    }
    else if (premItem != "y" && premItem != "n")    // Output if user does not enter yes (y) or no (n) for premium
    {
        cout << "You must enter y or n." << endl;
        return 1;
    }
    else    // If everything is entered correctly, calculate the amount earned in royalties
    {
	    first400 = (basePrice * 0.09) * 400;
	    
        if (unitsSent <= 400)       // Royalties earned for initial 400 units sold
            amtEarned = (basePrice * 0.09) * unitsSent;
        else if (unitsSent <= 1200)  // (unitsSent > 400 or unitsSent <= 1200); Royalties earned for the next 800 units sold
            if (premItem == "y")
                amtEarned = first400 + ((basePrice * 0.16) * (unitsSent - 400));    // For premium items
            else
                amtEarned = first400 + ((basePrice * 0.13) * (unitsSent - 400));    // For non-premium items
        else //if (unitsSent > 1200); Royalties earned for units sold beyond the initial 1200
            if (premItem == "y")
                amtEarned = first400 + ((basePrice * 0.16) * 800) + ((basePrice * 0.14) * (unitsSent - 1200));  // For premium items
            else
                amtEarned = first400 + ((basePrice * 0.13) * 800) + ((basePrice * 0.14) * (unitsSent - 1200));  // For non-premium items
    
        // Set amtEarned to print with two decimal places
        cout.setf(ios::fixed);
	    cout.precision(2);
        cout << title << " earned $" << amtEarned << " in royalties." << endl;
    }
    return 0;
}