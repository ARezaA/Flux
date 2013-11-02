#pragma once

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "MyContactListener.h"
#include "Enemies\Enemy.h"
#include "Enemies\Guard.h"
#include "Enemies\Spherodroid.h"
#include "Hero.h"
#include "Objects\CollisionObject.h"
#include "CollisionBox.h"
#include "PhaseBox.h"
#include "TrapBox.h"
#include "EnemyColBox.h"
#include "Portal.h"
#include "Camera.h"
#include "Powerup.h"
#include "Bullet.h"
#include "MyDestructionListener.h"
#include "Enemies\Fearodroid.h"

USING_NS_CC;

class RoomLayer:
	public cocos2d::CCLayer
{
protected:
	float								 scale;
	bool								 isTeleporting;
	bool								 direction;
	int									 direction2;
	int									 direction2Director;
	int									 preTeleport;
	int									 jointCount;
	float								 roomHeight;
	float								 roomWidth;
	MyContactListener					*contactListener;
	CCSize								 winSize;
	b2World								*world;
	b2Body								*groundBody;
	b2Fixture							*roomFixture;
	b2Fixture							*bottomFixture;
	b2Fixture							*doorFixture;
	Hero								*hero;
	CCMutableArray<Enemy*>				*enemies;
	CCMutableArray<CollisionObject*>	*objects;
	CCMutableArray<Powerup*>			*powerups;
	CCMutableArray<CollisionBox*>		*colBoxes;
	CCMutableArray<Portal*>				*portalBox;
	CCMutableArray<PhaseBox*>			*phaseBox;
	CCMutableArray<TrapBox*>			*trapBox;
	CCMutableArray<EnemyColBox*>		*enemyColBox;
	CCMutableArray<Bullet*>				*bullets;
	CCPoint								 inputStart;
	b2Vec2								 startPosition;
	Camera								*cameraView;
	b2MouseJoint						*mouseJoint;
	MyDestructionListener				*destructionListener;
	CCLabelTTF							*outputLog;
	char								*outputText;

public:
	RoomLayer(void);
	~RoomLayer(void);
	virtual bool init(); 

	static cocos2d::CCScene* scene();

	LAYER_NODE_FUNC(RoomLayer);

	virtual void createCamera();
	virtual void createWorld();
	virtual void createGround();
	virtual void createBackground();
	virtual CCSprite* sizeSprite(char *myString);
	virtual void createBoxes();
	virtual void createPortals();

	virtual void createHero();
	virtual void createEnemies();
	virtual void createPowerups();
	virtual void createBGMusic();

	virtual void tick(cocos2d::ccTime dt);
	virtual void update(cocos2d::ccTime dt);
	virtual void swapDirection(Enemy *enemy);
	
	virtual void ccTouchesBegan(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	virtual void ccTouchesMoved(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	virtual void ccTouchesCancelled(cocos2d::CCSet* touches,cocos2d::CCEvent* event);

	virtual void startTeleporting();
	virtual void endTeleporting();
	virtual void finishTeleport();

	virtual b2Fixture* getRoomFixture(){return roomFixture;}
	virtual void killEnemies(b2Vec2 inputP2);
	virtual void killHero();
	virtual void restart();
	virtual void moveCamera(){cameraView->moveCamera(hero);}
	virtual void showPortals();
	virtual void hidePortals();

	virtual void goToNextRoom();

	void output(char *text);
};

class RoomScene : 
	public cocos2d::CCScene
{
public:
	RoomScene():_layer(NULL){};
	~RoomScene();
	bool init();
	SCENE_NODE_FUNC(RoomScene);

	CC_SYNTHESIZE_READONLY(RoomLayer*, _layer, Layer);

};

class MiniMenuLayer:
	public cocos2d::CCLayer
{
protected:
	CCSprite* background;
	CCMenu* miniMenu;
	CCMenuItemImage* BTNsettings;
	CCMenuItemImage* BTNmainmenu;
	CCMenuItemImage* BTNabout;

public:
	MiniMenuLayer(void);
	~MiniMenuLayer(void);
	virtual bool init(); 

	static cocos2d::CCScene* scene();

	LAYER_NODE_FUNC(MiniMenuLayer);

	
	virtual void ccTouchesBegan(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	virtual void ccTouchesMoved(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	virtual void ccTouchesCancelled(cocos2d::CCSet* touches,cocos2d::CCEvent* event);

	virtual void goMainMenu(cocos2d::CCObject*sender);
	virtual void goSettings(cocos2d::CCObject*sender);
	virtual void goAbout(cocos2d::CCObject*sender);
};