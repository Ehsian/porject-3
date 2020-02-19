#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>

using namespace std;

Actor::Actor(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* sWorld)
	:GraphObject(imageID, startX, startY, dir, size, depth)
{
	m_alive = true;
	m_sWorld = sWorld;
}
StudentWorld* Actor::getWorld()
{
	return m_sWorld;
}
Actor::~Actor()
{

}

Socrates::Socrates(StudentWorld* sWorld)
	:Actor(IID_PLAYER, 0, 128, 0, 1.0, 1, sWorld)
{

}
void Socrates::doSomething()
{
	int key;
	if (getWorld()->getKey(key))
	{
		if (key == KEY_PRESS_LEFT)
		{
			moveAngle(getDirection(), 128);
			setDirection(getDirection() + 5);
			moveAngle(getDirection(), -128);
			cout << getDirection() << endl;
		}
		if (key == KEY_PRESS_RIGHT)
		{
			moveAngle(getDirection(), 128);
			setDirection(getDirection() - 5);
			moveAngle(getDirection(), -128);
			/*double x, y;
			moveAngle_2(getDirection() - 185, x, y);
			moveTo(x + 128, y + 128);
			setDirection(getDirection() - 5); //87.5
			*/
		}
	}
	return;
}
Socrates::~Socrates()
{

}

dirtPile::dirtPile(double startX, double startY, StudentWorld* sWorld)
	:Actor(IID_DIRT, startX, startY, 0, 1.0, 1, sWorld)
{

}
void dirtPile::doSomething()
{
	return;
}
dirtPile::~dirtPile()
{

}
Food::Food(double startX, double startY, StudentWorld* sWorld)
	:Actor(IID_FOOD, startX, startY, 90, 1.0, 1, sWorld)
{

}
void Food::doSomething()
{
	return;
}
Food::~Food()
{

}
Pit::Pit(double startX, double startY, StudentWorld* sWorld)
	:Actor(IID_PIT, startX, startY, 0, 1.0, 1, sWorld)
{

}
void Pit::doSomething()
{
	return;
}
Pit::~Pit()
{

}