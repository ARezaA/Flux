#pragma once
#include "cocos2d.h"
#include "Room.h"
#include "Game.h"

//Intro LAYER
class IntroLayer : public cocos2d::CCLayer
{
protected:
	cocos2d::CCSprite* text;
	int cineText;
	int followingLevel;
public:
	IntroLayer();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	
	// implement the "static node()" method manually
	LAYER_NODE_FUNC(IntroLayer);

	void ccTouchesBegan(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesEnded(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesCancelled(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void setFollowingLevel(int level);
	void text1Go();
	void text2Go();
	void text3Go();	
	void text4Go();
	void text5Go();
	void text6Go();
	void tutorialGo();

	~IntroLayer();
};

//Intro SCENE
class IntroScene: public cocos2d::CCScene
{
public:
	IntroScene():_layer(NULL){};
	
	bool init();
	void setFollowingLevel(int level);
	SCENE_NODE_FUNC(IntroScene);

	CC_SYNTHESIZE_READONLY(IntroLayer*, _layer, Layer);
	~IntroScene(){};
};
