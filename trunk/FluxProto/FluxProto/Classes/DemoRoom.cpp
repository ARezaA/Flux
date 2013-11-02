#include "pch.h"
#include "DemoRoom.h"

#define PTM_RATIO 256


bool DemoRoomScene::init()
{
	if(CCScene::init()){
		this->_layer = DemoRoomLayer::node();
		this->_layer->retain();
		this->addChild(_layer);
		return true;
	}
	return false;
}

DemoRoomLayer::DemoRoomLayer(void)
{
}


DemoRoomLayer::~DemoRoomLayer(void)
{
}

void DemoRoomLayer::goToNextRoom(){
	enemies->getObjectAtIndex(0)->setTagID(10);
	Lvl1_1Scene* Lvl1_1 = Lvl1_1Scene::node();
	CCDirector::sharedDirector()->replaceScene(Lvl1_1);
}

