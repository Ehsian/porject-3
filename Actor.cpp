#include "Actor.h"
#include "StudentWorld.h"

using namespace std;

Actor::Actor(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* sWorld)
	:GraphObject(imageID, startX, startY, dir, size, depth)
{
	isAlive = true;
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
			int direction = getDirection();
			setDirection(getDirection() - 5);
			moveAngle(getDirection() + 90,11);
			
			
		}
		if (key == KEY_PRESS_RIGHT)
		{
			moveAngle_2(getDirection()+185);
			setDirection(getDirection()+5);	
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