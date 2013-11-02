#pragma once
#include "cocos2d.h"
#include "Box2D\Box2D.h"

USING_NS_CC;

class PhaseBox: 
		public CCNode
{
protected:
	bool			debug;
	bool			visible;
	float			width;
	float			height;
	float			scale;	
	CCSprite*		boxSprite;
	CCSprite*		textureSprite;
	b2Vec2			location;
	b2Body*			boxBody;
	b2Fixture*		boxFixture;
public:

	PhaseBox(CCLayer *layer, b2World *world, float PTM_RATIO, float pixX, float pixY, float pixWidth, float pixHeight, bool debugMode = false, float setScale = 0.34, bool isVisible = true);
	~PhaseBox(void);

	virtual void init(CCLayer *layer, b2World *world, float PTM_RATIO);
	virtual void createSprite(float PTM_RATIO);
	virtual void createTexture(float PTM_RATIO);
	virtual void createBody(b2World *world, float PTM_RATIO);
	virtual void createFixture(b2World *world, float PTM_RATIO);
	virtual void showTexture();
	virtual void hideTexture();
	virtual b2Vec2 getB2Location(){return location;}
};

