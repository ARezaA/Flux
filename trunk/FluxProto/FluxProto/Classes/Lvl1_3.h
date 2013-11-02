#pragma once
#include "room.h"
#include "Game.h"

class Lvl1_3Layer :
	public RoomLayer
{
public:
	LAYER_NODE_FUNC(Lvl1_3Layer);

	Lvl1_3Layer(void);

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

	~Lvl1_3Layer(void);

};

class Lvl1_3Scene : 
	public cocos2d::CCScene
{
public:
	Lvl1_3Scene():_layer(NULL){};
	~Lvl1_3Scene(){};
	bool init();
	SCENE_NODE_FUNC(Lvl1_3Scene);

	CC_SYNTHESIZE_READONLY(Lvl1_3Layer*, _layer, Layer);

};