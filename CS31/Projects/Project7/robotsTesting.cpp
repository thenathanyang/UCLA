// robots.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;


#include <type_traits>
#include <cassert>

#define CHECKTYPE(c, f, r, a)  \
  static_assert(std::is_same<decltype(&c::f), r (c::*)a>::value, \
     "FAILED: You changed the type of " #c "::" #f);  \
  [[gnu::unused]] r (c::* xxx##c##_##f) a = &c::f



///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;             // max number of rows in the arena
const int MAXCOLS = 30;             // max number of columns in the arena
const int MAXROBOTS = 100;          // max number of robots allowed

const int UP    = 0;
const int DOWN  = 1;
const int LEFT  = 2;
const int RIGHT = 3;

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir);
int randInt(int min, int max);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
              // type name, since it's mentioned in the Robot declaration.

class Robot
{
  public:
        // Constructor
    Robot(Arena* ap, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;

        // Mutators
    void move();
    bool getAttacked(int dir);

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_hitOnce;
      // DONE: You'll probably find that a robot object needs an additional
      // data member to support your implementation of the behavior affected
      // by being hit and taking damage.
};

class Player
{
  public:
        // Constructor
    Player(Arena *ap, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    bool isDead() const;

        // Mutators
    void   stand();
    void   moveOrAttack(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_age;
    bool   m_dead;
};

class Arena
{
  public:
        // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

        // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     robotCount() const;
    int     nRobotsAt(int r, int c) const;
    bool    determineNewPosition(int& r, int& c, int dir) const;
    void    display() const;

        // Mutators
    bool   addRobot(int r, int c);
    bool   addPlayer(int r, int c);
    bool   attackRobotAt(int r, int c, int dir);
    bool   moveRobots();

  private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Robot*  m_robots[MAXROBOTS];
    int     m_nRobots;
};

class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nRobots);
    ~Game();

        // Mutators
    void play();

  private:
    Arena* m_arena;
};

///////////////////////////////////////////////////////////////////////////
//  Robot implementation
///////////////////////////////////////////////////////////////////////////

Robot::Robot(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A robot must be in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Robot created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_hitOnce = false;
}

int Robot::row() const
{
    return m_row;
}

int Robot::col() const
{
      // DONE: TRIVIAL:  Return what column the robot is at.

    return m_col;
}

void Robot::move()
{
      // Attempt to move in a random direction; if we can't move, don't move
    int dir = randInt(0, 3);  // dir is now UP, DOWN, LEFT, or RIGHT

      // DONE:  Attempt to move in direction dir; if we can't move, don't move.
    m_arena->determineNewPosition(m_row, m_col, dir);
}

bool Robot::getAttacked(int dir)  // return true if dies
{
      // DONE:  If the robot has been attacked once before, return true
      // (since a second attack kills a robot).  Otherwise, if possible, move
      // the robot in one position in direction dir and return false (since
      // it survived the damage).  Otherwise, do not move, but return true
      // (since the momentum from the blow would bump the robot against the
      // wall, dealing it additional fatal damage).
    
    if (m_hitOnce == true)
        return true;

    int temp_row = m_row;
    int temp_col = m_col; 

    if(m_arena->determineNewPosition(temp_row, temp_col, dir) == false)
        return true;

    m_arena->determineNewPosition(m_row, m_col, dir);
    m_hitOnce = true;
    return false;
}

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const
{
      // DONE: TRIVIAL:  Return what row the player is at.

    return m_row;
}

int Player::col() const
{
      // DONE: TRIVIAL:  Return what column the player is at.

    return m_col;
}

int Player::age() const
{
      // DONE:  TRIVIAL:  Return the player's age.

    return m_age;
}

void Player::stand()
{
    m_age++;
}

void Player::moveOrAttack(int dir)
{
    m_age++;
      // DONE:  If there is a robot adjacent to the player in the direction
      // dir, attack it.  Otherwise, move the player to that position if
      // possible (i.e., if the move would not be off the edge of the arena).

    int new_row = m_row;
    int new_col = m_col;

    m_arena->determineNewPosition(new_row, new_col, dir);

    if (m_arena->nRobotsAt(new_row, new_col) > 0)
    {
        m_arena->attackRobotAt(new_row, new_col, dir);
        return;
    }

    m_row = new_row;
    m_col = new_col;
}

bool Player::isDead() const
{
      // DONE:  TRIVIAL:  Return whether the player is dead.

    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementations
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
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
      // DONE:  Delete the player and all remaining dynamically allocated robots.
    delete m_player;
    for (int k = 0; k < m_nRobots; k++)
        delete m_robots[k];
}

int Arena::rows() const
{
      // DONE:  TRIVIAL:  Return the number of rows in the arena.

    return m_rows;
}

int Arena::cols() const
{
      // DONE:  TRIVIAL:  Return the number of columns in the arena.

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

int Arena::nRobotsAt(int r, int c) const
{
    // DONE:  Return the number of robots at row r, column c.
    
    int numAtSamePos = 0;
    for (int k = 0; k < m_nRobots; k++)
        if (m_robots[k]->row() == r && m_robots[k]->col() == c)
            numAtSamePos++;
    return numAtSamePos;

}

bool Arena::determineNewPosition(int& r, int& c, int dir) const
{
      // DONE:  If a move from row r, column c, one step in direction dir
      // would go off the edge of the arena, leave r and c unchanged and
      // return false.  Otherwise, set r or c so that row r, column c, is
      // now the new position resulting from the proposed move, and
      // return true. 
    switch (dir)
    {
      case UP:         
        if (r <= 1)
            return false;
        r--;
        break;
      case DOWN:   
        if (r >= rows())
            return false;
        r++;
        break;
      case LEFT:   
        if (c <= 1)
            return false;
        c--;
        break;
      case RIGHT:   
        if (c >= cols())
            return false;
        c++;
        break;
      default:
        return false;
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
      // DONE:  If one robot is at some grid point, set the char to 'R'.
      //        If it's 2 though 8, set it to '2' through '8'.
      //        For 9 or more, set it to '9'.

    if (m_nRobots > 0)
        for (r = 0; r < rows(); r++)
        {
            for (c = 0; c < cols(); c++)
            {
                if (nRobotsAt(r+1, c+1) > 1) 
                {
                    int numRobots = nRobotsAt(r+1, c+1);
                    if (numRobots < 9)
                        grid[r][c] = '0' + numRobots; 
                    else
                        grid[r][c] = '9';
                }
                else if (nRobotsAt(r+1, c+1) == 1)
                {
                    grid[r][c] = 'R';
                }
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
      // If MAXROBOTS have already been added, return false.  Otherwise,
      // dynamically allocate a new robot at coordinates (r,c).  Save the
      // pointer to the newly allocated robot and return true.

      // DONE:  Implement this.
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
      // DONE:  Attack one robot at row r, column c if at least one is at
      // that position.  If the robot does not survive the damage, destroy the
      // robot object, removing it from the arena, and return true.  Otherwise,
      // return false (no robot at (r,c), or robot didn't die).

    if (nRobotsAt(r, c) == 0)
        return false;

    for (int k = 0; k < m_nRobots; k++)
        if (m_robots[k]->row() == r && m_robots[k]->col() == c)
        {
            if(m_robots[k]->getAttacked(dir) == true)
            {
                delete m_robots[k];
                for (int i = k; i < m_nRobots - 1; i++)
                    m_robots[i] = m_robots[i+1]; 
                m_nRobots--;
                return true;
            }

            m_robots[k]->getAttacked(dir);
            return false;
        }
    return false;
}

bool Arena::moveRobots()
{
    for (int k = 0; k < m_nRobots; k++)
    {
      // DONE:  Have the k-th robot in the arena make one move.
      //        If that move results in that robot being in the same
      //        position as the player, the player dies.

        m_robots[k]->move();
        if(m_robots[k]->row() == m_player->row() && m_robots[k]->col() == m_player->col())
            m_player->setDead();

    }

      // return true if the player is still alive, false otherwise
    return ! m_player->isDead();
}

///////////////////////////////////////////////////////////////////////////
//  Game implementations
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRobots)
{
    if (nRobots < 0)
    {
        cout << "***** Cannot create Game with negative number of robots!" << endl;
        exit(1);
    }
    if (nRobots > MAXROBOTS)
    {
        cout << "***** Trying to create Game with " << nRobots
             << " robots; only " << MAXROBOTS << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1  &&  cols == 1  &&  nRobots > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the robots!" << endl;
        exit(1);
    }

        // Create arena
    m_arena = new Arena(rows, cols);

        // Add player
    int rPlayer = randInt(1, rows);
    int cPlayer = randInt(1, cols);
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with robots
    while (nRobots > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
          // Don't put a robot where the player is
        if (r == rPlayer  &&  c == cPlayer)
            continue;
        m_arena->addRobot(r, c);
        nRobots--;
    }
}

Game::~Game()
{
    delete m_arena;
}

void Game::play()
{
    Player* p = m_arena->player();
    if (p == nullptr)
    {
        m_arena->display();
        return;
    }
    do
    {
        m_arena->display();
        cout << endl;
        cout << "Move (u/d/l/r//q): ";
        string action;
        getline(cin,action);
        if (action.size() == 0)  // player stands
            p->stand();
        else
        {
            switch (action[0])
            {
              default:   // if bad move, nobody moves
                cout << '\a' << endl;  // beep
                continue;
              case 'q':
                return;
              case 'u':
              case 'd':
              case 'l':
              case 'r':
                p->moveOrAttack(decodeDirection(action[0]));
                break;
            }
        }
        m_arena->moveRobots();
    } while ( ! m_arena->player()->isDead()  &&  m_arena->robotCount() > 0);
    m_arena->display();
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir)
{
    switch (dir)
    {
      case 'u':  return UP;
      case 'd':  return DOWN;
      case 'l':  return LEFT;
      case 'r':  return RIGHT;
    }
    return -1;  // bad argument passed in!
}

  // Return a random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static mt19937 generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

void thisFunctionWillNeverBeCalled()
{
      // If the student deleted or changed the interfaces to the public
      // functions, this won't compile.  (This uses magic beyond the scope
      // of CS 31.)

    Robot(static_cast<Arena*>(0), 1, 1);
    CHECKTYPE(Robot, row, int, () const);
    CHECKTYPE(Robot, col, int, () const);
    CHECKTYPE(Robot, move, void, ());
    CHECKTYPE(Robot, getAttacked, bool, (int));

    Player(static_cast<Arena*>(0), 1, 1);
    CHECKTYPE(Player, row, int, () const);
    CHECKTYPE(Player, col, int, () const);
    CHECKTYPE(Player, age, int, () const);
    CHECKTYPE(Player, isDead, bool, () const);
    CHECKTYPE(Player, stand, void, ());
    CHECKTYPE(Player, moveOrAttack, void, (int));
    CHECKTYPE(Player, setDead, void, ());

    Arena(1, 1);
    CHECKTYPE(Arena, rows, int, () const);
    CHECKTYPE(Arena, cols, int, () const);
    CHECKTYPE(Arena, player, Player*, () const);
    CHECKTYPE(Arena, robotCount, int, () const);
    CHECKTYPE(Arena, nRobotsAt, int, (int, int) const);
    CHECKTYPE(Arena, determineNewPosition, bool, (int&, int&, int) const);
    CHECKTYPE(Arena, display, void, () const);
    CHECKTYPE(Arena, addRobot, bool, (int, int));
    CHECKTYPE(Arena, addPlayer, bool, (int, int));
    CHECKTYPE(Arena, attackRobotAt, bool, (int, int, int));
    CHECKTYPE(Arena, moveRobots, bool, ());

    Game(1,1,1);
    CHECKTYPE(Game, play, void, ());
}

void doBasicTests()
{
    {
        Arena a(10, 20);
        assert(a.addPlayer(2, 6));
        Player* pp = a.player();
        assert(pp->row() == 2  &&  pp->col() == 6  && ! pp->isDead());
        pp->moveOrAttack(UP);
        assert(pp->row() == 1  &&  pp->col() == 6  && ! pp->isDead());
        pp->moveOrAttack(UP);
        assert(pp->row() == 1  &&  pp->col() == 6  && ! pp->isDead());
        pp->setDead();
        assert(pp->row() == 1  &&  pp->col() == 6  && pp->isDead());
    }
    {
        Arena a(2, 2);
        assert(a.addPlayer(1, 1));
        assert(a.addRobot(2, 2));
        Player* pp = a.player();
        assert(a.moveRobots());
  assert( ! pp->isDead());
        for (int k = 0; k < 1000  &&  ! pp->isDead()  &&  a.moveRobots(); k++)
            ;
  assert(pp->isDead());
    }
    {
        Arena a(2, 6);
        assert(a.addPlayer(2, 1));
        assert(a.addRobot(2, 3));
        Player* pp = a.player();
  pp->moveOrAttack(RIGHT);
        assert(a.robotCount() == 1  &&  a.nRobotsAt(2, 3) == 1);
  pp->moveOrAttack(RIGHT);
        assert(a.robotCount() == 1  &&  a.nRobotsAt(2, 4) == 1);
  pp->moveOrAttack(RIGHT);
        assert(a.robotCount() == 1  &&  a.nRobotsAt(2, 4) == 1);
  pp->moveOrAttack(RIGHT);
        assert(a.robotCount() == 0  &&  a.nRobotsAt(2, 4) == 0   &&  a.nRobotsAt(2, 5) == 0);
  a.addRobot(1, 3);
        assert(a.robotCount() == 1  &&  a.nRobotsAt(1, 3) == 1);
  pp->moveOrAttack(UP);
        assert(a.robotCount() == 0  &&  a.nRobotsAt(1, 3) == 0);
    }
    cout << "Passed all basic tests" << endl;
}


int main()
{
  doBasicTests(); // Remove this line after completing test.
  return 0;       // Remove this line after completing test.
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);

      // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++ UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
