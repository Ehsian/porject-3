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
	bool isAlive() { return m_alive; };
	virtual void doSomething() = 0;
	virtual bool isPlayer() { return false; };
	virtual bool isGoodie() { return false; };
	virtual bool isProj() { return false; };
	virtual bool isFood() { return false; };
	virtual bool isPit() { return false; };
	void die() { m_alive = false; };
	virtual ~Actor();
private:
	bool m_alive;
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
	virtual bool isFood() { return true; };
	virtual ~Food();
};

class Pit : public Actor
{
public:
	Pit(double startX, double startY, StudentWorld* sWorld);
	virtual void doSomething();
	virtual bool isPit() { return true; };
	virtual ~Pit();

};
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_