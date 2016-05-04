#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <list>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Squirt;
struct Path;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	~StudentWorld();

	virtual int init();
	// {
	// 	//return GWSTATUS_CONTINUE_GAME;
	// }

	virtual int move();
	// {
	// 	  // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// 	  // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	// 	//decLives();
	// 	//return GWSTATUS_CONTINUE_GAME;
	// }

	virtual void cleanUp();

	template<typename T> bool isObjThere(int x, int y) const;
	template<typename T> double nearestObjDist(int x, int y);
    template<typename T> void illuminateAroundFrackMan();
    template<typename T> void getAllTypeOf(int x, int y, double radius, vector<T*>& v);
    template<typename T> T* getTypeOf(int x, int y, double radius);
    template<typename T> T* getNearestObj(int x, int y);

	FrackMan* getFrackMan() const;
	bool newPosValid(int& x, int& y, int moveBy, GraphObject::Direction);
	bool isInBounds(int x, int y) const;
	bool isDirtHere(int x, int y) const;
	bool removeDirt(int x, int y);
	void insertSquirt(Squirt* squirtObj);
	void decNumBarrels();
	void insertGoldNugget(GoldNugget* goldObj);
	GraphObject::Direction getRandomDir() const;
	void getBestPath(int initX, int initY, int destX, int destY, Path& finalPath);
	bool isDirtInDir(int x, int y, GraphObject::Direction dir) const;
	bool isDirBlocked(int x, int y, GraphObject::Direction dir) const;
	bool isBoulderInPath(int x, int y);

private:
	void setDisplayText();
	bool isDirtIn4x4(int x, int y) const;
	int m_numBarrels;
	int m_numProtesters;
	int m_ticksSinceProtesterAdded;
	vector<Actor*> m_actor;
	vector<vector<Dirt*>> m_dirtObjs;
	FrackMan* m_FrackManPlayer;

};

struct Coord
{
    Coord(int x, int y) : xCoord(x), yCoord(y) {}
  	int xCoord;
    int yCoord;
};

struct Path
{
  public:
  	Path() {}
    Path(const Coord& x) { pathList.push_back(x); }
    void pushIntoPath(Coord& x) { pathList.push_back(x); }
    Coord nextCoordInPath()
    {
    	if (!pathList.empty())
    	{
	        Coord nextCoord = pathList.back();
	        pathList.pop_back();
	    	return nextCoord;
    	}
    	return Coord(-1, -1);
    }

    int getPathSize() const { return pathList.size(); }

  private:
    list<Coord> pathList;
};

inline double distBetween(int x1, int y1, int x2, int y2)		// Calculates the distance between (x1, y1) and (x2, y2)
{
	return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));
}

template<typename T>
bool StudentWorld::isObjThere(int x, int y) const		// Checks to see if there is an object at (x, y)
{
	for (vector<Actor*>::const_iterator k = m_actor.begin(); k != m_actor.end(); ++k)
		if (dynamic_cast<T*>(*k) && x == (*k)->getX() && y == (*k)->getY())
			return true;
	return false;
}

template<typename T>
double StudentWorld::nearestObjDist(int x, int y)		// Returns the distance to the closest object
{
	double minDist = 1000;
	for (vector<Actor*>::iterator k = m_actor.begin(); k != m_actor.end(); ++k)
		if (dynamic_cast<T*>(*k))
			minDist = min(minDist, distBetween((*k)->getX(), (*k)->getY(), x , y));
	return minDist;
}

template<> inline										// Returns an object's distance from FrackMan
double StudentWorld::nearestObjDist<FrackMan>(int x, int y)
{
	return distBetween(m_FrackManPlayer->getX(), m_FrackManPlayer->getY(), x, y);
}

template<typename T>
void StudentWorld::illuminateAroundFrackMan()
{
	for (vector<Actor*>::iterator k = m_actor.begin(); k != m_actor.end(); ++k)
		if (dynamic_cast<T*>(*k) && distBetween((*k)->getX(), (*k)->getY(), m_FrackManPlayer->getX(), m_FrackManPlayer->getY()) <= 12.0)
			(*k)->setVisible(true);
}

template<typename T>
void StudentWorld::getAllTypeOf(int x, int y, double radius, vector<T*>& v)
{
	for (vector<Actor*>::iterator k = m_actor.begin(); k != m_actor.end(); ++k)
		if (dynamic_cast<T*>(*k) && distBetween(x, y, (*k)->getX(), (*k)->getY()) <= radius)
		{
			//cout << "Pushing back" << endl;
			v.push_back(dynamic_cast<T*>(*k));		
		}
}

template<typename T>
T* StudentWorld::getTypeOf(int x, int y, double radius)
{
	for (vector<Actor*>::iterator k = m_actor.begin(); k != m_actor.end(); ++k)
		if (dynamic_cast<T*>(*k) && distBetween(x, y, (*k)->getX(), (*k)->getY()) <= radius)
			return (dynamic_cast<T*>(*k));
	return nullptr;	
}

template<typename T>
T* StudentWorld::getNearestObj(int x, int y)
{
	double nearestDist = 10000;
	T* currentNearest = nullptr;
	for (vector<Actor*>::iterator k = m_actor.begin(); k != m_actor.end(); ++k)
		if (dynamic_cast<T*>(*k) && distBetween(x, y, (*k)->getX(), (*k)->getY()) <= nearestDist)
		{
			nearestDist = distBetween(x, y, (*k)->getX(), (*k)->getY());
			currentNearest = (dynamic_cast<T*>(*k));
		}
	return currentNearest;	
}

string addZeroesToScore(int score);							// Formats the display of the score
string addSpacesToStat(int score, int numSpaces);			// Formats the display of the game statistics

#endif // STUDENTWORLD_H_
