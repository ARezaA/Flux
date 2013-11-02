#pragma once
#include "room.h"
#include "Game.h"

class TutorialRoomLayer :
	public RoomLayer
{
public:
	LAYER_NODE_FUNC(TutorialRoomLayer);

	TutorialRoomLayer(void);

	virtual void createCamera();
	virtual void createWorld();
	virtual void createGround();
	virtual void createBackground();

	virtual void createHero();
	virtual void createEnemies();
	virtual void createPowerups();
	virtual void createBGMusic();
	virtual void createBoxes();
	virtual void createPortals();

	virtual void goToNextRoom();

	~TutorialRoomLayer(void);

};

class TutorialRoomScene : 
	public cocos2d::CCScene
{
public:
	TutorialRoomScene():_layer(NULL){};
	~TutorialRoomScene(){};
	bool init();
	SCENE_NODE_FUNC(TutorialRoomScene);

	CC_SYNTHESIZE_READONLY(TutorialRoomLayer*, _layer, Layer);

};
