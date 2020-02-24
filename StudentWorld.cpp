#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <cmath>
#include <iostream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath)
{
}

void StudentWorld::randomSpawn(double& x, double& y)
{
	const double PI = 4 * atan(1);
	int distance = randInt(0, 120);
	double angle = randInt(0, 360) * PI / 180;
	x = distance * cos(angle) + VIEW_RADIUS;
	y = distance * sin(angle) + VIEW_RADIUS;
}
int StudentWorld::init()
{
	player = new Socrates(this);
	for (int i = 0; i < getLevel(); i++)
	{
		double randX, randY;
		randomSpawn(randX, randY);
		Pit* newPit = new Pit(randX, randY, this);
		while (hasOverlap(newPit))
		{
			double x, y;
			randomSpawn(x, y);
			newPit->moveTo(x, y);
		}
		actList.push_back(newPit);
	}
	for (int i = 0; i < min(5 * getLevel(), 25); i++)
	{
		double randX, randY;
		randomSpawn(randX, randY);
		Food* newFood = new Food(randX, randY, this);
		while (hasOverlap(newFood))
		{
			double x, y;
			randomSpawn(x, y);
			newFood->moveTo(x, y);
		}
		actList.push_back(newFood);
	}
	for (int i = 0; i < max(180 - 20 * getLevel(), 20); i++)
	{
		double randX, randY;
		randomSpawn(randX, randY);
		dirtPile* newdirtPile = new dirtPile(randX, randY, this);
		while (hasOverlap(newdirtPile) && (!findOverlap(newdirtPile)->blocksBacteria()))
		{
			double x, y;
			randomSpawn(x, y);
			newdirtPile->moveTo(x, y);
		}
		actList.push_back(newdirtPile);
	}
	m_bacKilled = 0;
	/*for (double i = 0; i < 3; i++)
	{
		Food* newFood = new Food(256, 137+8*i, this);
		actList.push_back(newFood);
	}
	regSalm* newSalm = new regSalm(256, 128, this);
	actList.push_back(newSalm);
	*/
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	player->doSomething();
	list<Actor*>::iterator it;
	for (it = actList.begin(); it != actList.end();)
	{
		(*it)->doSomething();

		if (!(*it)->isAlive())
		{
			if ((*it)->hostile())
			{
				m_bacKilled++;
			}
			delete (*it);
			it = actList.erase(it);
		}
		else 
			it++;
	}
	if (!player->isAlive())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	if (remainingBac() == 0)
	{
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}
	setGameStatText("Score: " + to_string(getScore()) + " Level: " + to_string(getLevel()) + " Lives: " + to_string(getLives()));
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete player;
	player = nullptr;

	list<Actor*>::iterator it;
	for (it = actList.begin(); it != actList.end(); it++)
	{
		delete* it;
	}
	actList.clear();
}

double StudentWorld::calcOverlap(double x1, double y1, double x2, double y2)
{
	double distance = sqrt((x1 - x2) * (x1 - x2) +
		(y1 - y2) * (y1 - y2));
	return distance;
}

bool StudentWorld::overlap(double x1, double y1, double x2, double y2)
{
	// Euclidean distance
	if (calcOverlap(x1,y1,x2,y2) <= SPRITE_WIDTH)
	{
		return true;
	}
	return false;
}

bool StudentWorld::overlapBlock(double x1, double y1, double x2, double y2)
{
	if (calcOverlap(x1, y1, x2, y2) <= SPRITE_WIDTH/2)
	{
		return true;
	}
	return false;
}

bool StudentWorld::hasOverlap(Actor* a)
{
	list<Actor*>::iterator it;
	for (it = actList.begin(); it != actList.end(); it++)
	{
		if (a != *it && overlap(a->getX(),a->getY(), (*it)->getX(),(*it)->getY()))
			return true;
	}
	return false;
}

Actor* StudentWorld::findOverlap(Actor* a)
{
	list<Actor*>::iterator it;
	for (it = actList.begin(); it != actList.end(); it++)
	{
		if (a != *it && overlap(a->getX(), a->getY(), (*it)->getX(), (*it)->getY()))
			return *it;
	}
	return nullptr;
}


void StudentWorld::checkCollision(Projectile* a)
{
	DamageableObject* collision = static_cast<DamageableObject*>(findOverlap(a));
	if (findOverlap(a) != nullptr && findOverlap(a)->hasHP())
	{
		collision->takeDamage(a->getDamage());
		a->die();
	}
	if (findOverlap(a) != nullptr && !findOverlap(a)->hasHP() && findOverlap(a)->hitByProj())
	{
		findOverlap(a)->die();
		a->die();
	}
}

void StudentWorld::checkCollisionBac(Bacteria* a)
{
	if (overlap(a->getX(), a->getY(), player->getX(), player->getY()))
	{
		player->takeDamage(a->getDamage());
		return;
	}
	if (a->getFood() == 3)
	{
		double x, y;
		if (a->getX() < VIEW_WIDTH/2)
			x = a->getX() + SPRITE_WIDTH;
		if (a->getX() > VIEW_WIDTH / 2)
			x = a->getX() - SPRITE_WIDTH;
		if (a->getX() > VIEW_WIDTH / 2)
			x = a->getX() - SPRITE_WIDTH;
		if (a->getY() < VIEW_WIDTH / 2)
			y = a->getY() + SPRITE_WIDTH;
		if (a->getY() > VIEW_WIDTH / 2)
			y = a->getY() - SPRITE_WIDTH;
		if (a->getY() > VIEW_WIDTH / 2)
			y = a->getY() - SPRITE_WIDTH;

		if (a->getDamage() == 1)
		{
			regSalm* newSalm = new regSalm(x, y, this);
			actList.push_back(newSalm);
		}
		a->setFood(0);
		return;
	}
	if (findOverlap(a) != nullptr && findOverlap(a)->isBacFood())
	{
		findOverlap(a)->die();
		a->setFood(a->getFood() + 1);
	}
}

bool StudentWorld::checkBlockBac(Bacteria* a)
{
	double x, y;
	a->getPositionInThisDirection(a->getDirection(), 3, x, y);
	list<Actor*>::iterator it;
	for (it = actList.begin(); it != actList.end(); it++)
	{
		if (a != *it && overlapBlock(x, y, (*it)->getX(), (*it)->getY()) && (*it)->blocksBacteria())
			return true;
	}
	if (calcOverlap(x, y, VIEW_WIDTH / 2, VIEW_HEIGHT / 2) >= VIEW_RADIUS)
	{
		return true;
	}
	return false;
}

int StudentWorld::findFood(Bacteria* a)
{
	list<Actor*>::iterator it;
	Actor* nearFood = nullptr;
	double close = 128;
	for (it = actList.begin(); it != actList.end(); it++)
	{
		if (a != *it && (*it)->isBacFood())
		{
			if (calcOverlap(a->getX(), a->getY(), (*it)->getX(), (*it)->getY()) <= close)
			{
				close = calcOverlap(a->getX(), a->getY(), (*it)->getX(), (*it)->getY());
				nearFood = *it;
			}
		}
	}
	if (nearFood != nullptr)
	{
		double PI = 4 * atan(1);
		return (180 / PI) * atan((nearFood->getY() - a->getY()) / (nearFood->getX() - a->getX()));
	}
	else 
		return 0;
}

void StudentWorld::addActor(Actor* a)
{
	actList.push_back(a);
}

StudentWorld::~StudentWorld()
{
	this->cleanUp();
}