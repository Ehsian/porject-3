#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <cmath>
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

int StudentWorld::init()
{
	player = new Socrates(this);
	for (int i = 0; i < max(180 - 20 * getLevel(), 20); i++)
	{
		/*int randX = randInt(0, VIEW_WIDTH);
		int randY = randInt(0, VIEW_HEIGHT);*/
		const double PI = 4 * atan(1);
		int distance = randInt(0, 120);
		double angle = randInt(0, 360)*PI/180;
		double randX = distance * cos(angle) + VIEW_RADIUS;
		double randY = distance * sin(angle) + VIEW_RADIUS;
		dirtPile* newdirtPile = new dirtPile(randX, randY, this);
		actList.push_back(newdirtPile);
	}
	setGameStatText("Score: " + to_string(getScore()) + " Level: " + to_string(getLevel()) + " Lives: " + to_string(getLives()));
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	player->doSomething();
	list<Actor*>::iterator it;
	for (it = actList.begin(); it != actList.end(); it++)
	{
		(*it)->doSomething();
	}
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete player;
	player = NULL;

	list<Actor*>::iterator it;
	for (it = actList.begin(); it != actList.end(); it++)
	{
		delete* it;
	}
	actList.clear();
}

StudentWorld::~StudentWorld()
{
	this->cleanUp();
}