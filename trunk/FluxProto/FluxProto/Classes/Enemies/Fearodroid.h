#pragma once
#include "Enemies\Enemy.h"
class Fearodroid :
	public Enemy
{
protected:
	int					bulletCoolDown;
	CCSprite*			gun;
	b2Body*				gunBody;
	b2Fixture*			gunFixture;
	b2RevoluteJoint*	gunJoint;

public:
	Fearodroid(b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc, float scale=0.34);
	virtual void init(const char *spritePath, b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc);
	virtual void setSprite(const char* spritePath, int width, int height, CCPoint* loc);
	virtual int getType() {return 3;}
	virtual void setBody(b2World *initialWorld, float PTM_RATIO, CCPoint *loc);
	virtual void setFixture(float PTM_RATIO);
	virtual void moveEnemy();
	virtual b2Vec2 getB2gunLoc(){return gunBody->GetPosition();}
	void moveLeft();
	void moveRight();
	void stop();
	void attackHero(b2Vec2 heroLoc);
	void die();
	void dead();
	void respawn();
	void setAngle(float angle);
	void decrementCoolDown(){if(bulletCoolDown != 0) bulletCoolDown--;}
	void resetCoolDown(){bulletCoolDown = 240;}
	int getCoolDown(){return bulletCoolDown;}
	~Fearodroid(void);
};

