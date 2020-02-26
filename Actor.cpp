
#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>


using namespace std;

Actor::Actor(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* sWorld)
	:GraphObject(imageID, startX, startY, dir, size, depth), m_alive(true), m_sWorld(sWorld)
{}


StudentWorld* Actor::getWorld()
{
	return m_sWorld;
}

DamageableObject::DamageableObject(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* sWorld, double health)
	:Actor(imageID, startX, startY, dir, size, depth,sWorld), m_health(health)
{}

Projectile::Projectile(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* sWorld, double damage, double speed, double currDist, double maxDist)
	: Actor(imageID, startX, startY, dir, size, depth, sWorld), m_damage(damage), m_speed(speed),m_currDist(currDist), m_maxDist(maxDist)
{}

Bacteria::Bacteria(int imageID, double startX, double startY, Direction dir, double size, int depth, StudentWorld* sWorld, double health, double damage, double speed, int score, int plan, int food)
	:DamageableObject(imageID, startX, startY, dir,size, depth,sWorld, health), m_damage(damage), m_speed(speed), m_score(score), m_plan(plan), m_food(food)
{}

/*void Projectile::checkCollide()
{
	DamageableObject* collision = static_cast<DamageableObject*>(getWorld()->findOverlap(this));
	if (getWorld()->findOverlap(this) != nullptr && getWorld()->findOverlap(this)->hasHP())
	{
		collision->takeDamage(getDamage());
		die();
	}
	if (getWorld()->findOverlap(this) != nullptr && getWorld()->findOverlap(this)->hasHP())
	{
		if (collision->getHealth() <= 0)
		{
			getWorld()->findOverlap(this)->die();
		}
	}
}*/

void Projectile::move()
{
	moveForward(m_speed);
	m_currDist += m_speed;
	if (m_currDist >= m_maxDist)
	{
		die();
	}
}

void Projectile::doSomething()
{
	getWorld()->checkCollision(this);
	move();
}

void Bacteria::doSomething()
{
	getWorld()->checkCollisionBac(this);
}

Spray::Spray(double startX, double startY, Direction dir, StudentWorld* sWorld)
	:Projectile(IID_SPRAY, startX, startY, dir, 1.0, 1, sWorld, 2, SPRITE_WIDTH, 0, 112)
{}

Flame::Flame(double startX, double startY, Direction dir, StudentWorld* sWorld)
	: Projectile(IID_FLAME, startX, startY, dir, 1.0, 1, sWorld, 5, SPRITE_WIDTH, 0, 32)
{}

regSalm::regSalm(double startX, double startY, StudentWorld* sWorld)
	: Bacteria(IID_SALMONELLA, startX, startY, 90, 1.0, 1, sWorld, 4, 1, 3, 100, 0, 0)
{}

aggSalm::aggSalm(double startX, double startY, StudentWorld* sWorld)
	: Bacteria(IID_SALMONELLA, startX, startY, 90, 1.0, 1, sWorld, 10, 2, 3, 100, 0, 0)
{}

eColi::eColi(double startX, double startY, StudentWorld* sWorld)
	: Bacteria(IID_ECOLI, startX, startY, 90, 1.0, 1, sWorld, 5, 4, 3, 100, 0, 0)
{}

void Bacteria::resetPlan()
{
	if (getWorld()->findFood(this) == -1)
	{
		setDirection(randInt(0, 359));
		setPlan(10);
		return;
	}
	if (getWorld()->findFood(this) != -1)
	{
		setDirection(getWorld()->findFood(this));
		setPlan(10000);
		return;
	}
}

void Bacteria::move()
{
	setPlan(getPlan() - 1);
	if (!getWorld()->checkBlockBac(this,3))
	{
		moveForward(3);
	}
	if (getWorld()->checkBlockBac(this,3))
	{
		setDirection(randInt(0, 359));
		setPlan(10);
	}
}

void regSalm::doSomething()
{
	if (getHealth() <= 0)
	{
		if(randInt(1, 2)==1)
		{
			getWorld()->addActor(new Food(getX(), getY(), getWorld()));
		}
		die();
		getWorld()->playSound(SOUND_SALMONELLA_DIE);
		getWorld()->increaseScore(getScoreBac());
		return;
	}
	Bacteria::doSomething();
	if (getPlan() > 0)
	{
		move();
		if (getPlan() == 10)
			return;
	}
	if (getPlan() == 0)
	{
		resetPlan();
	}
}

void aggSalm::doSomething()
{
	if (getHealth() <= 0)
	{
		if (randInt(1, 2) == 1)
		{
			getWorld()->addActor(new Food(getX(), getY(), getWorld()));
		}
		die();
		getWorld()->playSound(SOUND_SALMONELLA_DIE);
		getWorld()->increaseScore(getScoreBac());
		return;
	}
	if (getWorld()->findSocrates(this,128) != -1)
	{
		setDirection(getWorld()->findSocrates(this,128));
		if (!getWorld()->checkBlockBac(this,3))
		{
			moveForward(3);
		}
		Bacteria::doSomething();
		return;
	}
	Bacteria::doSomething();
	if (getPlan() > 0)
	{
		move();
		if (getPlan() == 10)
			return;
	}
	if (getPlan() == 0)
	{
		resetPlan();
	}
}

void eColi::doSomething()
{
	if (getHealth() <= 0)
	{
		if (randInt(1, 2) == 1)
		{
			getWorld()->addActor(new Food(getX(), getY(), getWorld()));
		}
		die();
		getWorld()->playSound(SOUND_ECOLI_DIE);
		getWorld()->increaseScore(getScoreBac());
		return;
	}
	Bacteria::doSomething();
	if (getWorld()->findSocrates(this, 256) != -1)
	{
		setDirection(getWorld()->findSocrates(this, 256));
		for (int i = 0; i < 10; i++)
		{
			if (!getWorld()->checkBlockBac(this, 2))
			{
				moveForward(2);
				return;
			}
			if (getWorld()->checkBlockBac(this, 2))
			{
				setDirection(getDirection() + 10);
			}
		}
	}
}

Socrates::Socrates(StudentWorld* sWorld)
	:DamageableObject(IID_PLAYER, 0, 128, 0, 1.0, 1, sWorld,100)
{

}
void Socrates::doSomething()
{
	if (getHealth() <= 0)
	{
		die();
		getWorld()->playSound(SOUND_PLAYER_DIE);
		return;
	}
	int key;
	
	if (getWorld()->getKey(key))
	{
		switch (key)
		{
		case KEY_PRESS_LEFT:
			moveAngle(getDirection(), 128);
			setDirection(getDirection() + 5);
			moveAngle(getDirection(), -128);
			break;
		case KEY_PRESS_RIGHT:
			moveAngle(getDirection(), 128);
			setDirection(getDirection() - 5);
			moveAngle(getDirection(), -128);
			break;
		case KEY_PRESS_SPACE:
			if (m_spray > 0)
			{
				m_spray--;
				double x, y;
				getPositionInThisDirection(getDirection(), 4, x, y);
				getWorld()->addActor(new Spray(x, y, getDirection(), getWorld()));
				getWorld()->playSound(SOUND_PLAYER_SPRAY);
			}
			break;
		case KEY_PRESS_ENTER:
			if (m_flame > 0)
			{
				m_flame--;
				for (int i = 0; i < 16; i++)
				{
					double x, y;
					getPositionInThisDirection(getDirection() + 22 * i, SPRITE_WIDTH, x, y);
					getWorld()->addActor(new Flame(x, y, getDirection() + 22 * i, getWorld()));
				}
				getWorld()->playSound(SOUND_PLAYER_FIRE);
			}
			break;
		default: break;
		}
		/*if (key == KEY_PRESS_LEFT)
		{
			moveAngle(getDirection(), 128);
			setDirection(getDirection() + 5);
			moveAngle(getDirection(), -128);
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
			
		}
		if (key == KEY_PRESS_SPACE)
		{
			if (m_spray > 0)
			{
				m_spray--;
				getWorld()->addActor(new Spray(getX(), getY(), getDirection(), getWorld()));
			}
		}*/
	}
	if (key != KEY_PRESS_SPACE && m_spray < 20)
	{
		m_spray++;
	}
}

dirtPile::dirtPile(double startX, double startY, StudentWorld* sWorld)
	:Actor(IID_DIRT, startX, startY, 0, 1.0, 1, sWorld)
{

}
void dirtPile::doSomething()
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

Pit::Pit(double startX, double startY, StudentWorld* sWorld)
	:Actor(IID_PIT, startX, startY, 0, 1.0, 1, sWorld), m_regSalm(5), m_aggSalm(3), m_totalBac(5)
{

}
void Pit::doSomething()
{
	if (m_totalBac == 0)
	{
		die();
	}
	if (m_totalBac != 0)
	{
		if (randInt(1, 50) == 2)
		{
			getWorld()->addActor(new aggSalm(getX(), getY(), getWorld()));
			getWorld()->playSound(SOUND_BACTERIUM_BORN);
			m_totalBac--;
			m_regSalm--;
		}
	}
}
