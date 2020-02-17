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
	player = new Socrates(IID_PLAYER, 0, 128, 0, 1.0, 0, this);
	for (int i = 0; i < 20; i++)
	{
		/*int randX = randInt(0, VIEW_WIDTH);
		int randY = randInt(0, VIEW_HEIGHT);*/
		
		int distance = randInt(0, VIEW_RADIUS);
		double angle = randInt(0, 2 * 3.1415);
		int randX = distance * cos(angle) + VIEW_RADIUS;
		int randY = distance * sin(angle) + VIEW_RADIUS;
		dirtPile* newdirtPile = new dirtPile(IID_DIRT, randX, randY, 0, 1.0, 1, this);
		actList.push_back(newdirtPile);
	}
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    decLives();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}
