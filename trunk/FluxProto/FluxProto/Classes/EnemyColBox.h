#pragma once
#include "cocos2d.h"
#include "Box2D\Box2D.h"

USING_NS_CC;

class EnemyColBox: 
	public CCNode
{
protected:
	bool			debug;
	float			width;
	float			height;
	float			scale;	
	CCSprite*		boxSprite;
	b2Vec2			location;
	b2Body*			boxBody;
	b2Fixture*		boxFixture;
public:

	EnemyColBox(CCLayer *layer, b2World *world, float PTM_RATIO, float pixX, float pixY, float pixWidth, float pixHeight, bool debugMode = false, float setScale = 0.34);
	~EnemyColBox(void);

	virtual void init(CCLayer *layer, b2World *world, float PTM_RATIO);
	virtual void createSprite();
	virtual void createBody(b2World *world, float PTM_RATIO);
	virtual void createFixture(b2World *world, float PTM_RATIO);
	virtual bool isTouching(b2Fixture *fixtureA, b2Fixture *fixtureB);
};

