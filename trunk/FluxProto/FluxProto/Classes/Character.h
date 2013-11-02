#pragma once
#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "MyContactListener.h"
#include "CCApplication_win8_metro.h"
#include "CocosDenshion\include\SimpleAudioEngine.h"
#include <string.h>

USING_NS_CC;

//Base class for all enemies, the hero, and other characters
class Character:
	public cocos2d::CCNode
{
protected:
	bool						 living;
	bool						 leftOrRight; //true for Left, false for Right;
	float						 scale;
	float						 width;
	float						 height;
	int							 state;
	CCSprite					*charSprite;
	b2Body						*charBody;
	b2Fixture					*charFixture;
	b2BodyDef					*charBodyDef;
	b2PolygonShape				 collisionShape;
	CCMutableArray<CCAction*>	*charActions;
	b2Fixture*					 side;
	b2Vec2						 startPoint;

	//Spritesheet
	CCSpriteBatchNode* charSpriteSheet;
public:
	Character(const char *spritePath, b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc, int startState = 0, float charScale = 0.34);
	~Character(void);

	virtual void init(const char *spritePath, b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc);
	virtual void changeAction(int index);
	void addAction(CCAction* newAction);
	
	CCSprite* getSprite();
	CCPoint getCocosLocation(){return charSpriteSheet->getPosition();}
	b2Vec2 getB2Location(){return charBody->GetPosition();}
	virtual void setSprite(const char* spritePath, int width, int height, CCPoint* loc);
	
	void CreateBox2DObject(b2World *initialWorld, float PTM_RATIO, CCPoint *loc);
	virtual void setBody(b2World *initialWorld, float PTM_RATIO, CCPoint *loc);
	virtual b2Body* getBody();
	virtual void setFixture(float PTM_RATIO);
	virtual b2Fixture* getFixture();
	virtual bool getdirection(){return leftOrRight;}

	Character* isSpriteTag(int tagCheck);
	virtual int getType() {return 0;}
	virtual void die();
	virtual bool isAlive(){return living;}
	virtual void setLiving(bool alive){living = alive;}
	virtual void respawn();
	virtual bool isTouching(b2Fixture* fixture);
};

