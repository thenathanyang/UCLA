#ifndef GRID_INCLUDED
#define GRID_INCLUDED

const int MAXROWS = 20;
const int MAXCOLS = 20;

void setSize(int nr, int nc);
int getRows();
int getCols();
void setWall(int r, int c);
bool isWall(int r, int c);
void draw(int rStart, int cStart, int rEnd, int cEnd);
void draw(int rStart, int cStart, int rEnd, int cEnd, int rRobot, int cRobot);

#endif // GRID_INCLUDED
