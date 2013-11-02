#include "pch.h"
#include "MainMenu.h"
#include "DemoRoom.h"
#include "Lvl1_Win.h"
USING_NS_CC;

#define winWidth = 1920
#define winHeight = 1080

//MAIN MENU LAYER
MainMenuLayer::MainMenuLayer(){}
void MainMenuLayer::ccTouchesBegan(CCSet* touches, CCEvent* event){}
void MainMenuLayer::ccTouchesMoved(CCSet* touches, CCEvent* event){}
void MainMenuLayer::ccTouchesEnded(CCSet* touches, CCEvent* event){}
void MainMenuLayer::ccTouchesCancelled(CCSet* touches, CCEvent* event){}
bool MainMenuLayer::init() {

	if ( !CCLayer::init() )
	{
		return false;
	}
	this->setIsTouchEnabled(true);
	CCSprite *bgSprite = CCSprite::spriteWithFile("test_background.jpg");
	bgSprite->setPosition( ccp(1920/2.0,  1080/2.0) );

	bgSprite->setScaleX(1920/bgSprite->getContentSize().width);
	bgSprite->setScaleY(1080/bgSprite->getContentSize().height);

	this->addChild(bgSprite);
	BTNContinue = CCMenuItemImage::itemFromNormalImage("Continue.png","Selected.png", this,  menu_selector(MainMenuLayer::continueSelected));
	BTNLevelSelect = CCMenuItemImage::itemFromNormalImage("LevelSelect.png","Selected.png", this,  menu_selector(MainMenuLayer::levelSelectSelected));
	BTNInfo = CCMenuItemImage::itemFromNormalImage("Instructions.png","Selected.png", this,  menu_selector(MainMenuLayer::infoSelected));
	BTNAbout = CCMenuItemImage::itemFromNormalImage("About.png","Selected.png", this,  menu_selector(MainMenuLayer::aboutSelected));
	BTNSettings = CCMenuItemImage::itemFromNormalImage("Settings.png","Selected.png", this,  menu_selector(MainMenuLayer::settingsSelected));
	ElMenu = CCMenu::menuWithItem(BTNContinue);
	ElMenu->addChild(BTNLevelSelect, 1);
	ElMenu->addChild(BTNInfo, 2);
	ElMenu->addChild(BTNAbout,3);
	ElMenu->addChild(BTNSettings, 4);
	ElMenu->setScale(0.34);
	ElMenu->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width/2)*0.34, (CCDirector::sharedDirector()->getWinSize().height/2) *0.34));
	ElMenu->alignItemsVertically();
	
	this->addChild(ElMenu);
	return true;
}
void MainMenuLayer::continueSelected(cocos2d::CCObject*sender) {
	ContinueScene* ContinueScene = ContinueScene::node();
	CCDirector::sharedDirector()->replaceScene(ContinueScene);
}
void MainMenuLayer::levelSelectSelected(cocos2d::CCObject*sender) {
	LevelSelectScene* LevelSelectScene = LevelSelectScene::node();
	CCDirector::sharedDirector()->replaceScene(LevelSelectScene);
}
void MainMenuLayer::infoSelected(cocos2d::CCObject*sender) {
	InfoScene* InfoScene = InfoScene::node();
	CCDirector::sharedDirector()->replaceScene(InfoScene);
}
void MainMenuLayer::aboutSelected(cocos2d::CCObject*sender) {
	AboutScene* AboutScene = AboutScene::node();
	CCDirector::sharedDirector()->replaceScene(AboutScene);
}
void MainMenuLayer::settingsSelected(cocos2d::CCObject*sender) {
	SettingsScene* SettingsScene = SettingsScene::node();
	CCDirector::sharedDirector()->replaceScene(SettingsScene);
}
MainMenuLayer::~MainMenuLayer(void){}

//MAIN MENU SCENE
bool MainMenuScene::init()
{
	if(CCScene::init()){
		this->_layer = MainMenuLayer::node();
		this->_layer->retain();
		this->addChild(_layer);
		//this->runAction(CCFollow::actionWithTarget(_layer->getCameraView()));
		return true;
	}
	return false;
}

//SETTINGS LAYER
SettingsLayer::SettingsLayer(){}
bool SettingsLayer::init() {

	if ( !CCLayer::init() )
	{
		return false;
	}

	this->setIsTouchEnabled(true);
	CCSprite *bgSprite = CCSprite::spriteWithFile("test_background.jpg");
	bgSprite->setPosition( ccp(1920/2.0,  1080/2.0) );

	bgSprite->setScaleX(1920/bgSprite->getContentSize().width);
	bgSprite->setScaleY(1080/bgSprite->getContentSize().height);

	this->addChild(bgSprite);

	this->setIsTouchEnabled(true);
	BTNBackies = CCMenuItemImage::itemFromNormalImage("Back.png","Selected.png", this,  menu_selector(SettingsLayer::backSelected));
	LeMenu = CCMenu::menuWithItem(BTNBackies);
	//ElMenu->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width/2), (CCDirector::sharedDirector()->getWinSize().height/2) ));
	LeMenu->setScale(0.34);
	LeMenu->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width/2)*0.34, (CCDirector::sharedDirector()->getWinSize().height/2) *0.34));
	
	this->addChild(LeMenu);
}
void SettingsLayer::backSelected(cocos2d::CCObject*sender) {
	MainMenuScene* MainMenuScene = MainMenuScene::node();
	CCDirector::sharedDirector()->replaceScene(MainMenuScene);
}
void SettingsLayer::ccTouchesBegan(CCSet* touches, CCEvent* event){}
void SettingsLayer::ccTouchesMoved(CCSet* touches, CCEvent* event){}
void SettingsLayer::ccTouchesEnded(CCSet* touches, CCEvent* event){}
void SettingsLayer::ccTouchesCancelled(CCSet* touches, CCEvent* event){}

SettingsLayer::~SettingsLayer() {}

//SETTINGS SCENE
bool SettingsScene::init()
{
	if(CCScene::init()){
		this->_layer = SettingsLayer::node();
		this->_layer->retain();
		this->addChild(_layer);
		//this->runAction(CCFollow::actionWithTarget(_layer->getCameraView()));
		return true;
	}
	return false;
}

//ABOOT LAYER
AboutLayer::AboutLayer(){}
bool AboutLayer::init() {

	if ( !CCLayer::init() )
	{
		return false;
	}

	this->setIsTouchEnabled(true);
	CCSprite *bgSprite = CCSprite::spriteWithFile("test_background.jpg");
	bgSprite->setPosition( ccp(1920/2.0,  1080/2.0) );

	bgSprite->setScaleX(1920/bgSprite->getContentSize().width);
	bgSprite->setScaleY(1080/bgSprite->getContentSize().height);

	this->addChild(bgSprite);

	this->setIsTouchEnabled(true);
	BTNBackies = CCMenuItemImage::itemFromNormalImage("Back.png","Selected.png", this,  menu_selector(AboutLayer::backSelected));
	DasMenu = CCMenu::menuWithItem(BTNBackies);
	DasMenu->setScale(0.34);
	DasMenu->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width/2)*0.34, (CCDirector::sharedDirector()->getWinSize().height/2) *0.34));
	CCLabelTTF *description = new CCLabelTTF();
	description->initWithString("We are coders 'n art-ers and we made this.\nProps to us for being awesome.","Arial", 30);
	description->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width)*0.34, (1000) *0.34));
	this->addChild(DasMenu);
	this->addChild(description);
}
void AboutLayer::backSelected(cocos2d::CCObject*sender) {
	MainMenuScene* MainMenuScene = MainMenuScene::node();
	CCDirector::sharedDirector()->replaceScene(MainMenuScene);
}
void AboutLayer::ccTouchesBegan(CCSet* touches, CCEvent* event){}
void AboutLayer::ccTouchesMoved(CCSet* touches, CCEvent* event){}
void AboutLayer::ccTouchesEnded(CCSet* touches, CCEvent* event){}
void AboutLayer::ccTouchesCancelled(CCSet* touches, CCEvent* event){}

AboutLayer::~AboutLayer() {}

//ABOOT SCENE
bool AboutScene::init()
{
	if(CCScene::init()){
		this->_layer = AboutLayer::node();
		this->_layer->retain();
		this->addChild(_layer);
		return true;
	}
	return false;
}

//LEVELSELECT LAYER
LevelSelectLayer::LevelSelectLayer(){}
bool LevelSelectLayer::init() {
	int i = 1;
	if ( !CCLayer::init() )
	{
		return false;
	}

	this->setIsTouchEnabled(true);
	CCSprite *bgSprite = CCSprite::spriteWithFile("test_background.jpg");
	bgSprite->setPosition( ccp(1920/2.0,  1080/2.0) );

	bgSprite->setScaleX(1920/bgSprite->getContentSize().width);
	bgSprite->setScaleY(1080/bgSprite->getContentSize().height);

	this->addChild(bgSprite);

	this->setIsTouchEnabled(true);
	BTNBackies = CCMenuItemImage::itemFromNormalImage("Back.png","Selected.png", this,  menu_selector(LevelSelectLayer::backSelected));
	BTNDemo = CCMenuItemImage::itemFromNormalImage("BTN0_Demo.png","Selected.png", this,  menu_selector(LevelSelectLayer::demoLevelGo));
	BTNTutorial = CCMenuItemImage::itemFromNormalImage("BTN0_Tutorial.png","Selected.png", this,  menu_selector(LevelSelectLayer::tutorialLevelGo));
	BTNLevel1_1 = CCMenuItemImage::itemFromNormalImage("BTN1_1.png","Selected.png", this,  menu_selector(LevelSelectLayer::level1_1Go));
	BTNLevel1_2 = CCMenuItemImage::itemFromNormalImage("BTN1_2.png","Selected.png", this,  menu_selector(LevelSelectLayer::level1_2Go));
	BTNLevel1_3 = CCMenuItemImage::itemFromNormalImage("BTN1_3.png","Selected.png", this,  menu_selector(LevelSelectLayer::level1_3Go));
	BTNLevel1_4 = CCMenuItemImage::itemFromNormalImage("BTN1_4.png","Selected.png", this,  menu_selector(LevelSelectLayer::level1_4Go));
	BTNLevel1_Win = CCMenuItemImage::itemFromNormalImage("BTN1_Win.png","Selected.png", this,  menu_selector(LevelSelectLayer::level1_WinGo));
	menu0 = CCMenu::menuWithItem(BTNTutorial);
	menu0->addChild(BTNDemo, 1);
	menu1 = CCMenu::menuWithItem(BTNLevel1_1);
	menu1->addChild(BTNLevel1_2, 1);
	menu1->addChild(BTNLevel1_3, 2);
	menu1->addChild(BTNLevel1_4, 3);
	menu1->addChild(BTNLevel1_Win, 4);
	menuBack = CCMenu::menuWithItem(BTNBackies);
	menu0->alignItemsHorizontally();
	menu1->alignItemsHorizontally();
	menuBack->alignItemsHorizontally();
	menu0->setScale(0.34);
	menu1->setScale(0.34);
	menuBack->setScale(0.34);
	menuBack->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width/2)*0.34, (CCDirector::sharedDirector()->getWinSize().height/2) *0.34));
	menu1->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width/2)*0.34, (CCDirector::sharedDirector()->getWinSize().height/2) *0.34+130));
	menu0->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width/2)*0.34, (CCDirector::sharedDirector()->getWinSize().height/2) *0.34+260));
	this->addChild(menu0);
	this->addChild(menu1);
	this->addChild(menuBack);
}
void LevelSelectLayer::demoLevelGo(cocos2d::CCObject*sender) {
	DemoRoomScene* DemoRoomScene = DemoRoomScene::node();
	CCDirector::sharedDirector()->replaceScene(DemoRoomScene);
}
void LevelSelectLayer::tutorialLevelGo(cocos2d::CCObject*sender) {
	TutorialRoomScene* TutorialRoomScene = TutorialRoomScene::node();
	CCDirector::sharedDirector()->replaceScene(TutorialRoomScene);
}
void LevelSelectLayer::level1_1Go(cocos2d::CCObject*sender) {
	Lvl1_1Scene* Lvl1_1 = Lvl1_1Scene::node();
	CCDirector::sharedDirector()->replaceScene(Lvl1_1);
}
void LevelSelectLayer::level1_2Go(cocos2d::CCObject*sender) {
	Lvl1_2Scene* Lvl1_2 = Lvl1_2Scene::node();
	CCDirector::sharedDirector()->replaceScene(Lvl1_2);
}
void LevelSelectLayer::level1_3Go(cocos2d::CCObject*sender) {
	Lvl1_3Scene* Lvl1_3 = Lvl1_3Scene::node();
	CCDirector::sharedDirector()->replaceScene(Lvl1_3);
}
void LevelSelectLayer::level1_4Go(cocos2d::CCObject*sender) {
	Lvl1_4Scene* Lvl1_4 = Lvl1_4Scene::node();
	CCDirector::sharedDirector()->replaceScene(Lvl1_4);
}
void LevelSelectLayer::level1_WinGo(cocos2d::CCObject*sender) {
	Lvl1_WinScene* Lvl1_Win = Lvl1_WinScene::node();
	CCDirector::sharedDirector()->replaceScene(Lvl1_Win);
}
void LevelSelectLayer::backSelected(cocos2d::CCObject*sender) {
	MainMenuScene* MainMenuScene = MainMenuScene::node();
	CCDirector::sharedDirector()->replaceScene(MainMenuScene);
}
void LevelSelectLayer::ccTouchesBegan(CCSet* touches, CCEvent* event){}
void LevelSelectLayer::ccTouchesMoved(CCSet* touches, CCEvent* event){}
void LevelSelectLayer::ccTouchesEnded(CCSet* touches, CCEvent* event){}
void LevelSelectLayer::ccTouchesCancelled(CCSet* touches, CCEvent* event){}

LevelSelectLayer::~LevelSelectLayer() {}

//LEVELSELECT SCENE
bool LevelSelectScene::init()
{
	if(CCScene::init()){
		this->_layer = LevelSelectLayer::node();
		this->_layer->retain();
		this->addChild(_layer);
		return true;
	}
	return false;
}

//INFO LAYER
InfoLayer::InfoLayer(){}
bool InfoLayer::init() {

	if ( !CCLayer::init() )
	{
		return false;
	}

	this->setIsTouchEnabled(true);
	CCSprite *bgSprite = CCSprite::spriteWithFile("test_background.jpg");
	bgSprite->setPosition( ccp(1920/2.0,  1080/2.0) );

	bgSprite->setScaleX(1920/bgSprite->getContentSize().width);
	bgSprite->setScaleY(1080/bgSprite->getContentSize().height);

	this->addChild(bgSprite);

	this->setIsTouchEnabled(true);
	BTNBackies = CCMenuItemImage::itemFromNormalImage("Back.png","Selected.png", this,  menu_selector(LevelSelectLayer::backSelected));
	InfoText  = CCSprite::spriteWithFile("InstructionsText.png");
	DerMenu = CCMenu::menuWithItem(BTNBackies);
	DerMenu->alignItemsVertically();
	DerMenu->setScale(0.34);
	DerMenu->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width/2)*0.34, (CCDirector::sharedDirector()->getWinSize().height/2) *0.34));
	//InfoText->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width)*0.34, (1000) *0.34));
	InfoText->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width)*0.34, (CCDirector::sharedDirector()->getWinSize().height*2+(CCDirector::sharedDirector()->getWinSize().height/4)) *0.34));
	this->addChild(DerMenu);
	this->addChild(InfoText);
}
void InfoLayer::backSelected(cocos2d::CCObject*sender) {
	MainMenuScene* MainMenuScene = MainMenuScene::node();
	CCDirector::sharedDirector()->replaceScene(MainMenuScene);
}
void InfoLayer::ccTouchesBegan(CCSet* touches, CCEvent* event){}
void InfoLayer::ccTouchesMoved(CCSet* touches, CCEvent* event){}
void InfoLayer::ccTouchesEnded(CCSet* touches, CCEvent* event){}
void InfoLayer::ccTouchesCancelled(CCSet* touches, CCEvent* event){}

InfoLayer::~InfoLayer() {}

//INFO SCENE
bool InfoScene::init()
{
	if(CCScene::init()){
		this->_layer = InfoLayer::node();
		this->_layer->retain();
		this->addChild(_layer);
		return true;
	}
	return false;
}

//CONTINUE LAYER
ContinueLayer::ContinueLayer(){}
bool ContinueLayer::init() {

	if ( !CCLayer::init() )
	{
		return false;
	}

	this->setIsTouchEnabled(true);
	CCSprite *bgSprite = CCSprite::spriteWithFile("test_background.jpg");
	bgSprite->setPosition( ccp(1920/2.0,  1080/2.0) );

	bgSprite->setScaleX(1920/bgSprite->getContentSize().width);
	bgSprite->setScaleY(1080/bgSprite->getContentSize().height);

	this->addChild(bgSprite);

	this->setIsTouchEnabled(true);
	BTNBackies = CCMenuItemImage::itemFromNormalImage("Back.png","Selected.png", this,  menu_selector(ContinueLayer::backSelected));
	BTNPlay = CCMenuItemImage::itemFromNormalImage("Play.png","Selected.png", this,  menu_selector(ContinueLayer::playSelected));
	BTNTutorial = CCMenuItemImage::itemFromNormalImage("Tutorial.png","Selected.png", this,  menu_selector(ContinueLayer::tutorialSelected));
	LosMenu = CCMenu::menuWithItem(BTNTutorial);
	LosMenu->addChild(BTNPlay, 1);
	CCMenu* BackMenu = CCMenu::menuWithItem(BTNBackies);
	LosMenu->alignItemsHorizontally();
	LosMenu->setScale(0.34);
	BackMenu->setScale(0.34);
	LosMenu->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width/2)*0.34, (CCDirector::sharedDirector()->getWinSize().height/2) *0.34));
	BackMenu->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width/2)*0.34, (CCDirector::sharedDirector()->getWinSize().height/2) *0.34 - 130));
	this->addChild(LosMenu);
	this->addChild(BackMenu);
}
void ContinueLayer::playSelected(cocos2d::CCObject*sender) {
	IntroScene* IntroScene = IntroScene::node();
	IntroScene->setFollowingLevel(1);
	CCDirector::sharedDirector()->replaceScene(IntroScene);
}
void ContinueLayer::tutorialSelected(cocos2d::CCObject*sender) {
	IntroScene* IntroScene = IntroScene::node();
	IntroScene->setFollowingLevel(0);
	CCDirector::sharedDirector()->replaceScene(IntroScene);/*
	TutorialRoomScene* TutorialRoomScene = TutorialRoomScene::node();
	CCDirector::sharedDirector()->replaceScene(TutorialRoomScene);*/
}
void ContinueLayer::backSelected(cocos2d::CCObject*sender) {
	MainMenuScene* MainMenuScene = MainMenuScene::node();
	CCDirector::sharedDirector()->replaceScene(MainMenuScene);
}
void ContinueLayer::ccTouchesBegan(CCSet* touches, CCEvent* event){}
void ContinueLayer::ccTouchesMoved(CCSet* touches, CCEvent* event){}
void ContinueLayer::ccTouchesEnded(CCSet* touches, CCEvent* event){}
void ContinueLayer::ccTouchesCancelled(CCSet* touches, CCEvent* event){}

ContinueLayer::~ContinueLayer() {}

//CONTINUE SCENE
bool ContinueScene::init()
{
	if(CCScene::init()){
		this->_layer = ContinueLayer::node();
		this->_layer->retain();
		this->addChild(_layer);
		return true;
	}
	return false;
}