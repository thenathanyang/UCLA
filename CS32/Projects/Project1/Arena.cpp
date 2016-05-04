// Arena.cpp

///////////////////////////////////////////////////////////////////////////
//  Arena implementations
///////////////////////////////////////////////////////////////////////////

#include "globals.h"
#include "Arena.h"
#include "Player.h"
#include "Robot.h"
#include "History.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Arena::Arena(int nRows, int nCols)
    : m_history(nRows, nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nRobots = 0;
}

Arena::~Arena()
{
    for (int k = 0; k < m_nRobots; k++)
        delete m_robots[k];
    delete m_player;
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::robotCount() const
{
    return m_nRobots;
}

History& Arena::history()
{
    return m_history;
}


int Arena::nRobotsAt(int r, int c) const
{
    int count = 0;
    for (int k = 0; k < m_nRobots; k++)
    {
        const Robot* rp = m_robots[k];
        if (rp->row() == r  &&  rp->col() == c)
            count++;
    }
    return count;
}

bool Arena::determineNewPosition(int& r, int& c, int dir) const
{
    switch (dir)
    {
      case UP:     if (r <= 1)      return false; else r--; break;
      case DOWN:   if (r >= rows()) return false; else r++; break;
      case LEFT:   if (c <= 1)      return false; else c--; break;
      case RIGHT:  if (c >= cols()) return false; else c++; break;
      default:     return false;
    }
    return true;
}

void Arena::display() const
{
      // Position (row,col) in the arena coordinate system is represented in
      // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    
        // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';

        // Indicate each robot's position
    for (int k = 0; k < m_nRobots; k++)
    {
        const Robot* rp = m_robots[k];
        char& gridChar = grid[rp->row()-1][rp->col()-1];
        switch (gridChar)
        {
          case '.':  gridChar = 'R'; break;
          case 'R':  gridChar = '2'; break;
          case '9':  break;
          default:   gridChar++; break;  // '2' through '8'
        }
    }

        // Indicate player's position
    if (m_player != nullptr)
    {
          // Set the char to '@', unless there's also a robot there,
          // in which case set it to '*'.
        char& gridChar = grid[m_player->row()-1][m_player->col()-1];
        if (gridChar == '.')
            gridChar = '@';
        else
            gridChar = '*';
    }

        // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

        // Write message, robot, and player info
    cout << endl;
    cout << "There are " << robotCount() << " robots remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool Arena::addRobot(int r, int c)
{
      // Dynamically allocate a new Robot and add it to the arena
    if (m_nRobots == MAXROBOTS)
        return false;
    m_robots[m_nRobots] = new Robot(this, r, c);
    m_nRobots++;
    return true;
}

bool Arena::addPlayer(int r, int c)
{
      // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

      // Dynamically allocate a new Player and add it to the arena
    m_player = new Player(this, r, c);
    return true;
}

bool Arena::attackRobotAt(int r, int c, int dir)
{
      // Attack one robot.  Returns true if a robot was attacked and destroyed,
      // false otherwise (no robot there, or the attack did not destroy the
      // robot).
    int k = 0;
    for ( ; k < m_nRobots; k++)
    {
        if (m_robots[k]->row() == r  &&  m_robots[k]->col() == c)
            break;
    }
    if (k < m_nRobots  &&  m_robots[k]->getAttacked(dir))  // robot dies
    {
        delete m_robots[k];
        m_robots[k] = m_robots[m_nRobots-1];
        m_nRobots--;
        m_history.record(r, c);
        return true;
    }
    return false;
}

bool Arena::moveRobots()
{
    for (int k = 0; k < m_nRobots; k++)
    {
        Robot* rp = m_robots[k];
        rp->move();
        if (rp->row() == m_player->row()  &&  rp->col() == m_player->col())
            m_player->setDead();
    }

      // return true if the player is still alive, false otherwise
    return ! m_player->isDead();
}
