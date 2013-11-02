#pragma once
#include "cocos2d.h"
#include "Box2D\Box2D.h"

USING_NS_CC;

class Powerup:
	public CCNode
{
protected:
	bool			used;
	float			scale;
	CCPoint			location;
	CCSprite*		powSprite;
	b2Body*			powBody;
	b2Fixture*		powFixture;
	
public:
	Powerup(CCLayer *layer, b2World *world, float PTM_RATIO, float posX, float posY, bool isUsed = false, float setScale = 0.17);
	~Powerup(void);

	void init(CCLayer *layer, b2World *world, float PTM_RATIO);
	void createSprite();
	void createBody(b2World *world, float PTM_RATIO);
	void createFixture(b2World *world, float PTM_RATIO);

	bool isTouching(b2Fixture* fixtureA, b2Fixture* fixtureB);
	void usePowerup();
	void resetPowerup();
};

