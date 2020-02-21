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
	bool overlap(Actor* a1, Actor* a2);
	bool hasOverlap(Actor* a);
	Actor* findOverlap(Actor* a);
	void addActor(Actor* a);
	~StudentWorld();

private:
	std::list<Actor*> actList;
	Socrates* player;
};

#endif // STUDENTWORLD_H_