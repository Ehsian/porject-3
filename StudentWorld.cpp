#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

const double PI = 4 * atan(1);

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
	for (it = actList.begin(); it != actList.end(); it++)
	{
		(*it)->doSomething();
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
	}
	for (it = actList.begin(); it != actList.end();)
	{
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
	addGoodie();
	ostringstream oss;
	oss << "  Score: " << setw(6) << getScore() << oss.fill('0');
	oss << "  Level: " << getLevel();
	oss << "  Lives: " << getLives();
	oss << "  Health: " << player->getHealth();
	oss << "  Sprays: " << player->getSpray();
	oss << "  Flames: " << player->getFlame();
	setGameStatText(oss.str());
	//setGameStatText("Score: " + to_string(getScore()) + " Level: " + to_string(getLevel()) + " Lives: " + to_string(getLives()) + " Health: " + to_string(player->getHealth()) + " Sprays: " + to_string(player->getSpray()) + " Flames: " + to_string(player->getFlame()));
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
	if (calcOverlap(x1, y1, x2, y2) <= SPRITE_WIDTH)
	{
		return true;
	}
	return false;
}

bool StudentWorld::overlapBlock(double x1, double y1, double x2, double y2)
{
	if (calcOverlap(x1, y1, x2, y2) <= SPRITE_WIDTH / 2)
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
		if (a != *it && overlap(a->getX(), a->getY(), (*it)->getX(), (*it)->getY()))
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

bool StudentWorld::playerOverlap(Actor* a)
{
	if (overlap(a->getX(), a->getY(), player->getX(), player->getY()))
	{
		return true;
	}
	return false;
}

void StudentWorld::checkCollision(Projectile* a)
{
	Bacteria* collision = static_cast<Bacteria*>(findOverlap(a));
	if (findOverlap(a) != nullptr && findOverlap(a)->hasHP())
	{
		collision->takeDamage(a->getDamage());
		if (collision->getDamage() == 1 || collision->getDamage() == 2)
		{
			playSound(SOUND_SALMONELLA_HURT);
		}
		if (collision->getDamage() == 4)
		{
			playSound(SOUND_ECOLI_HURT);
		}
		//how to play sound
		a->die();
	}
	if (findOverlap(a) != nullptr && !findOverlap(a)->hasHP() && findOverlap(a)->hitByProj() && findOverlap(a)->isAlive())
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
		playSound(SOUND_PLAYER_HURT);
		return;
	}
	if (a->getFood() == 3)
	{
		double x, y;
		if (a->getX() < VIEW_WIDTH / 2)
			x = a->getX() + SPRITE_WIDTH;
		else if (a->getX() > VIEW_WIDTH / 2)
			x = a->getX() - SPRITE_WIDTH;
		else
			x = a->getX();
		if (a->getY() < VIEW_WIDTH / 2)
			y = a->getY() + SPRITE_WIDTH;
		else if (a->getY() > VIEW_WIDTH / 2)
			y = a->getY() - SPRITE_WIDTH;
		else
			y = a->getY();

		if (a->getDamage() == 1)
		{
			regSalm* newSalm = new regSalm(x, y, this);
			actList.push_back(newSalm);
			m_bacKilled--;
		}
		if (a->getDamage() == 2)
		{
			aggSalm* newaSalm = new aggSalm(x, y, this);
			actList.push_back(newaSalm);
			m_bacKilled--;
		}
		if (a->getDamage() == 4)
		{
			eColi* neweSalm = new eColi(x, y, this);
			actList.push_back(neweSalm);
			m_bacKilled--;
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

bool StudentWorld::checkBlockBac(Bacteria* a, int distance)
{
	double x, y;
	a->getPositionInThisDirection(a->getDirection(), distance, x, y);
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
		double angle = (180 / PI) * atan2((nearFood->getY() - a->getY()), (nearFood->getX() - a->getX()));
		return angle;
	}
	else
		return -1;
}

int StudentWorld::findSocrates(Bacteria* a, int range)
{
	if (calcOverlap(a->getX(), a->getY(), player->getX(), player->getY()) <= range)
	{
		double angle = (180 / PI) * atan2((player->getY() - a->getY()), (player->getX() - a->getX()));
		return angle;
	}
	else
		return -1;
}

void StudentWorld::addGoodie()
{
	int angle = randInt(0, 360);
	double x = (VIEW_RADIUS * cos(angle * (PI / 180))) + VIEW_RADIUS;
	double y = (VIEW_RADIUS * sin(angle * (PI / 180))) + VIEW_RADIUS;
	if (randInt(0, max(510 - getLevel() * 10, 200)) == 0)
	{
		addActor(new Fungus(x, y, this));
	}
	if (randInt(0, max(510 - getLevel() * 10, 250)) == 0)
	{
		int rand = randInt(1, 100);
		if ( rand <= 60)
		{
			addActor(new RestoreHealthGoodie(x, y, this));
			return;
		}
		else if ( rand <= 90)
		{
			addActor(new FlameGoodie(x, y, this));
			return;
		}
		else 
		{
			addActor(new ExtraLifeGoodie(x, y, this));
			return;
		}
	}
}

void StudentWorld::addActor(Actor* a)
{
	actList.push_back(a);
}

StudentWorld::~StudentWorld()
{
	this->cleanUp();
}