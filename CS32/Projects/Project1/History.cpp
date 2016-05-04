// History.cpp

#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
	m_rows = nRows;
	m_cols = nCols;

	int r, c;
    
        // Fill the grid with dots
    for (r = 0; r < m_rows; r++)
        for (c = 0; c < m_cols; c++)
            grid[r][c] = '.';
}

bool History::record(int r, int c)
{
	if (r < 1  ||  r > m_rows  ||  c < 1  ||  c > m_cols)
		return false;

	char& gridChar = grid[r-1][c-1];		// WHY AGAIN?!
	switch (gridChar)
        {
          case '.':  
          	gridChar = 'A'; 
          	break;
          case 'Z':
          	break;
          default:   
          	gridChar++; 
          	break;  // 'B' (66) through 'Y' (89)
        }
    return true;
}

void History::display() const
{
	int r, c;
            // Draw the grid
    clearScreen();
    for (r = 0; r < m_rows; r++)
    {
        for (c = 0; c < m_cols; c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;
}
