#include <iostream>
#include <string>
#include <cctype>
#include <cassert>
#include "grid.h" // makes your program aware of the functions in grid.cpp
using namespace std;

bool isDirectionValid(char dir);
bool isLocationValid(int r, int c);
bool isRouteWellFormed(string route);
int stepsTakenInDir(int r, int c, char dir, int maxSteps);
int navigateSegment(int r, int c, char dir, int maxSteps);
int navigateRoute(int sr, int sc, int er, int ec, string route, int& nsteps);

int main()
{
	return 0;
}

bool isRouteWellFormed(string route)
{
	// Check first letter, then see if 0, 1, or 2 digits after, if so move on to next segment
	int size = route.size();
	int pos = 0;

	while(pos < size)
	{
		if(!isDirectionValid(route[pos]))		// If character is not a directional character --> return false
			return false;

		int digits = 0;
		pos++;

		while(pos < size)
		{
			if(!isdigit(route[pos]) && !isDirectionValid(route[pos]))	// If character is not a digit or valid direction character, segment invalid --> return false
				return false;
			if(isDirectionValid(route[pos]))	// If valid direction character, break out of loop
				break;
			if(isdigit(route[pos]))				// Counts number of digits in segment
				digits++;
			if(digits > 2)						// If more than two digits, segment invalid --> return false		
				return false;
			pos++;
		}
	}
	return true;		// If route is valid, return true
}

int navigateSegment(int r, int c, char dir, int maxSteps)
{
	if (!isLocationValid(r, c))	// Checks to see if starting position is on grid
		return -1;
	else if (isWall(r,c) == 1)	// Checks to see if starting position is in wall
		return -1;
	else if (isDirectionValid(dir) == 0)	// Checks to see if direction is valid
		return -1;
	else if (maxSteps < 0)	// Checks to see if maxSteps is less than 0
		return -1;
	else
		return stepsTakenInDir(r, c, dir, maxSteps);	// Returns number of steps taken if everything is correct
}

int navigateRoute(int sr, int sc, int er, int ec, string route, int& nsteps)
{
	if (!isLocationValid(sr,sc) || !isLocationValid(er,ec) || isWall(sr,sc) || isWall(er,ec) || !isRouteWellFormed(route))		// Checks to see if route valid and starting position & ending position are valid. If not, return 2.
		return 2;

	nsteps = 0;
	string route2 = "";
	int totalStepsTaken = 0;

	char dir;
	int newRow = sr;
	int newCol = sc;

	for(int pos = 0; pos < route.size(); pos++)		// Rewrites route so that for directional characters without a number after them, a 1 is appended to that directional character
	{
		route2 += route[pos];
		if(isDirectionValid(route[pos]) && (pos + 1) < route.size() && isDirectionValid(route[pos + 1]))
			route2 += "1";
	}

	// special case: add a 1 if the last character is a direction character
	if (isDirectionValid(route[route.size() - 1])) {
		route2 += "1";
	}

	int size = route2.size();
	int pos = 0;

	while(pos < size)
	{
		string maxStepsStr = "";

		dir = tolower(route2[pos]);				// Takes first directional charcter of a segment

		int digits = 0;
		pos++;

		while(digits < 2 && pos < size)			// Takes next 2 digits & appends it to a string
		{
			if(isDirectionValid(route2[pos]))
				break;

			if(isdigit(route2[pos]))	
			{	
				maxStepsStr += route2[pos];
				digits++;
				pos++;
			}
		}

		int maxSteps = stoi(maxStepsStr);		// Converts string of digits to an int
		int stepsTaken = stepsTakenInDir(newRow, newCol, dir, maxSteps);
		totalStepsTaken += stepsTaken;
			
		if(stepsTaken != maxSteps)			// If robot runs into a wall or off the grid --> return 3
		{
			nsteps = totalStepsTaken;
			return 3;
		}
		
		if (tolower(dir) == 'n') newRow -= stepsTaken;		// Row and column changed according to how many steps taken in specified direction
		if (tolower(dir) == 's') newRow += stepsTaken;
		if (tolower(dir) == 'e') newCol += stepsTaken;
		if (tolower(dir) == 'w') newCol -= stepsTaken;
	}

	nsteps = totalStepsTaken;

	if (newRow == er && newCol == ec)			// Successfully navigated to end coordinates --> return 0
		return 0;
	else 										// Route successfully navigated but did not end at end coordinates --> return 1
		return 1;
}

bool isDirectionValid(char dir)		// Checks is letter character is a directional character
{
	char lowerDir = tolower(dir);
	if (lowerDir == 'n' || lowerDir == 's' || lowerDir == 'e' || lowerDir == 'w')
		return true;
	return false;
}

bool isLocationValid(int r, int c)		//Checks to see if specified location is on grid
{
	return (r >= 1 && c>= 1 && r <= getRows() && c <= getCols());
}

int stepsTakenInDir(int r, int c, char dir, int maxSteps)	// Determines how many steps taken in a certain direction
{
	int stepsTaken = 0;
	dir = tolower(dir);

	while(stepsTaken < maxSteps)
	{
		if (dir == 'n') r--;
		if (dir == 's') r++;
		if (dir == 'e') c++;
		if (dir == 'w') c--;
		if (!isLocationValid(r, c) || isWall(r, c)) return stepsTaken;
		stepsTaken++;
	}
	return stepsTaken;
}

