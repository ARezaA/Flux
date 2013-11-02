#pragma once
#include "room.h"
#include "Game.h"

class Lvl1_2Layer :
	public RoomLayer
{
public:
	LAYER_NODE_FUNC(Lvl1_2Layer);

	Lvl1_2Layer(void);

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

	~Lvl1_2Layer(void);

};

class Lvl1_2Scene : 
	public cocos2d::CCScene
{
public:
	Lvl1_2Scene():_layer(NULL){};
	~Lvl1_2Scene(){};
	bool init();
	SCENE_NODE_FUNC(Lvl1_2Scene);

	CC_SYNTHESIZE_READONLY(Lvl1_2Layer*, _layer, Layer);

};
