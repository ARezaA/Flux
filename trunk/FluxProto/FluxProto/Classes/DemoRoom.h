#pragma once
#include "Room.h"
#include "Game.h"

class DemoRoomLayer :
	public RoomLayer
{
public:
	LAYER_NODE_FUNC(DemoRoomLayer);
	DemoRoomLayer(void);

	~DemoRoomLayer(void);

	void goToNextRoom();
};

class DemoRoomScene : 
	public cocos2d::CCScene
{
public:
	DemoRoomScene():_layer(NULL){};
	~DemoRoomScene(){};
	bool init();
	SCENE_NODE_FUNC(DemoRoomScene);

	CC_SYNTHESIZE_READONLY(DemoRoomLayer*, _layer, Layer);

};
