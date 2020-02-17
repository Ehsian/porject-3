#include "Actor.h"
#include "StudentWorld.h"

using namespace std;

Actor::Actor(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sWorld)
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

Socrates::Socrates(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sWorld)
	:Actor(imageID, startX, startY, dir, size, depth, sWorld)
{
	
}
void Socrates::doSomething()
{
	int key;
	if (getWorld()->getKey(key))
	{

	}
	return;
}
Socrates::~Socrates()
{

}

dirtPile::dirtPile(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sWorld)
	:Actor(imageID, startX, startY, dir, size, depth, sWorld)
{

}
void dirtPile :: doSomething()
{
	return;
}
dirtPile::~dirtPile()
{

}