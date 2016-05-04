#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

GraphObject::Direction keyToDir(int key)		// Converts which key pressed to Direction
{
	if (key == KEY_PRESS_LEFT)
		return GraphObject::Direction::left;
	else if(key == KEY_PRESS_RIGHT)
		return GraphObject::Direction::right;
	else if (key == KEY_PRESS_UP)
		return GraphObject::Direction::up;
	else if (key == KEY_PRESS_DOWN)
		return GraphObject::Direction::down;
	else
		return GraphObject::Direction::none;
}

int dirToKey(GraphObject::Direction d)			// Converts Direction to which key pressed
{
	if (d == GraphObject::Direction::left)
		return KEY_PRESS_LEFT;
	else if (d == GraphObject::Direction::right)
		return KEY_PRESS_RIGHT;
	else if (d == GraphObject::Direction::up)
		return KEY_PRESS_UP;
	else if (d == GraphObject::Direction::down)
		return KEY_PRESS_DOWN;
	else
		return 0;
}

Actor::Actor(StudentWorld* world, int imageID, int startX, int startY, unsigned int depth, Direction dir, double size)		
	: GraphObject(imageID, startX, startY, dir, size, depth), m_alive(true), m_world(world), m_hitPoints(10), m_numTicks(0)
{
		setVisible(true);
		setDirection(dir);
		moveTo(startX, startY);
}

Actor::~Actor()
{
	setVisible(false);		
}

bool Actor::isAlive() const 			// Returns whether or not actor is alive
{
	return m_alive;
}

void Actor::setDead()					// Set the actor to dead
{
	m_alive = false;
}

StudentWorld* Actor::getWorld() const 		
{
	return m_world;
}

int Actor::getHitPoints() const 		// Return hit points
{
	return m_hitPoints;
}

void Actor::annoyBy(int amt)			// Decreases hit points by a certain amount 
{
	m_hitPoints -= amt;
}

bool Actor::makeObjNearFrackManVisible()		// Makes any hidden objects near FrackMan visible
{
	if (!isVisible() && getWorld()->nearestObjDist<FrackMan>(getX(), getY()) <= 4.0)
	{
		setVisible(true);
		return true;
	}
	return false;
}

int Actor::getNumTicks() const 			// Returns the number of ticks passed if the number of ticks are being recorded
{
	return m_numTicks;
}

void Actor::incNumTicks()				// Increments the number of ticks passed for each tick
{
	m_numTicks++;
}

void Actor::setHitPoints(int amt)
{
	m_hitPoints = amt;
}

bool Actor::isProtester() const
{
	return false;
}

// void Actor::movePosBy(int x, int y)		// Moves actor by a specified amount
// {
// 	int j = getX();
// 	int k = getY();	

// 	if (getWorld()->isInBounds(j+x, k+y) && getWorld()->nearestObjDist<Boulder>(j + x, k + y) > 3.0)	
// 		moveTo(j+x, k+y);
// 	else 
// 		moveTo(j, k);
// }

//////////////////////////////////////////////////

FrackMan::FrackMan(StudentWorld* world)
	: Actor(world, IID_PLAYER, 30, 60), m_squirts(50), m_sonarCharges(1), m_goldNuggets(10)		// CHANGE
{}

FrackMan::~FrackMan()
{}

void FrackMan::doSomething()
{
	if (!isAlive()) return;
	if (getHitPoints() <= 0) setDead(); //makeFrackManDie();

	dig();

	int key = 0;
	if (getWorld()->getKey(key))		// An action is performed depending in which key is pressed
	{
		if (key == KEY_PRESS_LEFT || key == KEY_PRESS_RIGHT || key == KEY_PRESS_UP || key == KEY_PRESS_DOWN) moveFrackMan(key);
		if (key == KEY_PRESS_SPACE) squirtAction();
		if (key == KEY_PRESS_ESCAPE) setDead();
		if (key == KEY_PRESS_TAB) dropGoldNugget();
		if (key == 'Z' || key == 'z') useSonar();
	}	
}

void FrackMan::moveFrackMan(int key)		// Makes FrackMan face or move in a particular direction
{
	Direction dir = keyToDir(key); 
	if (getDirection() != dir)
		setDirection(dir);
    else								
    {			
    	if (dir == left) movePosBy(-1, 0);
    	if (dir == right) movePosBy(1, 0);
    	if (dir == up) movePosBy(0, 1);
    	if (dir == down) movePosBy(0, -1);
    }
}

void FrackMan::movePosBy(int x, int y)		// Moves FrackMan by a specified amount
{
	int j = getX();
	int k = getY();	

	if (getWorld()->isInBounds(j+x, k+y) && getWorld()->nearestObjDist<Boulder>(j + x, k + y) > 3.0)	
		moveTo(j+x, k+y);
	else 
		moveTo(j, k);
}

void FrackMan::dig()			// Allows FrackMan to "dig" (remove) dirt
{
	int x = getX();
	int y = getY();

	bool dirtRemoved = false;

	for (int j = 0; j < 4; j++)
		for (int k = 0; k < 4; k++)
			if (getWorld()->isDirtHere(x+j,y+k))
				dirtRemoved = getWorld()->removeDirt(x+j,y+k);

	if (dirtRemoved == true)
		getWorld()->playSound(SOUND_DIG);
}

void FrackMan::squirtAction()		// Allows FrackMan to squirt water
{
	if (m_squirts == 0)
		return;

	m_squirts--;
	int x = getX();
	int y = getY();
	Direction dir = getDirection();
	
	if (getWorld()->newPosValid(x, y, 4, dir) && !(getWorld()->isDirtHere(x, y)) && getWorld()->nearestObjDist<Boulder>(x,y) > 3.0)	
	{
		Squirt* sq = new Squirt(x, y, dir, getWorld());
		getWorld()->insertSquirt(sq);
	}

	getWorld()->playSound(SOUND_PLAYER_SQUIRT);
}

int FrackMan::getSquirtsLeftInSquirtGun() const
{
	return m_squirts;
}

int FrackMan::getPlayerGoldCount() const
{
	return m_goldNuggets;
}

int FrackMan::getPlayerSonarChargeCount() const
{
	return m_sonarCharges;
}

void FrackMan::incGoldNuggets()
{
	m_goldNuggets++;
}

void FrackMan::dropGoldNugget()			// Allows FrackMan to drop a gold nugget
{
	GoldNugget* newDroppedGoldNugget = new GoldNugget(getX(), getY(), GoldNugget::dropped, getWorld());
	getWorld()->insertGoldNugget(newDroppedGoldNugget);
}

void FrackMan::pickedUpSonarKit()
{
	m_sonarCharges += 2;
}

void FrackMan::useSonar()
{
	if (m_sonarCharges > 0)
	{
		m_sonarCharges--;
		getWorld()->illuminateAroundFrackMan<Actor>();
		getWorld()->playSound(SOUND_SONAR);
	}
}

void FrackMan::pickedUpWater()
{
	m_squirts += 5;
}

void FrackMan::makeFrackManDie()
{
	setDead();		// If FrackMan has no more hit points, then FrackMan is dead
}

//////////////////////////////////////////////////

Dirt::Dirt(int startX, int startY, StudentWorld* world)
	: Actor(world, IID_DIRT, startX, startY, 3, right, 0.25)
{}

Dirt::~Dirt()
{}

void Dirt::doSomething()
{}

//////////////////////////////////////////////////

Boulder::Boulder(int x, int y, StudentWorld* world)
	: Actor(world, IID_BOULDER, x, y, 1, down), m_state(stable)
{}

Boulder::~Boulder()
{}

void Boulder::doSomething()
{
	if (!isAlive()) return;

	switch(m_state)
	{
		case stable:		// Stable state
			if (!isDirtBelow(getX(), getY()))
				m_state = waiting;
			break;
		case waiting:		// Waiting state
			incNumTicks();
			if (getNumTicks() == 30)
			{
				m_state = falling;
				getWorld()->playSound(SOUND_FALLING_ROCK);
			}
			break;
		case falling:		// Falling state
			if (getWorld()->nearestObjDist<FrackMan>(getX(), getY()) <= 3.0)
			{
				//getWorld()->getFrackMan()->annoyBy(100);			// CORRECT?!?!? MESSING UP
				getWorld()->getFrackMan()->makeFrackManDie();
				return;
			}


			vector<Protester*> m_protesters;

			getWorld()->getAllTypeOf<Protester>(getX(), getY(), 3.0, m_protesters);
			//cout << "m_protesters.size(): " << m_protesters.size() << endl;
			
			for (vector<Protester*>::iterator k = m_protesters.begin(); k != m_protesters.end(); ++k)
			{
				if (!(*k)->isLeaving())
				{
					(*k)->annoyBy(100);
					(*k)->playLeavingSound();
					//if ((*k)->getHitPoints() <= 0) getWorld()->increaseScore(500);
				}
			}

			if (getWorld()->isInBounds(getX(), getY()-1) && !(isDirtBelow(getX(), getY())) && !(getWorld()->isObjThere<Boulder>(getX(), getY()-4)))
				moveTo(getX(), getY()-1);
			else
				setDead();
			
			break;
	}
}

bool Boulder::isDirtBelow(int x, int y)			// Checks to see if there is any dirt below a boulder
{
	for (int k = 0; k < 4; k++)
		if (getWorld()->isDirtHere(x+k, y-1))
			return true;
	return false;
}

bool Boulder::isBoulderFalling() const
{
	if (m_state == falling) return true;
	return false;
}

//////////////////////////////////////////////////

Squirt::Squirt(int x, int y, Direction dir, StudentWorld* world)
	: Actor(world, IID_WATER_SPURT, x, y, 1, dir), m_travelDist(4)
{}

Squirt::~Squirt()
{}

void Squirt::doSomething()
{
	int x = getX();
	int y = getY();	
	Direction dir = getDirection();
	vector<Protester*> m_protesters;

	getWorld()->getAllTypeOf<Protester>(x, y, 3.0, m_protesters);
	//cout << "m_protesters.size(): " << m_protesters.size() << endl;

	for (vector<Protester*>::iterator k = m_protesters.begin(); k != m_protesters.end(); ++k)
	{
		//cout << "Protester Hit Points Before: " << (*k)->getHitPoints() << endl;
		(*k)->annoyBy(2);	
		//cout << "Protester Hit Points After: " << (*k)->getHitPoints() << endl;
		//if ((*k)->getHitPoints() <= 0 && !soundPlayed) getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);		// CORRECT?!?!?
		if ((*k)->getHitPoints() <= 0) (*k)->playLeavingSound();
		setDead();
		//if ((*k)->getHitPoints() <= 0) getWorld()->increaseScore(100);
	}

	if (m_travelDist == 0 || !(getWorld()->newPosValid(x, y, 1, dir)) || getWorld()->isDirtHere(x, y) || getWorld()->isObjThere<Boulder>(x,y))
	{
		setDead();
		return;
	}

	if (m_travelDist != 0)		// Makes the squirt travel a distance of 4
	{
		moveTo(x, y);
		m_travelDist--;
	}
}

//////////////////////////////////////////////////

Barrel::Barrel(int x, int y, StudentWorld* world)
	: Actor(world, IID_BARREL, x, y, 2)
{
	setVisible(false);			// Hides the barrel when created
}

Barrel::~Barrel()
{}

void Barrel::doSomething()
{
	if (!isAlive()) return;
	if (makeObjNearFrackManVisible()) return;

	if (getWorld()->nearestObjDist<FrackMan>(getX(), getY()) <= 3.0)
	{
		getWorld()->decNumBarrels();
		setDead();
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
	}
}

//////////////////////////////////////////////////

GoldNugget::GoldNugget(int x, int y, goldState state, StudentWorld* world)
	: Actor(world, IID_GOLD, x, y, 2), m_state(state)
{
	if (state == buried) setVisible(false);			// Permanent/buried state: hidden when created
	if (state == dropped) setVisible(true);			// Dropped state: visible when created
}

GoldNugget::~GoldNugget()
{}

void GoldNugget::doSomething()
{
	if (!isAlive()) return;
	if (makeObjNearFrackManVisible()) return;

	if (m_state == buried && getWorld()->nearestObjDist<FrackMan>(getX(), getY()) <= 3.0)		// FrackMan picks up gold nugget
	{
		getWorld()->getFrackMan()->incGoldNuggets();
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		//cout << "GOT GOLD" << endl;
		getWorld()->increaseScore(10);
	}

	Protester* protester = getWorld()->getTypeOf<Protester>(getX(), getY(), 3.0);
	if (m_state == dropped && protester)
	{
		setDead();
		protester->bribe();
		return;							// INCOMPLETE: FIX FOR PROTESTER
	}
	else if (m_state == dropped)
	{
		incNumTicks();
		//cout << getNumTicks() << endl;
		if (getNumTicks() == 100) setDead();
	}
}

//////////////////////////////////////////////////

SonarKit::SonarKit(int x, int y, StudentWorld* world)
	: Actor(world, IID_SONAR, x, y, 2)
{
	m_tickLifetime = max(100, 300 - (10*(static_cast<int>(getWorld()->getLevel()))));		// Sets a sonar kit's lifetime on the oil field
}

SonarKit::~SonarKit()
{}

void SonarKit::doSomething()
{
	if (!isAlive()) return;

	incNumTicks();

	if (getWorld()->nearestObjDist<FrackMan>(getX(), getY()) <= 3.0)
	{
		getWorld()->getFrackMan()->pickedUpSonarKit();
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		//cout << "GOT SONAR KIT" << endl;
		getWorld()->increaseScore(75);
	}
	if (getNumTicks() == m_tickLifetime) setDead();
}

//////////////////////////////////////////////////

WaterPool::WaterPool(int x, int y, StudentWorld* world)
	: Actor(world, IID_WATER_POOL, x, y, 2)
{
	m_tickLifetime = max(100, 300 - (10*(static_cast<int>(getWorld()->getLevel()))));		// Sets a water pool's lifetime on the oil field
}

WaterPool::~WaterPool()
{}

void WaterPool::doSomething()
{
	if (!isAlive()) return;

	incNumTicks();

	if (getWorld()->nearestObjDist<FrackMan>(getX(), getY()) <= 3.0)
	{
		getWorld()->getFrackMan()->pickedUpWater();
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		//cout << "GOT WATER POOL" << endl;
		getWorld()->increaseScore(100);
	}
	if (getNumTicks() == m_tickLifetime) setDead();
}

//////////////////////////////////////////////////

Protester::Protester(StudentWorld* world, int imageID)
	: Actor(world, imageID, 60, 60, 0, left), m_isLeaving(false), m_hasLeavingSoundPlayed(false), m_ticksSinceLastShout(0), m_ticksToBeStunned(0), m_ticksSinceLastTurn(0), m_numSquaresToMoveInCurrentDirection(8 + (rand() % (60 - 8 + 1))), m_pathTo(*new Path())	 // 8 to 60
{
	setHitPoints(5);
	//cout << "Protester's Hit Points: " << getHitPoints() << endl;
	m_ticksToWaitBetweenMoves = max(0, 3 - ((static_cast<int>(getWorld()->getLevel()))/4));
	m_restingTicks = m_ticksToWaitBetweenMoves;
	//cout << "m_ticksToWaitBetweenMoves: " << m_ticksToWaitBetweenMoves << endl;
}

// Needs to ignore falling boulders

Protester::~Protester()
{}

void Protester::doSomething()
{
	// 1
	if (!isAlive()) return;
	if (m_ticksToBeStunned > 0 && getHitPoints() > 0)
	{
		//cout << "IM STUNNED/DISTRACTED!" << endl;
		m_ticksToBeStunned--;
		return;
	}

	// 2
	if (m_restingTicks > 0)
	{
		m_restingTicks--;
		return;
	}

	m_restingTicks = m_ticksToWaitBetweenMoves;
	m_ticksSinceLastShout++;
	m_ticksSinceLastTurn++;
	Direction protDir = getDirection();

	// 3
	if (m_isLeaving == true)		// Leaving state  
	{
		//cout << "I'M LEAVING!!!" << endl;

		Coord nextCoord = m_pathTo.nextCoordInPath();
		setDirection(dirToFaceFrackMan(nextCoord.xCoord, nextCoord.yCoord));

		moveTo(nextCoord.xCoord, nextCoord.yCoord);

		if (getX() == 60 && getY() == 60) 
		{
			setDead();
			//cout << "A PROTESTER IS DEAD NOW." << endl;
		}

		return;
	}
    
    // For Hardcore's pathfinding to FrackMan
   if (getWorld()->nearestObjDist<FrackMan>(getX(), getY()) > 4.0)
       if (followFrackMan()) return;


	//cout << "m_ticksSinceLastShout: " << m_ticksSinceLastShout << endl;
	//cout << "m_numSquaresToMoveInCurrentDirection: " << m_numSquaresToMoveInCurrentDirection << endl;
	// 4
	if ((getWorld()->nearestObjDist<FrackMan>(getX(), getY()) <= 4.0) && isFacingFrackman())
	{
		if (m_ticksSinceLastShout >= 15)
		{
			getWorld()->playSound(SOUND_PROTESTER_YELL);
			getWorld()->getFrackMan()->annoyBy(2);
			m_ticksSinceLastShout = 0;
		}
		return;
	}

	
	// 5
	// else if
    // if (getWorld()->nearestObjDist<FrackMan>(getX(), getY()) > 4.0) {		// Helen's code?
    //     followFrackMan();
    //     //cout << "Sup2" << endl;
    //     if (isInLineOfSightToFrackMan()){
    //         Direction oldDir = getDirection();
    //         Direction newDir = dirToFaceFrackMan(getWorld()->getFrackMan()->getX(), getWorld()->getFrackMan()->getY());
    //         // d = Direction of the protester if it were to face the frackman
    //         // is there a clear path from me to the frackman in `d`  direction?
    //         // if so ... change direction, reset m_num.....
    //         //cout << "newDir: " << newDir << endl;
    //         if (!getWorld()->isDirBlocked(getX(), getY(), newDir))
    //         {
    //             setDirection(newDir);
    //             m_numSquaresToMoveInCurrentDirection = 0;
    //             moveProtesterInDirBy(newDir, 1);
                
    //             if (((oldDir == left || oldDir == right) && (newDir == up || newDir == down)) || ((oldDir == up || oldDir == down) && (newDir == left || newDir == right)))
    //             {
    //                 m_ticksSinceLastTurn = 0;
    //                 cout << "m_ticksSinceLastTurn reset0: " << m_ticksSinceLastTurn << endl;
    //             }
                
    //             return;
    //         }
    //     }
    // }
        
	else if (isInLineOfSightToFrackMan() && (getWorld()->nearestObjDist<FrackMan>(getX(), getY()) > 4.0))
	{
		//cout << "Sup2" << endl;
		Direction oldDir = getDirection();
		Direction newDir = dirToFaceFrackMan(getWorld()->getFrackMan()->getX(), getWorld()->getFrackMan()->getY());
		// d = Direction of the protester if it were to face the frackman
		// is there a clear path from me to the frackman in `d`  direction?
		// if so ... change direction, reset m_num.....
		//cout << "newDir: " << newDir << endl;
		if (!getWorld()->isDirBlocked(getX(), getY(), newDir))
		{
			setDirection(newDir);
			m_numSquaresToMoveInCurrentDirection = 0;
	    	//moveProtesterInDirBy(newDir, 1);

			int mx = getX(), my = getY();
			if (getWorld()->newPosValid(mx,my,1,newDir) && getWorld()->nearestObjDist<Boulder>(mx, my) > 3.0 && !getWorld()->isDirBlocked(getX(), getY(), getDirection()))
				moveTo(mx,my);

	    	if (((oldDir == left || oldDir == right) && (newDir == up || newDir == down)) || ((oldDir == up || oldDir == down) && (newDir == left || newDir == right)))
			{
				m_ticksSinceLastTurn = 0;
				//cout << "m_ticksSinceLastTurn reset0: " << m_ticksSinceLastTurn << endl;
			}

	    	return;
	    }
   	// return;
	}

	// 7
	// if ticks have passed
	//	if left/right,
	//		check up/down
	//		if 1 viable, pick it, else if 2 viable, pick random ,else nothing
	//	if up/down,
	//		check left/right
	//		if 1 viable, pick it, else if 2 viable, pick random ,else nothing
	if (!isInLineOfSightToFrackMan() && isAtViableIntersection(getX(), getY(), protDir) && m_ticksSinceLastTurn >= 200)	//!isInLineOfSightToFrackMan() && 		DOESNT PROPERLY CHECK FOR TICKS SINCE LAST PERPENDICULAR TURN
	{
		//cout << "At intersection & trying to turn." << endl;
		
		int numIntersections = numOfViableIntersections(protDir);
		//cout << "numOfViableIntersections: " << numIntersections << endl;

		Direction newDir = dirProtesterToTurn(protDir);

		while (getWorld()->isDirBlocked(getX(), getY(), newDir))			// BIG PROBLEM HERE?
			newDir = dirProtesterToTurn(protDir);
		
		
		setDirection(newDir);
		
		m_numSquaresToMoveInCurrentDirection = 8 + (rand() % (60 - 8 + 1));		// 8 to 60
		m_ticksSinceLastTurn = 0;
		//cout << "m_ticksSinceLastTurn reset1: " << m_ticksSinceLastTurn << endl;
	}

	// 6
	else if (!isInLineOfSightToFrackMan())
	{
		//cout << "Sup3" << endl;
		m_numSquaresToMoveInCurrentDirection--;
		if (m_numSquaresToMoveInCurrentDirection <= 0)
		{
			//cout << "Sup3" << endl;
			Direction oldDir = getDirection();
			Direction newDir = getWorld()->getRandomDir();
			//bool isBlocked = true;
			while (getWorld()->isDirBlocked(getX(), getY(), newDir))
			{
				// setDirection(getWorld()->getRandomDir());
				// cout << "Direction: " << getDirection() << endl;
				// isBlocked = getWorld()->isDirBlocked(getX(), getY(), getDirection());

				newDir = getWorld()->getRandomDir();
				//cout << "Direction: " << getDirection() << endl;
			}
			setDirection(newDir);
			if (((oldDir == left || oldDir == right) && (newDir == up || newDir == down)) || ((oldDir == up || oldDir == down) && (newDir == left || newDir == right)))
			{
				m_ticksSinceLastTurn = 0;
				//cout << "m_ticksSinceLastTurn reset2: " << m_ticksSinceLastTurn << endl;
			}
			m_numSquaresToMoveInCurrentDirection = 8 + (rand() % (60 - 8 + 1));		// 8 to 60
			//cout << "New direction set." << endl;
		}	
	}

	// 7
	// if (!isInLineOfSightToFrackMan() && isAtViableIntersection(protDir))// && m_ticksSinceLastTurn >= 200)	//!isInLineOfSightToFrackMan() && 		DOESNT PROPERLY CHECK FOR TICKS SINCE LAST PERPENDICULAR TURN
	// {
	// 	cout << "At intersection & trying to turn." << endl;
		
	// 	int numIntersections = numOfViableIntersections(protDir);
	// 	cout << "numOfViableIntersections: " << numIntersections << endl;

	// 	Direction newDir = dirProtesterToTurn(protDir);

	// 	while (getWorld()->isDirBlocked(getX(), getY(), newDir))
	// 	{
	// 		newDir = dirProtesterToTurn(protDir);
	// 	}
	// 	setDirection(newDir);
	// 	m_numSquaresToMoveInCurrentDirection = 8 + (rand() % (60 - 8 + 1));		// 8 to 60
	// 	m_ticksSinceLastTurn = 0;
	// }

	// 8
	int mx = getX(), my = getY();
	if (getWorld()->newPosValid(mx,my,1,getDirection()) && getWorld()->nearestObjDist<Boulder>(mx, my) > 3.0 && !getWorld()->isDirBlocked(getX(), getY(), getDirection()))
		moveTo(mx,my);
	//moveProtesterInDirBy(getDirection(), 1);

	if (getWorld()->isDirBlocked(getX(), getY(), getDirection())) m_numSquaresToMoveInCurrentDirection = 0;

}

bool Protester::isFacingFrackman() const 	
{
	double pi = 3.14159265359;

	int frackmanX = getWorld()->getFrackMan()->getX();
	int frackmanY = getWorld()->getFrackMan()->getY();
	double x = getX() - frackmanX;
	double y = getY() - frackmanY;

	Direction dir = getDirection();

	if (x == 0 && y >= 0 && dir == down) return true;
	if (x == 0 && y <= 0 && dir == up) return true;
	// WHAT ABOUT FOR LEFT AND RIGHT?!?!?

	double degrees = (atan2(y, x)) * (180 / pi);
	//cout << "Dir: " << dir << endl;
	//cout << "Degrees: " << degrees << endl;
	if (dir == down && degrees >= 45 && degrees <= 135) return true;
	if (dir == up && degrees <= -45 && degrees >= -135) return true;
	if (dir == left && degrees <= 45 && degrees >= -45) return true;
	if (dir == right && (degrees <= -135 || degrees >= -135)) return true;
	// if (dir == right && degrees >= 135 && degrees <= -135) return true;

	return false;
}

bool Protester::isInLineOfSightToFrackMan() const 			// CORRECT?!?!?
{
	int protesterX = getX();
	int protesterY = getY();
	int frackmanX = getWorld()->getFrackMan()->getX();
	int frackmanY = getWorld()->getFrackMan()->getY();

	// if (getX() == getWorld()->getFrackMan()->getX() || getY() == getWorld()->getFrackMan()->getY()) 
	if (protesterX == frackmanX || protesterY == frackmanY) 
	{
		for (int j = min(protesterX, frackmanX); j <= max(protesterX, frackmanX); j++)
			for (int k = min(protesterY, frackmanY); k <= max(protesterY, frackmanY); k++)
				//if (!(getWorld()->isDirtHere(j,k)) && !(getWorld()->isObjThere<Boulder>(j,k)))
				if (getWorld()->isDirtInDir(getX(), getY(), getDirection()) || getWorld()->isObjThere<Boulder>(j,k))
					return false;
		return true;
	}
	return false; 
}

bool Protester::isLeaving() const
{
	if (m_isLeaving) return true;
	return false;
}

GraphObject::Direction Protester::dirToFaceFrackMan(int x, int y)
{
	// int frackmanX = getWorld()->getFrackMan()->getX();
	// int frackmanY = getWorld()->getFrackMan()->getY();
	x = getX() - x;
	y = getY() - y;

	if (x == 0 && y <= 0) return up;
	if (x == 0 && y > 0) return down;
	if (x >= 0 && y == 0) return left;
	if (x < 0 && y == 0) return right;
    return left;
}

void Protester::annoyBy(int amt)
{
	Actor::annoyBy(amt);
	if (getHitPoints() <= 0)
	{ 
		if (m_isLeaving == false)
			updateScoreWhenDead(amt);
		m_isLeaving = true;
		getWorld()->getBestPath(getX(), getY(), 60, 60, m_pathTo);
		m_restingTicks = 0;
	}
	else
	{
		stun();
	}
}

void Protester::updateScoreWhenDead(int hpDecrease) {
	if (hpDecrease == 100) getWorld()->increaseScore(500);
	else getWorld()->increaseScore(100);
}

void Protester::bribe()
{
	getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
	getWorld()->increaseScore(25);
	m_isLeaving = true;
	getWorld()->getBestPath(getX(), getY(), 60, 60, m_pathTo);
}

void Protester::stun()
{
	getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
	setTicksToBeStunned(max(50, 100 - (static_cast<int>(getWorld()->getLevel()) * 10)));
	//m_ticksToBeStunned = max(50, 100 - (static_cast<int>(getWorld()->getLevel()) * 10));
}

bool Protester::isAtViableIntersection(int x, int y, Direction protDir) const
{
	if ((protDir == left || protDir == right) && (!getWorld()->isDirtInDir(getX(), getY(), up) || !getWorld()->isDirtInDir(getX(), getY(), down))) 
	{
		if ((!getWorld()->isDirtInDir(getX(), getY(), up) && getWorld()->isInBounds(x, y+4)) || (!getWorld()->isDirtInDir(getX(), getY(), down) && getWorld()->isInBounds(x, y-1)))
		{
			//cout << "At viable intersction. 1" << endl;
			return true;
		}
	}
	if ((protDir == up || protDir == down) && (!getWorld()->isDirtInDir(getX(), getY(), left) || !getWorld()->isDirtInDir(getX(), getY(), right))) 
	{
		if ((!getWorld()->isDirtInDir(getX(), getY(), left) && getWorld()->isInBounds(x-1, y)) || (!getWorld()->isDirtInDir(getX(), getY(), right) && getWorld()->isInBounds(x+4, y)))
		{
			//cout << "At viable intersction. 2" << endl;
			return true;
		}
	}
	return false;
}

int Protester::numOfViableIntersections(Direction protDir) const
{
	if ((protDir == left || protDir == right) && (!getWorld()->isDirtInDir(getX(), getY(), up) && !getWorld()->isDirtInDir(getX(), getY(), down))) return 2;
	else if ((protDir == left || protDir == right) && (!getWorld()->isDirtInDir(getX(), getY(), up) || !getWorld()->isDirtInDir(getX(), getY(), down))) return 1;

	if ((protDir == up || protDir == down) && (!getWorld()->isDirtInDir(getX(), getY(), left) && !getWorld()->isDirtInDir(getX(), getY(), right))) return 2;
	else if ((protDir == up || protDir == down) && (!getWorld()->isDirtInDir(getX(), getY(), left) || !getWorld()->isDirtInDir(getX(), getY(), right))) return 1;
	
	return 0;
}

GraphObject::Direction Protester::dirProtesterToTurn(Direction protDir) const
{	
	if (numOfViableIntersections(protDir) == 1)
	{
		if ((protDir == left || protDir == right) && (!getWorld()->isDirBlocked(getX(), getY(), up))) return up;	
		else if ((protDir == left || protDir == right) && (!getWorld()->isDirBlocked(getX(), getY(), down))) return down;

		if ((protDir == up || protDir == down) && (!getWorld()->isDirBlocked(getX(), getY(), left))) return left;
		else if ((protDir == up || protDir == down) && (!getWorld()->isDirBlocked(getX(), getY(), right))) return right;
	}

	else if (numOfViableIntersections(protDir) == 2)
	{
		int randDir = rand() % 2;
		if ((protDir == left || protDir == right) && (!getWorld()->isDirBlocked(getX(), getY(), up) && !getWorld()->isDirBlocked(getX(), getY(), down)))
		{
			if (randDir == 0) return up;
			else return down;
		}
		if ((protDir == up || protDir == down) && (!getWorld()->isDirBlocked(getX(), getY(), left) && !getWorld()->isDirBlocked(getX(), getY(), right)))
		{
			if (randDir == 0) return left;
			else return down;
		}
	}
	return none;
}

void Protester::setTicksToBeStunned(int amt) { m_ticksToBeStunned = amt; }

void Protester::playLeavingSound()
{
	if (!m_hasLeavingSoundPlayed)
	{
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		m_hasLeavingSoundPlayed = true;
	}
}

bool Protester::followFrackMan() { return false; }

bool Protester::isProtester() const { return true; }

//////////////////////////////////////////////////

HardcoreProtester::HardcoreProtester(StudentWorld* world)
	: Protester(world, IID_HARD_CORE_PROTESTER), m_pathToFrackMan(*new Path())
{ setHitPoints(20); }

HardcoreProtester::~HardcoreProtester() {}

void HardcoreProtester::updateScoreWhenDead(int hpDecrease) {
	if (hpDecrease == 100) getWorld()->increaseScore(500);
	else getWorld()->increaseScore(250);
}

void HardcoreProtester::bribe()
{
	getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
	getWorld()->increaseScore(50);
	setTicksToBeStunned(max(50, 100 - (static_cast<int>(getWorld()->getLevel()) * 10)));		// ticks to stare
}

bool HardcoreProtester::followFrackMan()
{
	int M = 16 + (static_cast<int>(getWorld()->getLevel() * 2));
    getWorld()->getBestPath(getX(), getY(), getWorld()->getFrackMan()->getX(), getWorld()->getFrackMan()->getY(), m_pathToFrackMan);
	if (m_pathToFrackMan.getPathSize() <= M)
	{
		Coord next = m_pathToFrackMan.nextCoordInPath();
		setDirection(dirToFaceFrackMan(next.xCoord, next.yCoord));
		moveTo(next.xCoord, next.yCoord);	
		return true;
	}

	return false;
}
