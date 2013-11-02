#pragma once
#include "room.h"
#include "Game.h"

class Lvl1_1Layer :
	public RoomLayer
{
public:
	LAYER_NODE_FUNC(Lvl1_1Layer);

	Lvl1_1Layer(void);

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

	~Lvl1_1Layer(void);

};

class Lvl1_1Scene : 
	public cocos2d::CCScene
{
public:
	Lvl1_1Scene():_layer(NULL){};
	~Lvl1_1Scene(){};
	bool init();
	SCENE_NODE_FUNC(Lvl1_1Scene);

	CC_SYNTHESIZE_READONLY(Lvl1_1Layer*, _layer, Layer);

};