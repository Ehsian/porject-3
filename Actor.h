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
	virtual ~Actor();
private:
	bool isAlive;
	StudentWorld* m_sWorld;
};

class Socrates :public Actor
{
public:
	Socrates (StudentWorld* sWorld);
	virtual void doSomething();

	virtual void moveAngle_2(Direction angle)
	{
		const double PI = 4 * atan(1);
		double newX = (VIEW_RADIUS * cos(angle * 1.0 / 360 * 2 * PI));
		double newY = (VIEW_RADIUS * sin(angle * 1.0 / 360 * 2 * PI));

		moveTo(newX + 128, newY + 128);
		increaseAnimationNumber();
	}
	/*
	virtual void moveAngle(Direction angle, int units = 1)
    {
    	const double PI = 4 * atan(1);
    	double newX = (getX() + units * cos(angle*1.0 / 360 * 2 * PI));
    	double newY = (getY() + units * sin(angle*1.0 / 360 * 2 * PI));

    	moveTo(newX, newY);
    	increaseAnimationNumber();
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

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_