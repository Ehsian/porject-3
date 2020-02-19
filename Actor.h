#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* sWorld);
	StudentWorld* getWorld();
	virtual void doSomething() = 0;
	virtual bool isFood() { return false; };
	virtual ~Actor();
private:
	bool isAlive;
	StudentWorld* m_sWorld;
};

class Socrates :public Actor
{
public:
	Socrates(StudentWorld* sWorld);
	virtual void doSomething();
	/*virtual void moveAngle_2(Direction angle, double &x, double &y)
	{
		const double PI = 4 * atan(1);
		x = (VIEW_RADIUS * cos(angle * 1.0 / 360 * 2 * PI));
		y = (VIEW_RADIUS * sin(angle * 1.0 / 360 * 2 * PI));
	}*/
	virtual ~Socrates();
};

class dirtPile :public Actor
{
public:
	dirtPile(double startX, double startY, StudentWorld* sWorld);
	virtual void doSomething();
	virtual ~dirtPile();
};

class Food :public Actor
{
public:
	Food(double startX, double startY, StudentWorld* sWorld);
	virtual void doSomething();
	virtual bool isFood() {return true;};
	virtual ~Food();
};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_