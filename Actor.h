#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sWorld);
	StudentWorld* getWorld();
	virtual void doSomething() = 0;
	virtual ~Actor();
private:
	bool isAlive;
	StudentWorld* m_sWorld;
};

class Socrates :public Actor 
{
public:
	Socrates(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sWorld);
	virtual void doSomething();
	virtual ~Socrates();
};

class dirtPile :public Actor
{
public:
	dirtPile(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sWorld);
	virtual void doSomething();
	virtual ~dirtPile();
};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
