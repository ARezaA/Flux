#pragma once
#include "Enemy.h"
class Guard :
	public Enemy
{
public:
	Guard(b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc, float scale = 0.34);
	virtual void init(const char *spritePath, b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc);
	virtual void setSprite(const char* spritePath, int width, int height, CCPoint* loc);
	virtual void setFixture(float PTM_RATIO);
	virtual void moveEnemy();
	void moveLeft();
	void moveRight();
	void stop();
	void attackEnd();
	virtual void attackHero(b2Vec2 heroLocation);
	virtual void resetAttackTimeout(){attackTimeout = 75;}
	virtual bool isInAttackRange(b2Vec2 checkLoc){return attackRange->TestPoint(checkLoc);}
	virtual bool madeHit(b2Vec2 heroLocation);

	virtual void respawn();
	virtual void die();
	virtual void dead();
	virtual void playAttackSound(){CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Sounds\\attack2.mp3", false);}
	virtual void playDeathSound();

	virtual int getType() {return 1;}
	~Guard(void);
};

