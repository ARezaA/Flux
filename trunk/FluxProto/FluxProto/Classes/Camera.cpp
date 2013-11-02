#include "pch.h"
#include "Camera.h"


Camera::Camera(float rWidth, float rHeight, CCLayer* layer)
	:roomWidth(rWidth), roomHeight(rHeight), winSize(CCDirector::sharedDirector()->getWinSize())
{
	init(layer);
	layer->addChild(cameraView);
}


Camera::~Camera(void)
{
}

void Camera::init(CCLayer* layer){
	cameraView = CCSprite::spriteWithFile("camera.png", CCRectMake(0,0,roomWidth,roomHeight));
	cameraView->setPosition( ccp(winSize.width/2, winSize.height/2) );
}

void Camera::moveCamera(Hero* hero){
	float x, y;

	if(hero->isMovingLeft()){
		if(winSize.width >= roomWidth){
			x = cameraView->getPositionX();
		}else if(hero->getCocosLocation().x <= winSize.width * 6/10){
			x = winSize.width * 1/2;
		}else if(hero->getCocosLocation().x  >= roomWidth - (winSize.width * 6/10)){
			x = roomWidth - (winSize.width * 1/2);
		}else{
			x = hero->getCocosLocation().x - winSize.width/2 + winSize.width *2/5;
		}
	}else{
		if(winSize.width >= roomWidth){
			x = cameraView->getPositionX();
		}else if(hero->getCocosLocation().x <= winSize.width * 6/10){
			x = winSize.width * 1/2;
		}else if(hero->getCocosLocation().x  >= roomWidth - (winSize.width* 6/10)){
			x = roomWidth - (winSize.width *1/2);
		}else{
			x = winSize.width/2 - winSize.width *2/5 + hero->getCocosLocation().x ;
		}
	}

	if(winSize.height >= roomHeight){
		y = cameraView->getPositionY();
	}else if(hero->getCocosLocation().y  <= winSize.height* 6/10){
		y = winSize.height/2;
	}else if(hero->getCocosLocation().y >= roomHeight - (winSize.width * 4/10)){
		y = roomHeight - winSize.height/2 + 100;
	}else{
		y = winSize.height/2 - winSize.height *2/5 + hero->getCocosLocation().y;
	}

	cameraView->setPosition(ccp(x,y));
}

