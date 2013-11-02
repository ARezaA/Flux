#pragma once
#include "Character.h"
class Enemy : public Character
{
protected:
	static int	tagID;
	bool		hit;
	int			attackTimeout;
	int			totalMovements;
	int			movementCoolDown;
	b2Fixture	*attackRange;
public:
	virtual void init(const char *spritePath, b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, int width, int height, CCPoint* loc);
	virtual void moveEnemy();
	virtual void moveLeft(){}
	virtual void moveRight(){}
	virtual void attackEnd(){}
	virtual void stop(){}
	virtual void setState(int i){state = i;}
	virtual void resetMoveCoolDown(){movementCoolDown = 45;}
	virtual bool canTurn(){return movementCoolDown == 0;}
	virtual bool isAttackRange(b2Fixture* fixture){return fixture == attackRange;}
	virtual bool isInAttackRange(b2Vec2 checkLoc){return attackRange->TestPoint(checkLoc);}
	virtual void decrementAttackTimeout(){attackTimeout--;}
	virtual int getAttackTimeout(){return attackTimeout;}
	virtual void resetAttackTimeout(){attackTimeout = 60;}
	virtual bool madeHit(b2Vec2 heroLocation){return hit && isInAttackRange(heroLocation);}
	virtual void resetHit(){hit = false;}
	virtual void attackHero(b2Vec2 heroLocation){}
	static void setTagID(int id = 0){ tagID = id;}
	virtual void playAttackSound(){}
	virtual void playDeathSound(){}
	virtual int getDirection(){return state;}
	Enemy(const char *spritePath, b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, int width, int height, CCPoint* loc, int attack, float scale = 0.34);
	~Enemy(void);
};

