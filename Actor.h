#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>

class StudentWorld;

////////////////////////////////////////////////////////////////////////////////////////////////
// Actor Declaration
////////////////////////////////////////////////////////////////////////////////////////////////

class Actor : public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* sWorld);
	StudentWorld* getWorld();
	bool isAlive() { return m_alive; };
	virtual void doSomething() = 0;
	virtual bool isBacFood() { return false; };
	void die() { m_alive = false; };
	virtual bool hasHP() { return false; };
	virtual bool blocksBacteria() { return false; };
	virtual bool hostile() { return false; };
	virtual bool hitByProj() { return false; };
	virtual ~Actor() {};
private:
	bool m_alive;
	StudentWorld* m_sWorld;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// DamageableObject Declaration
////////////////////////////////////////////////////////////////////////////////////////////////

class DamageableObject : public Actor
{
public:
	DamageableObject(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* sWorld, double health);
	int getHealth() const { return m_health; }
	void takeDamage(double damage) { m_health -= damage; }
	virtual void restoreHealth() { m_health = 100; }
	virtual bool hasHP() { return true; };

private:
	int m_health;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// Socrates Declaration
////////////////////////////////////////////////////////////////////////////////////////////////

class Socrates :public DamageableObject
{
public:
	Socrates(StudentWorld* sWorld);
	int getSpray() { return m_spray; };
	int getFlame() { return m_flame; };
	void setFlame(int flame) { m_flame = flame; };
	virtual void doSomething();
	/*virtual void moveAngle_2(Direction angle, double &x, double &y)
	{
		const double PI = 4 * atan(1);
		x = (VIEW_RADIUS * cos(angle * 1.0 / 360 * 2 * PI));
		y = (VIEW_RADIUS * sin(angle * 1.0 / 360 * 2 * PI));
	}*/
private:
	int m_spray = 20;
	int m_flame = 5;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// Bacteria & Child Classes Declaration
////////////////////////////////////////////////////////////////////////////////////////////////

class Bacteria : public DamageableObject
{
public:
	Bacteria(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* sWorld, double health, double damage, double speed, int score, int plan, int food);
	void setPlan(int plan) { m_plan = plan; };
	int getPlan() { return m_plan; };
	double getDamage() { return m_damage; };
	int getScoreBac() { return m_score; };
	int getFood() { return m_food; };
	void setFood(int food) { m_food = food; };
	void resetPlan();
	virtual void move();
	virtual void doSomething();
	virtual bool hostile() { return true; };
private:
	double m_damage;
	double m_speed;
	int m_score;
	int m_plan;
	int m_food;
};

class regSalm : public Bacteria
{
public:
	regSalm(double startX, double startY, StudentWorld* sWorld);
	void doSomething();
};

class aggSalm : public Bacteria
{
public:
	aggSalm(double startX, double startY, StudentWorld* sWorld);
	void doSomething();
};

class eColi : public Bacteria
{
public:
	eColi(double startX, double startY, StudentWorld* sWorld);
	void doSomething();
};

////////////////////////////////////////////////////////////////////////////////////////////////
// Goodie & Child Class Declaration
////////////////////////////////////////////////////////////////////////////////////////////////

class Goodie : public Actor
{
public:
	Goodie(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* sWorld, int score);
	void doSomething();
	int getScore() { return m_score; };
	virtual bool hitByProj() { return true; };
	virtual void specialAbility() = 0;
private:
	int m_lifeTicks;
	int m_score;
};

class RestoreHealthGoodie : public Goodie
{
public:
	RestoreHealthGoodie(double startX, double startY, StudentWorld* sWorld);
	virtual void specialAbility();
};

class FlameGoodie : public Goodie
{
public:
	FlameGoodie(double startX, double startY, StudentWorld* sWorld);
	virtual void specialAbility();
};

class Fungus : public Goodie
{
public:
	Fungus(double startX, double startY, StudentWorld* sWorld);
	virtual void specialAbility();
};

class ExtraLifeGoodie : public Goodie
{
public:
	ExtraLifeGoodie(double startX, double startY, StudentWorld* sWorld);
	virtual void specialAbility();
};

////////////////////////////////////////////////////////////////////////////////////////////////
// Projectile and Child Class Declaration
////////////////////////////////////////////////////////////////////////////////////////////////

class Projectile : public Actor
{
public:
	Projectile(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* sWorld, double damage, double speed, double currDist, double maxDist);
	double getDamage() { return m_damage; }
	virtual void doSomething();
	/*virtual void checkCollide();*/
private:
	virtual void move();
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

class Flame : public Projectile
{
public:
	Flame(double startX, double startY, Direction dir, StudentWorld* sWorld);
};

////////////////////////////////////////////////////////////////////////////////////////////////
// Dirt Declaration
////////////////////////////////////////////////////////////////////////////////////////////////

class dirtPile :public Actor
{
public:
	dirtPile(double startX, double startY, StudentWorld* sWorld);
	virtual bool blocksBacteria() { return true; };
	virtual bool hitByProj() { return true; };
	virtual void doSomething();
};

////////////////////////////////////////////////////////////////////////////////////////////////
// Food Declaration
////////////////////////////////////////////////////////////////////////////////////////////////

class Food :public Actor
{
public:
	Food(double startX, double startY, StudentWorld* sWorld);
	virtual bool isBacFood() { return true; };
	virtual void doSomething();
};

////////////////////////////////////////////////////////////////////////////////////////////////
// Pit Declaration
////////////////////////////////////////////////////////////////////////////////////////////////

class Pit : public Actor
{
public:
	Pit(double startX, double startY, StudentWorld* sWorld);
	virtual void doSomething();
private:
	int m_regSalm;
	int m_aggSalm;
	int m_eColi;
	int m_totalBac;
};
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_