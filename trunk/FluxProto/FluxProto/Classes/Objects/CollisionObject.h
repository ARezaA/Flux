#pragma once
#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "MyContactListener.h"
#include "CCApplication_win8_metro.h"
#include <string.h>

USING_NS_CC;

class CollisionObject :
	public cocos2d::CCNode
{
protected:
	float						 scale;
	int							 type;
	CCMutableArray<CCSprite*>	*sprites;
	b2Body						*objectBody;
	b2Fixture					*objectFixture;
	b2BodyDef					*objectBodyDef;
	b2PolygonShape				 collisionShape;
	CCLayer						*room;

public:
	CollisionObject(b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc, CCSprite *sprite, int type = 0, float width = 1, float height = 1, float scale = 0.34);
	void init(CCSprite *sprite, b2World * initialWorld, float PTM_RATIO, CCPoint* loc, float width, float height);
	void addSprite(CCSprite *sprite, float width, float height, CCPoint* loc);
	void CreateBox2DObject(b2World *initialWorld, float PTM_RATIO, CCPoint *loc, float width, float height);
	void setBody(b2World *initialWorld, float PTM_RATIO, CCPoint *loc);
	b2Body* getBody();
	void setFixture(float PTM_RATIO, float width, float height);
	b2Fixture* getFixture();
	~CollisionObject(void);
};

