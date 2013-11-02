#pragma once
#include "room.h"
#include "Game.h"

class Lvl1_4Layer :
	public RoomLayer
{
public:
	LAYER_NODE_FUNC(Lvl1_4Layer);

	Lvl1_4Layer(void);

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

	~Lvl1_4Layer(void);

};

class Lvl1_4Scene : 
	public cocos2d::CCScene
{
public:
	Lvl1_4Scene():_layer(NULL){};
	~Lvl1_4Scene(){};
	bool init();
	SCENE_NODE_FUNC(Lvl1_4Scene);

	CC_SYNTHESIZE_READONLY(Lvl1_4Layer*, _layer, Layer);

};