#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayer
{
public:

	HelloWorld();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	
	// implement the "static node()" method manually
	LAYER_NODE_FUNC(HelloWorld);

	void ccTouchesEnded(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
};

#endif // __HELLOWORLD_SCENE_H__
