// maze.cpp

#include <iostream>
using namespace std;

// If the start location is equal to the ending location, then we've
//     solved the maze, so return true.
// Mark the start location as visted.
// For each of the four directions,
//     If the location one step in that direction (from the start location) is unvisited,
// 	    then call pathExists starting from that location (and
// 			ending at the same ending location as in the current call).
// 		 If that returned true,
// 		     then return true.
// Return false.

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    // Return true if there is a path from (sr,sc) to (er,ec)
    // through the maze; return false otherwise

	if (sr == er && sc == ec)			// start location is equal to end location
		return true;

	maze[sr][sc] = '*';

	if (sr-1 >= 0 && sr-1 < nRows && maze[sr-1][sc] == '.')		// North
		if (pathExists(maze, nRows, nCols, sr-1, sc, er, ec))
			return true;
	if (sc+1 >= 0 && sc+1 < nRows && maze[sr][sc+1] == '.')		// East
		if (pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
			return true;
	if (sr+1 >= 0 && sr+1 < nRows && maze[sr+1][sc] == '.')		// South
		if (pathExists(maze, nRows, nCols, sr+1, sc, er, ec))
			return true;
	if (sc-1 >= 0 && sc-1 < nRows && maze[sr][sc-1] == '.')		// West
		if (pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
			return true;

	return false;
}


int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };

    if (pathExists(maze, 10,10, 6,4, 1,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
