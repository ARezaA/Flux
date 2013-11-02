#pragma once
#include "Enemies\Enemy.h"
class Spherodroid :
	public Enemy
{
public:
	Spherodroid(b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc, float scale=0.34);
	virtual void init(const char *spritePath, b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc);
	virtual void setSprite(const char* spritePath, int width, int height, CCPoint* loc);
	virtual int getType() {return 2;}
	virtual void setBody(b2World *initialWorld, float PTM_RATIO, CCPoint *loc);
	virtual void setFixture(float PTM_RATIO);
	virtual void moveEnemy();
	void moveLeft();
	void moveRight();
	void stop();
	void die();
	void dead();
	~Spherodroid(void);
};

