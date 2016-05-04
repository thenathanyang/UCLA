#include "StudentWorld.h"
#include "GraphObject.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <ctime>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir)
 : GameWorld(assetDir), m_dirtObjs(64, vector<Dirt*>(64, nullptr))
{
	//srand(time(nullptr));
	srand(time(0));
	
	// m_dirtObjs = vector<vector<Dirt*>>(64, vector<Dirt*>(64));
}

StudentWorld::~StudentWorld() { cleanUp(); }

int StudentWorld::init()
{
	m_FrackManPlayer = new FrackMan(this);		// Inserts a new FrackMan into the world

	// Insert Dirt
	for (int x = 0; x < m_dirtObjs.size(); x++)
		for (int y = 0; y < m_dirtObjs[x].size(); y++)
			if (!(x >= 30 && x <= 33 && y >= 4 && y <= 59) && y <= 59)
				m_dirtObjs[x][y] = new Dirt(x, y, this);

	// Insert boulders, barrels, and gold nuggets
	int B = min(static_cast<int>(getLevel() / 2) + 2, 6);
	int L = min(2 + static_cast<int>(getLevel()), 20);
	int G = max(5 - static_cast<int>(getLevel() / 2), 2);
	m_ticksSinceProtesterAdded = max(25, 200 - (static_cast<int>(getLevel())));
	m_numProtesters = 0;
	m_numBarrels = L;
	// cout << "m_numBarrels: " << m_numBarrels << endl;
	while (B > 0 || L > 0 || G > 0)
	{
		int xRand = rand() % (60 + 1);					// 0 to 60
		int yRand = 20 + (rand() % (56 - 20 + 1));		// 20 to 56
		// cout << "---" << endl;
		// cout << "Try: " << xRand << " --- " << yRand << endl;
        if ((xRand >= 25 && xRand <= 37) || nearestObjDist<Actor>(xRand, yRand) <= 6.0)
            continue;

		if (B > 0)
		{
			
			Boulder* newBoulder = new Boulder(xRand, yRand, this);
			m_actor.push_back(newBoulder);
			// cout << "Boulder: " << xRand << " --- " << yRand << endl;
			
			for (int j = 0; j < 4; j++)
				for (int k = 0; k < 4; k++)
					if (isDirtHere(xRand+j,yRand+k))
						removeDirt(xRand+j,yRand+k);
			B--;
			continue;
		}

		if (L > 0)
		{
			Barrel* newBarrel = new Barrel(xRand, yRand, this);
			m_actor.push_back(newBarrel);
			// cout << "Barrel: " << xRand << " --- " << yRand << endl;
			L--;
			continue;
		}

		if (G > 0)
		{
			GoldNugget* newGoldNugget = new GoldNugget(xRand, yRand, GoldNugget::buried, this);
			m_actor.push_back(newGoldNugget);
			G--;
			continue;
		}
	}

	// Boulder* newBoulder1 = new Boulder(25, 40, this);
	// m_actor.push_back(newBoulder1);

	// for (int j = 0; j < 4; j++)
	// 	for (int k = 0; k < 4; k++)
	// 		if (isDirtHere(25+j,40+k))
	// 			removeDirt(25+j,40+k);

	// Boulder* newBoulder2 = new Boulder(25, 20, this);
	// m_actor.push_back(newBoulder2);

	// for (int j = 0; j < 4; j++)
	// 	for (int k = 0; k < 4; k++)
	// 		if (isDirtHere(25+j,20+k))
	// 			removeDirt(25+j,20+k);

	// TEMPORARY
	// HardcoreProtester* newHardcoreProtester = new HardcoreProtester(this);
	// m_actor.push_back(newHardcoreProtester);
	

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	if (m_numBarrels == 0)					// If no barrels left, level is finished
	{
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	if (!(m_FrackManPlayer->isAlive()))		// If FrackMan is not alive, decrease the number of lives
	{
		decLives();
		playSound(SOUND_PLAYER_GIVE_UP);
		return GWSTATUS_PLAYER_DIED;
	}

	setDisplayText();		// Displays game statistics

	int G = static_cast<int>(getLevel()) * 25 + 300;	
	int goodieChance = rand() % G;						// 0 to G-1
	//cout << "goodieChance: " << goodieChance << endl;
	if (goodieChance == 1)
	{
		// cout << "Chanced Achieved" << endl;
		int sonarOrWaterChance = rand() % 4;				// 0 to 4			
		// cout << "sonarOrWaterChance: " << sonarOrWaterChance << endl;
		if (sonarOrWaterChance == 1)
		{
			SonarKit* newSonarKit = new SonarKit(0, 60, this);
			m_actor.push_back(newSonarKit);
		}
		else
		{
			bool insertedWaterPool = false;
			while (insertedWaterPool == false)
			{
				bool validPos = true;
				int xRand = rand() % (60 + 1);		// 0 to 60
				int yRand = rand() % (60 + 1);		// 0 to 60
				//cout << xRand << " :=====: " << yRand << endl;

				for (int x = xRand; x < (xRand + 4); x++)
					for (int y = yRand; y < (yRand + 4); y++)
						if (isDirtHere(x, y))
							validPos = false;

				if (validPos == false) continue;

				WaterPool* newWaterPool = new WaterPool(xRand, yRand, this);
				m_actor.push_back(newWaterPool);
				insertedWaterPool = true;
			}
		}
	}


	int T = max(25, 200 - static_cast<int>(getLevel()));
	int P = min(15, 2 + (static_cast<int>(getLevel() * 1.5)));
	int probabilityOfHardcore = min(90, (static_cast<int>(getLevel()) * 10) + 30);		// CORRECT?!?!
	int hardcoreChance = rand() % probabilityOfHardcore; 	// 0 to probabilityOfHardcore	
	//cout << "P: " << P << " m_numProtesters: " << m_numProtesters << endl;
	m_ticksSinceProtesterAdded++;
	if (m_ticksSinceProtesterAdded >= T && m_numProtesters < P)
	{
		if (hardcoreChance == 1)
		{
			HardcoreProtester* newHardcoreProtester = new HardcoreProtester(this);
			m_actor.push_back(newHardcoreProtester);
		}
		else
		{
			Protester* newProtester = new Protester(this);
			m_actor.push_back(newProtester);
		}

		m_ticksSinceProtesterAdded = 0;
		m_numProtesters++;
	}

	m_FrackManPlayer->doSomething();
	for (vector<Actor*>::iterator x = m_actor.begin(); x != m_actor.end(); ++x)			// Calls each actor's doSomething()
	{
		(*x)->doSomething();
		if (!(*x)->isAlive())				// If an actor is not alive, remove it form the world
		{
			if ((*x)->isProtester() == true)
				m_numProtesters--;	
			delete *x;
			x = --m_actor.erase(x);
			
		}
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (vector<Actor*>::iterator x = m_actor.begin(); x != m_actor.end(); ++x)		// Deletes all the actors
	{
		delete *x;
		x = --m_actor.erase(x);
	}

	for (vector<vector<Dirt*>>::iterator i = m_dirtObjs.begin(); i != m_dirtObjs.end(); ++i)		// Deletes all the Dirt objects
		for (vector<Dirt*>::iterator j = (*i).begin(); j != (*i).end(); ++j)
			delete *j;	
    
    delete m_FrackManPlayer;		// Deletes FrackMan
}

FrackMan* StudentWorld::getFrackMan() const
{
	return m_FrackManPlayer;
}

void StudentWorld::setDisplayText()		// Formats the display of the game statistics
{
	string score = addZeroesToScore(getScore());
	string level = addSpacesToStat(getLevel(), 2);
	string lives = to_string(getLives());
	string healthPercent = addSpacesToStat(m_FrackManPlayer->getHitPoints() * 10, 3) + "%";
	string squirts = addSpacesToStat(m_FrackManPlayer->getSquirtsLeftInSquirtGun(), 2);
	string gold = addSpacesToStat(m_FrackManPlayer->getPlayerGoldCount(), 2);
	string sonar = addSpacesToStat(m_FrackManPlayer->getPlayerSonarChargeCount(), 2);
	string barrelsLeft = addSpacesToStat(m_numBarrels, 2);

	string stats = "Scr: " + score + "  Lvl: " + level + "  Lives: " + lives + "  Hlth: " + healthPercent + "  Wtr: " + squirts + "  Gld: " + gold + "  Sonar: " + sonar + "  Oil Left: " + barrelsLeft;
	setGameStatText(stats);
}

bool StudentWorld::newPosValid(int& x, int& y, int moveBy, GraphObject::Direction dir)		// Determines if a new position is valid and updates x and y
{
	if (dir == GraphObject::left) x -= moveBy;
	if (dir == GraphObject::right) x += moveBy;
	if (dir == GraphObject::up) y += moveBy;
	if (dir == GraphObject::down) y -= moveBy;
	return isInBounds(x, y);
}

bool StudentWorld::isInBounds(int x, int y) const		// Determines whether an object of size 1 is in bounds
{
	if (x >= 0 && x <= 60 && y >= 0 && y <= 60)			
		return true;
	return false;
}

bool StudentWorld::isDirtHere(int x, int y) const 		// Checks to see if there is Dirt at a specific location
{
    if (x >= 0 && x < m_dirtObjs.size() && y >= 0 && y < m_dirtObjs[x].size() && m_dirtObjs[x][y]) return true;
	return false;
}

bool StudentWorld::removeDirt(int x, int y)				// Removes Dirt at a certain location
{
	if (isDirtHere(x,y))	
	{
		delete m_dirtObjs[x][y];
		m_dirtObjs[x][y] = nullptr;
		return true;
	}
	return false;
}

void StudentWorld::insertSquirt(Squirt* squirtObj)		// Inserts a squirt object into the actor vector
{
	m_actor.push_back(squirtObj);
}


void StudentWorld::decNumBarrels()		// Decreases the number of barrels by 1
{
	m_numBarrels--;
}

void StudentWorld::insertGoldNugget(GoldNugget* goldObj)	// Inserts a gold nugget object into the actor vector
{
	m_actor.push_back(goldObj);
}

GraphObject::Direction StudentWorld::getRandomDir() const
{
	int randNum = rand() % 4;
	switch (randNum)
	{
		case 0: return GraphObject::left;
		case 1: return GraphObject::right;
		case 2: return GraphObject::up;
		default: return GraphObject::down;
	}
}

string addZeroesToScore(int score)		// Formats the display of the score
{
	string scoreString = to_string(score);
	while (scoreString.size() != 6)
		scoreString = "0" + scoreString;
	return scoreString;
}

string addSpacesToStat(int value, int length)	// Formats the display of the game statistics
{
	string statValue = to_string(value);
	while (statValue.size() != length)	
		statValue = " " + statValue;
	return statValue;
}

bool StudentWorld::isDirtInDir(int x, int y, GraphObject::Direction dir) const
{
	// for (int k = 0; k < 4; k++)
	// 	if (!isInBounds(x+k, y+k))
	// 		return true;

	for (int k = 0; k < 4; k++)
	{
		if (dir == GraphObject::left && isDirtHere(x-1, y+k)) return true;
		if (dir == GraphObject::right && isDirtHere(x+4, y+k)) return true;
		if (dir == GraphObject::up && isDirtHere(x+k, y+4)) return true;
		if (dir == GraphObject::down && isDirtHere(x+k, y-1)) return true;
	}
	return false;
}

bool StudentWorld::isDirBlocked(int x, int y, GraphObject::Direction dir) const
{
	for (int k = 0; k < 4; k++)
	{
		if (dir == GraphObject::left)
		{
			if (!isInBounds(x-1, y) || isDirtInDir(x, y, GraphObject::left) || isObjThere<Boulder>(x-1,y+k))	// isDirtHere(x-1, y+k)
			{	
				//cout << "left: " << isInBounds(x-1, y+k) << endl;
				return true;
			}
		}
		 if (dir == GraphObject::right)
		 {
	 		if (!isInBounds(x+4, y) || isDirtInDir(x, y, GraphObject::right) || isObjThere<Boulder>(x+4,y+k))		// isDirtHere(x+4, y+k)
	 		{
	 			//cout << "right: " << isInBounds(x+4, y+k) << endl;
	 			return true;
	 		}
		 }
		 if (dir == GraphObject::up)
		 {
	 		if (!isInBounds(x, y+4) || isDirtInDir(x, y, GraphObject::up) || isObjThere<Boulder>(x+k,y+4))		// isDirtHere(x+k, y+4)
	 		{
	 			//cout << "up: " << isInBounds(x+k, y+4) << endl;
	 			return true;
	 		}
		 }
		 if (dir == GraphObject::down)
		 {
	 		if (!isInBounds(x, y-1) || isDirtInDir(x, y, GraphObject::down) || isObjThere<Boulder>(x+k,y-1))		// isDirtHere(x+k, y-1)	
	 		{
	 			//cout << "down: " << isInBounds(x+k, y-1) << endl;
	 			return true;
	 		}
		}
	}
	return false;
}



void StudentWorld::getBestPath(int initX, int initY, int destX, int destY, Path& finalPath)
{
	static const int U = 1000000;

	int m_Grid[61][61];
	for (int k = 0; k < 61; k++)
        fill_n(m_Grid[k], 61, U);

	queue<Coord> pathQueue;
	pathQueue.push(Coord(initX, initY));
	Coord front = pathQueue.front();
	int frontX = front.xCoord;
	int frontY = front.yCoord;

	m_Grid[initX][initY] = 0;

    while (m_Grid[destX][destY] == U && !pathQueue.empty())
	{
		front = pathQueue.front();
		pathQueue.pop();
		frontX = front.xCoord;
		frontY = front.yCoord;

		// if (frontX == destX && frontY == destY)		// CHECK IF POSITION VISITED ALREADY?!?!?
		// 	break;

		if (frontY+1 <= 60 && isInBounds(frontX, frontY+1) && !isDirtIn4x4(frontX, frontY+1) && !isBoulderInPath(frontX, frontY+1) && m_Grid[frontX][frontY+1] == U)		// Up 		// isDirtHere
		{
			pathQueue.push(Coord(frontX, frontY+1));
			m_Grid[frontX][frontY+1] = m_Grid[frontX][frontY] + 1;
		}
		if (frontY-1 >= 0 && isInBounds(frontX, frontY-1) && !isDirtIn4x4(frontX, frontY-1) && !isBoulderInPath(frontX, frontY-1)  && m_Grid[frontX][frontY-1] == U)		// down		// !isObjThere<Boulder>(frontX, frontY-1)
		{
			pathQueue.push(Coord(frontX, frontY-1));
			m_Grid[frontX][frontY-1] = m_Grid[frontX][frontY] + 1;
		} 
		if (frontX-1 >= 0 && isInBounds(frontX-1, frontY) && !isDirtIn4x4(frontX-1, frontY) && !isBoulderInPath(frontX-1, frontY)  && m_Grid[frontX-1][frontY] == U)		// Left
		{
			pathQueue.push(Coord(frontX-1, frontY));
			m_Grid[frontX-1][frontY] = m_Grid[frontX][frontY] + 1;
		}
		if (frontX+1 <= 60 && isInBounds(frontX+1, frontY) && !isDirtIn4x4(frontX+1, frontY) && !isBoulderInPath(frontX+1, frontY)  && m_Grid[frontX+1][frontY] == U)		// Right
		{
			pathQueue.push(Coord(frontX+1, frontY));
			m_Grid[frontX+1][frontY] = m_Grid[frontX][frontY] + 1;
		}
	}

	// start from end, find way back to start, 
	// by visiting the position with the least distance from the start at every step

    Path temp;
	Coord pos = Coord(destX, destY);
    
    int x = pos.xCoord;
    int y = pos.yCoord;
	while (pos.xCoord != initX || pos.yCoord != initY)
	{
        temp.pushIntoPath(pos);
        x = pos.xCoord;
        y = pos.yCoord;
		int smallestValue = 100000;

		if (isInBounds(pos.xCoord-1, pos.yCoord) && m_Grid[pos.xCoord-1][pos.yCoord] < smallestValue) 
		{
			smallestValue = m_Grid[pos.xCoord-1][pos.yCoord];
			x = pos.xCoord-1;
			y = pos.yCoord;
                        //cout << "1:" << smallestValue << endl;
		}
		if (isInBounds(pos.xCoord+1, pos.yCoord) && m_Grid[pos.xCoord+1][pos.yCoord] < smallestValue) 
		{
			smallestValue = m_Grid[pos.xCoord+1][pos.yCoord];
			x = pos.xCoord+1;
			y = pos.yCoord;
                        //cout << "2: " << smallestValue << endl;
		}
		if (isInBounds(pos.xCoord, pos.yCoord-1) && m_Grid[pos.xCoord][pos.yCoord-1] < smallestValue) 
		{
			smallestValue = m_Grid[pos.xCoord][pos.yCoord-1];
			x = pos.xCoord;
			y = pos.yCoord-1;
                        //cout << "3: " << smallestValue << endl;
		}
		if (isInBounds(pos.xCoord, pos.yCoord+1) && m_Grid[pos.xCoord][pos.yCoord+1] < smallestValue)
		{
			smallestValue = m_Grid[pos.xCoord][pos.yCoord+1];
			x = pos.xCoord;
			y = pos.yCoord+1;
            //cout << "4: " << smallestValue << endl;
		}
        //cout << "final: " << smallestValue << endl;

		pos.xCoord = x;
		pos.yCoord = y;
        //cout << pos.xCoord << ", " << pos.yCoord << endl;

	}
    // temp.pushIntoPath(pos);
    finalPath = temp;
	//cout << "____________" << finalPath.getPathSize() << endl;
}


bool StudentWorld::isDirtIn4x4(int x, int y) const
{
	for (int k = 0; k < 4; k++)
		for (int j = 0; j < 4; j++)
			if (isDirtHere(x+k, y+j))
				return true;
	return false;
}

bool StudentWorld::isBoulderInPath(int x, int y)
{
	for (int k = -3; k < 4; k++)
		for (int j = -3; j < 4; j++)
			if (isObjThere<Boulder>(x+k,y+j) && !(getNearestObj<Boulder>(x+k, y+j)->isBoulderFalling()))
				return true;
	return false;
}







