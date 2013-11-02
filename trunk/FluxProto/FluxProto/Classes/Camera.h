#pragma once
#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "Hero.h"

class Camera
{
protected:
	float		 roomHeight;
	float		 roomWidth;
	CCSize		 winSize;
	CCSprite	*cameraView;
	b2Body		*cameraBody;
public:
	Camera(float rWidth, float rHeight, CCLayer* layer);
	~Camera(void);

	void init(CCLayer* layer);
	CCSprite* getView(){return cameraView;}
	float getBottomPos(){return cameraView->getPositionY() - (winSize.height/2 *0.9);}
	void setPosition(CCPoint loc){cameraView->setPosition(loc);}
	void moveCamera(Hero* hero);
};

