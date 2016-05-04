// mazestack.cpp		Problem 1

#include <stack>
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

	stack<Coord> coordStack;
	coordStack.push(Coord(sr, sc));
	maze[sr][sc] = '*';

	while (! coordStack.empty())
	{
		Coord top = coordStack.top();
		coordStack.pop();

		int top_r = top.r();	
		int top_c = top.c();

		if (top_r == er && top_c == ec)
			return true;

		if (top_r-1 >= 0 && top_r-1 < nRows && maze[top_r-1][top_c] == '.')		// North
		{
			coordStack.push(Coord(top_r-1, top_c));
			maze[top_r-1][top_c] = '*';
		}
		if (top_c+1 >= 0 && top_c+1 < nCols && maze[top_r][top_c+1] == '.')		// East
		{
			coordStack.push(Coord(top_r, top_c+1));
			maze[top_r][top_c+1] = '*';
		} 
		if (top_r+1 >= 0 && top_r+1 < nRows && maze[top_r+1][top_c] == '.')		// South
		{
			coordStack.push(Coord(top_r+1, top_c));
			maze[top_r+1][top_c] = '*';
		}
		if (top_c-1 >= 0 && top_c-1 < nCols && maze[top_r][top_c-1] == '.')		// West
		{
			coordStack.push(Coord(top_r, top_c-1));
			maze[top_r][top_c-1] = '*';
		}
	}
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
