// mazestack.cpp		Problem 3

#include <queue>
#include <string>
#include <iostream>
using namespace std;

class Coord
{
  public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
  private:
    int m_r;
    int m_c;
};


bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    // Return true if there is a path from (sr,sc) to (er,ec)
    // through the maze; return false otherwise

	queue<Coord> coordStack;
	coordStack.push(Coord(sr, sc));
	maze[sr][sc] = '*';

	while (! coordStack.empty())
	{
		Coord front = coordStack.front();
		coordStack.pop();

		int front_r = front.r();
		int front_c = front.c();

		if (front_r == er && front_c == ec)
			return true;

		if (front_r-1 >= 0 && front_r-1 < nRows && maze[front_r-1][front_c] == '.')		// North
		{
			coordStack.push(Coord(front_r-1, front_c));
			maze[front_r-1][front_c] = '*';
		}
		if (front_c+1 >= 0 && front_c+1 < nCols && maze[front_r][front_c+1] == '.')		// East
		{
			coordStack.push(Coord(front_r, front_c+1));
			maze[front_r][front_c+1] = '*';
		} 
		if (front_r+1 >= 0 && front_r+1 < nRows && maze[front_r+1][front_c] == '.')		// South
		{
			coordStack.push(Coord(front_r+1, front_c));
			maze[front_r+1][front_c] = '*';
		}
		if (front_c-1 >= 0 && front_c-1 < nCols && maze[front_r][front_c-1] == '.')		// West
		{
			coordStack.push(Coord(front_r, front_c-1));
			maze[front_r][front_c-1] = '*';
		}
	}
	return false;
}
