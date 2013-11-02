#pragma once
#include "cocos2d.h"
#include "Box2D\Box2D.h"

USING_NS_CC;

class Bullet:
	public CCNode
{
protected:
	static int			tagID;
	bool				live;
	float				width;
	float				height;
	float				scale;
	float				angle;
	float				speedY;
	float				slope;
	float				y;
	CCSprite*			bulletSprite;
	//CCSpriteBatchNode*	bulletSpriteSheet;
	b2Vec2				location;
	b2Vec2				heroLoc;
	b2Body*				boxBody;
	b2Fixture*			boxFixture;
public:

	Bullet(CCLayer *layer, b2World *world, float PTM_RATIO, b2Vec2 loc, b2Vec2 heroLocation, float setScale = 0.34);
	~Bullet(void);

	virtual void init(CCLayer *layer, b2World *world, float PTM_RATIO);
	virtual void createSprite(float PTM_RATIO);
	virtual void createBody(b2World *world, float PTM_RATIO);
	virtual void createFixture(b2World *world, float PTM_RATIO);
	virtual void fireBullet();
	virtual void die();
	virtual bool isAlive(){return live;}
	virtual b2Body* getBody(){return boxBody;}
	virtual CCSprite* getSprite(){return bulletSprite;}
	virtual bool isTouching(b2Fixture* A, b2Fixture* B){return (boxFixture == A || boxFixture == B);}
};

