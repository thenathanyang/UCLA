#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;	
struct Path;				

class Actor : public GraphObject
{
	public:
		Actor(StudentWorld* world, int imageID, int startX, int startY, unsigned int depth = 0, Direction dir = right, double size = 1.0);
		virtual ~Actor();
		virtual void doSomething() = 0;
		bool isAlive() const;						
		void setDead();									
		StudentWorld* getWorld() const;
		int getHitPoints() const;
		virtual void annoyBy(int amt);
		bool makeObjNearFrackManVisible();
		int getNumTicks() const;
		void incNumTicks();
		void setHitPoints(int amt);
		virtual bool isProtester() const;
		//bool isDirtInDir(GraphObject::Direction dir) const;

	private:
		bool m_alive;
		StudentWorld* m_world;
		int m_hitPoints;
		int m_numTicks;

};

class FrackMan : public Actor
{
	public:
		FrackMan(StudentWorld* world);
		~FrackMan();
		virtual void doSomething();
		int getSquirtsLeftInSquirtGun() const;
		int getPlayerGoldCount() const;
		int getPlayerSonarChargeCount() const;
		void incGoldNuggets();
		void pickedUpSonarKit();
		void pickedUpWater();
		void makeFrackManDie();

	private:
		void moveFrackMan(int key);
		void movePosBy(int x, int y);
		void dig();
		void squirtAction();
		void dropGoldNugget();
		void useSonar();
		int m_squirts;
		int m_sonarCharges;
		int m_goldNuggets;
};

class Dirt : public Actor
{
	public:
		Dirt(int startX, int startY, StudentWorld* world);
		~Dirt();
		virtual void doSomething();
};

class Boulder : public Actor
{
	public:
		enum boulderState { stable, waiting, falling };
		Boulder(int x, int y, StudentWorld* world);
		~Boulder();
		virtual void doSomething();
		bool isBoulderFalling() const;
	private:
		bool isDirtBelow(int x, int y);
		boulderState m_state;
		int m_numTicks;

};

class Squirt : public Actor
{
	public:
		Squirt(int x, int y, Direction dir, StudentWorld* world);
		~Squirt();
		virtual void doSomething();
	private:
		int m_travelDist;

};

class Barrel : public Actor
{
	public:
		Barrel(int x, int y, StudentWorld* world);
		~Barrel();
		virtual void doSomething();
};

class GoldNugget : public Actor
{
	public:
		enum goldState { buried, dropped };
		GoldNugget(int x, int y, goldState state, StudentWorld* world);
		~GoldNugget();
		virtual void doSomething();
	private:
		goldState m_state;
};

class SonarKit : public Actor
{
	public:
		SonarKit(int x, int y, StudentWorld* world);
		~SonarKit();
		virtual void doSomething();
	private:
		int m_tickLifetime;
};

class WaterPool : public Actor
{
	public:
		WaterPool(int x, int y, StudentWorld* world);
		~WaterPool();
		virtual void doSomething();
	private:
		int m_tickLifetime;

};

class Protester : public Actor
{
	public:
		Protester(StudentWorld* world, int imageID = IID_PROTESTER);
		~Protester();
		virtual void doSomething();
		bool isFacingFrackman() const;
		bool isInLineOfSightToFrackMan() const;
		bool isLeaving() const;
		GraphObject::Direction dirToFaceFrackMan(int x, int y);
		virtual void annoyBy(int amt);
		virtual void bribe();
		void stun();
		bool isAtViableIntersection(int x, int y, Direction protDir) const;
		int numOfViableIntersections(Direction protDir) const;
		GraphObject::Direction dirProtesterToTurn(Direction protDir) const;
		void setTicksToBeStunned(int amt);
		void playLeavingSound();
		virtual bool isProtester() const;

	private:
		virtual bool followFrackMan();
		virtual void updateScoreWhenDead(int hpDecrease);
		bool m_isLeaving;
		bool m_hasLeavingSoundPlayed;
		int m_numSquaresToMoveInCurrentDirection;
		int m_ticksToWaitBetweenMoves;
		int m_ticksToBeStunned;
		int m_restingTicks;
		int m_ticksSinceLastShout;
		int m_ticksSinceLastTurn;
		Path& m_pathTo;
		

};

class HardcoreProtester : public Protester
{
	public:
		HardcoreProtester(StudentWorld* world);
		~HardcoreProtester();
		void bribe() override;
	private:
		bool followFrackMan() override;
		void updateScoreWhenDead(int hpDecrease) override;
		Path& m_pathToFrackMan;
};

#endif // ACTOR_H_
