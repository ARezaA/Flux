#pragma once
#include "cocos2d.h"
#include "Box2D\Box2D.h"

USING_NS_CC;

class Portal 
	: public CCNode
{
private:
	bool			debug;
	bool			solid;
	bool			visible;
	int				coolDown;
	float			width;
	float			height;
	float			scale;	
	CCSprite*		boxSprite;
	CCSprite*		textureSprite;
	b2Vec2			location;
	b2Body*			boxBody;
	b2Fixture*		boxFixture;
	b2Vec2 start;
	b2Vec2 end;
public:
	Portal(CCLayer *layer, b2World *world, float PTM_RATIO, float pixX, float pixY, float pixWidth, float pixHeight, float p1x, float p1y, float p2x, float p2y, bool debugMode = false, bool hard = false, float setScale = 0.34, bool isVisible=true);
	~Portal(void);
	
	void init(CCLayer *layer, b2World *world, float PTM_RATIO);
	void createSprite(float PTM_RATIO);
	void createTexture(float PTM_RATIO);
	void createBody(b2World *world, float PTM_RATIO);
	void createFixture(b2World* world, float PTM_RATIO);
	bool teleCollision(b2Fixture *fixtureA, b2Fixture *fixtureB);
	void resetPortal();
	void startAndEnd(b2Vec2 hero);

	b2Vec2 getStart(){return start;}
	b2Vec2 getEnd(){return end;}

	void showTexture();
	void hideTexture();
	b2Vec2 getB2Location(){return location;}
};

