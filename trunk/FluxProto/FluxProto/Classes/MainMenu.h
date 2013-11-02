#pragma once
#include "cocos2d.h"
#include "Room.h"
#include "Game.h"
#include "Intro.h"

//MAIN MENU LAYER
class MainMenuLayer : public cocos2d::CCLayer
{
protected:
	cocos2d::CCMenu* ElMenu;
	cocos2d::CCMenuItemImage* BTNContinue;
	cocos2d::CCMenuItemImage* BTNLevelSelect;
	cocos2d::CCMenuItemImage* BTNInfo;
	cocos2d::CCMenuItemImage* BTNAbout;
	cocos2d::CCMenuItemImage* BTNSettings;

public:
	MainMenuLayer();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	
	// implement the "static node()" method manually
	LAYER_NODE_FUNC(MainMenuLayer);

	void ccTouchesBegan(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesEnded(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesCancelled(cocos2d::CCSet* touches,cocos2d::CCEvent* event);

	void continueSelected(cocos2d::CCObject*sender);
	void levelSelectSelected(cocos2d::CCObject*sender);
	void infoSelected(cocos2d::CCObject*sender);
	void aboutSelected(cocos2d::CCObject*sender);
	void settingsSelected(cocos2d::CCObject*sender);

	~MainMenuLayer();
};

//MAIN MENU SCENE
class MainMenuScene: public cocos2d::CCScene
{
public:
	MainMenuScene():_layer(NULL){};
	
	bool init();
	SCENE_NODE_FUNC(MainMenuScene);

	CC_SYNTHESIZE_READONLY(MainMenuLayer*, _layer, Layer);
	~MainMenuScene(){};
};

//SETTINGS LAYER
class SettingsLayer : public cocos2d::CCLayer
{
protected:
	cocos2d::CCMenu* LeMenu;
	cocos2d::CCMenuItemImage* BTNBackies;

public:
	SettingsLayer();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	
	// implement the "static node()" method manually
	LAYER_NODE_FUNC(SettingsLayer);

	void ccTouchesBegan(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesEnded(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesCancelled(cocos2d::CCSet* touches,cocos2d::CCEvent* event);

	void backSelected(cocos2d::CCObject*sender);

	~SettingsLayer();
};

//SETTINGS SCENE
class SettingsScene: public cocos2d::CCScene
{
public:
	SettingsScene():_layer(NULL){};
	
	bool init();
	SCENE_NODE_FUNC(SettingsScene);

	CC_SYNTHESIZE_READONLY(SettingsLayer*, _layer, Layer);
	~SettingsScene(){};
};

//ABOOT LAYER
class AboutLayer : public cocos2d::CCLayer
{
protected:
	cocos2d::CCMenu* DasMenu;
	cocos2d::CCMenuItemImage* BTNBackies;

public:
	AboutLayer();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	
	// implement the "static node()" method manually
	LAYER_NODE_FUNC(AboutLayer);

	void ccTouchesBegan(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesEnded(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesCancelled(cocos2d::CCSet* touches,cocos2d::CCEvent* event);

	void backSelected(cocos2d::CCObject*sender);

	~AboutLayer();
};

//ABOOT SCENE
class AboutScene: public cocos2d::CCScene
{
public:
	AboutScene():_layer(NULL){};
	
	bool init();
	SCENE_NODE_FUNC(AboutScene);

	CC_SYNTHESIZE_READONLY(AboutLayer*, _layer, Layer);
	~AboutScene(){};
};

//LEVELSELECT LAYER
class LevelSelectLayer : public cocos2d::CCLayer
{
protected:
	cocos2d::CCMenu* menu0;
	cocos2d::CCMenu* menu1;
	cocos2d::CCMenu* menuBack;
	cocos2d::CCMenuItemImage* BTNBackies;
	cocos2d::CCMenuItemImage* BTNTutorial;
	cocos2d::CCMenuItemImage* BTNDemo;
	cocos2d::CCMenuItemImage* BTNLevel1_1;
	cocos2d::CCMenuItemImage* BTNLevel1_2;
	cocos2d::CCMenuItemImage* BTNLevel1_3;
	cocos2d::CCMenuItemImage* BTNLevel1_4;
	cocos2d::CCMenuItemImage* BTNLevel1_Win;

public:
	LevelSelectLayer();
	LAYER_NODE_FUNC(LevelSelectLayer);
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	
	// implement the "static node()" method manually
	
	void ccTouchesBegan(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesEnded(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesCancelled(cocos2d::CCSet* touches,cocos2d::CCEvent* event);

	void backSelected(cocos2d::CCObject*sender);
	void demoLevelGo(cocos2d::CCObject*sender);
	void tutorialLevelGo(cocos2d::CCObject*sender);
	void level1_1Go(cocos2d::CCObject*sender);
	void level1_2Go(cocos2d::CCObject*sender);
	void level1_3Go(cocos2d::CCObject*sender);
	void level1_4Go(cocos2d::CCObject*sender);
	void level1_WinGo(cocos2d::CCObject*sender);

	~LevelSelectLayer();
};

//LEVELSELECT SCENE
class LevelSelectScene: public cocos2d::CCScene
{
public:
	LevelSelectScene():_layer(NULL){};
	
	bool init();
	SCENE_NODE_FUNC(LevelSelectScene);

	CC_SYNTHESIZE_READONLY(LevelSelectLayer*, _layer, Layer);
	~LevelSelectScene(){};
};

//INFO LAYER
class InfoLayer : public cocos2d::CCLayer
{
protected:
	cocos2d::CCMenu* DerMenu;
	cocos2d::CCMenuItemImage* BTNBackies;
	cocos2d::CCSprite* InfoText;

public:
	InfoLayer();
	LAYER_NODE_FUNC(InfoLayer);
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	
	// implement the "static node()" method manually
	
	void ccTouchesBegan(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesEnded(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesCancelled(cocos2d::CCSet* touches,cocos2d::CCEvent* event);

	void backSelected(cocos2d::CCObject*sender);

	~InfoLayer();
};

//INFO SCENE
class InfoScene: public cocos2d::CCScene
{
public:
	InfoScene():_layer(NULL){};
	
	bool init();
	SCENE_NODE_FUNC(InfoScene);

	CC_SYNTHESIZE_READONLY(InfoLayer*, _layer, Layer);
	~InfoScene(){};
};
//SETTINGS LAYER
class ContinueLayer : public cocos2d::CCLayer
{
protected:
	cocos2d::CCMenu* LosMenu;
	cocos2d::CCMenuItemImage* BTNBackies;
	cocos2d::CCMenuItemImage* BTNPlay;
	cocos2d::CCMenuItemImage* BTNTutorial;

public:
	ContinueLayer();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	
	// implement the "static node()" method manually
	LAYER_NODE_FUNC(ContinueLayer);

	void ccTouchesBegan(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesEnded(cocos2d::CCSet* touches,cocos2d::CCEvent* event);
	void ccTouchesCancelled(cocos2d::CCSet* touches,cocos2d::CCEvent* event);

	void backSelected(cocos2d::CCObject*sender);
	void playSelected(cocos2d::CCObject*sender);
	void tutorialSelected(cocos2d::CCObject*sender);

	~ContinueLayer();
};

//SETTINGS SCENE
class ContinueScene: public cocos2d::CCScene
{
public:
	ContinueScene():_layer(NULL){};
	
	bool init();
	SCENE_NODE_FUNC(ContinueScene);

	CC_SYNTHESIZE_READONLY(ContinueLayer*, _layer, Layer);
	~ContinueScene(){};
};