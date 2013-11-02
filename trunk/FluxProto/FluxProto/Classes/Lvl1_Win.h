#pragma once
#include "room.h"
#include "Game.h"

class Lvl1_WinLayer :
	public RoomLayer
{
public:
	LAYER_NODE_FUNC(Lvl1_WinLayer);

	Lvl1_WinLayer(void);

	virtual void createCamera();
	virtual void createWorld();
	virtual void createGround();
	virtual void createBackground();

	virtual void createHero();
	virtual void createEnemies();
	virtual void createPowerups();
	virtual void createBoxes();
	virtual void createPortals();

	virtual void goToNextRoom();

	~Lvl1_WinLayer(void);

};

class Lvl1_WinScene : 
	public cocos2d::CCScene
{
public:
	Lvl1_WinScene():_layer(NULL){};
	~Lvl1_WinScene(){};
	bool init();
	SCENE_NODE_FUNC(Lvl1_WinScene);

	CC_SYNTHESIZE_READONLY(Lvl1_WinLayer*, _layer, Layer);

};
