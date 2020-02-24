#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <list>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void randomSpawn(double& x, double& y);
	double calcOverlap(double x1, double y1, double x2, double y2);
	bool overlap(double x1, double y1, double x2, double y2);
	bool overlapBlock(double x1, double y1, double x2, double y2);
	bool hasOverlap(Actor* a);
	Actor* findOverlap(Actor* a);
	void checkCollision(Projectile* a);
	void checkCollisionBac(Bacteria* a);
	bool checkBlockBac(Bacteria* a);
	int findFood(Bacteria* a);
	void addActor(Actor* a);
	int remainingBac() { return getLevel() * 5 - m_bacKilled; };
	~StudentWorld();

private:
	std::list<Actor*> actList;
	Socrates* player;
	int m_bacKilled;
};

#endif // STUDENTWORLD_H_