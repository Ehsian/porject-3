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
	virtual bool blocksDirt() { return false; };
	void die() { m_alive = false; };
	virtual bool hasHP() { return false; };
private:
	bool m_alive;
	StudentWorld* m_sWorld;
};

class DamageableObject : public Actor
{
public:
	DamageableObject(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* sWorld, double health);
	double getHealth() const { return m_health; }
	void takeDamage(double damage) { m_health -= damage; }
	virtual void restoreHealth(double health) { m_health += health; }
	virtual bool hasHP() { return true; };

private:
	double m_health;
};

class Projectile : public Actor
{
public:
	Projectile(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* sWorld, double damage, double speed, double m_currDist, double maxDist);
	double getDamage() const { return m_damage; }
	virtual void doSomething();
	virtual void checkCollide();
	virtual void move();

private:
	double m_damage;
	double m_speed;
	double m_currDist;
	double m_maxDist;
};

class Spray : public Projectile 
{
public:
	Spray(double startX, double startY, Direction dir, StudentWorld* sWorld);
};

class Socrates :public DamageableObject
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
private:
	int m_spray = 20;
};

class dirtPile :public DamageableObject
{
public:
	dirtPile(double startX, double startY, StudentWorld* sWorld);
	virtual void doSomething();
};

class Food :public Actor
{
public:
	Food(double startX, double startY, StudentWorld* sWorld);
	virtual void doSomething();
	virtual bool blocksDirt() { return true; };
};

class Pit : public Actor
{
public:
	Pit(double startX, double startY, StudentWorld* sWorld);
	virtual void doSomething();
	virtual bool blocksDirt() { return true; }
};
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_