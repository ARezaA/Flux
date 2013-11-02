#include "pch.h"
#include "Intro.h"
#include "DemoRoom.h"
#include "Lvl1_1.h"
#include "Lvl1_Win.h"
#include "Game.h"
#include "SimpleAudioEngine.h"
#include "TutorialRoom.h"
USING_NS_CC;

#define winWidth = 1920
#define winHeight = 1080

//MAIN MENU LAYER
IntroLayer::IntroLayer(): cineText(0), followingLevel(1){
init();
}
void IntroLayer::ccTouchesBegan(CCSet* touches, CCEvent* event){
	cineText++;
}
void IntroLayer::ccTouchesMoved(CCSet* touches, CCEvent* event){}
void IntroLayer::ccTouchesEnded(CCSet* touches, CCEvent* event){
	
	switch(cineText){
	case 0:
		break;
	case 1:
		text1Go();
		break;
	case 2:
		text2Go();
		break;
	case 3:
		text3Go();
		break;
	case 4:
		text4Go();
		break;
	case 5:
		text5Go();
		break;
	case 6:
		text6Go();
		break;
	case 7:
		tutorialGo();
		break;

	}


}
void IntroLayer::ccTouchesCancelled(CCSet* touches, CCEvent* event){}
bool IntroLayer::init() {

	if ( !CCLayer::init() )
	{
		return false;
	}
	this->setIsTouchEnabled(true);
	CCSprite *bgSprite = CCSprite::spriteWithFile("test_background.jpg");
	bgSprite->setPosition( ccp(1920/2.0,  1080/2.0) );
	CCSprite *introbgSprite = CCSprite::spriteWithFile("IntroBG.png");
	introbgSprite->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width/2), (CCDirector::sharedDirector()->getWinSize().height/2)));
	introbgSprite->setScale(0.34);
	bgSprite->setScaleX(1920/bgSprite->getContentSize().width);
	bgSprite->setScaleY(1080/bgSprite->getContentSize().height);

	text = CCSprite::spriteWithFile("Intro0.png");
	text->setScale(0.34);
	text->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width/2), (CCDirector::sharedDirector()->getWinSize().height/2)));
	this->addChild(bgSprite);
	this->addChild(introbgSprite);
	this->addChild(text);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.9);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds//FLuXOST_Tutorial.mp3", true);
	return true;
}
void IntroLayer::text1Go() {
	this->removeChild(text, true);
	text = CCSprite::spriteWithFile("Intro1.png");
	text->setScale(0.34);
	text->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));
	this->addChild(text);
}
void IntroLayer::text2Go() {
	this->removeChild(text, true);
	text = CCSprite::spriteWithFile("Intro2.png");
	text->setScale(0.34);
	text->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));
	this->addChild(text);
}
void IntroLayer::text3Go() {
	this->removeChild(text, true);
	text = CCSprite::spriteWithFile("Intro3.png");
	text->setScale(0.34);
	text->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));
	this->addChild(text);
}
void IntroLayer::text4Go() {
	this->removeChild(text, true);
	text = CCSprite::spriteWithFile("Intro4.png");
	text->setScale(0.34);
	text->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));
	this->addChild(text);
}
void IntroLayer::text5Go() {
	this->removeChild(text, true);
	text = CCSprite::spriteWithFile("Intro5.png");
	text->setScale(0.34);
	text->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));
	this->addChild(text);
}
void IntroLayer::text6Go() {
	this->removeChild(text, true);
	text = CCSprite::spriteWithFile("Intro6.png");
	text->setScale(0.34);
	text->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));
	this->addChild(text);
}
void IntroLayer::tutorialGo() {
	if(followingLevel == 1){
		Lvl1_1Scene* Lvl1_1Scene = Lvl1_1Scene::node();
		CCDirector::sharedDirector()->replaceScene(Lvl1_1Scene);
	}
	else {
		TutorialRoomScene* TutorialRoomScene = TutorialRoomScene::node();
		CCDirector::sharedDirector()->replaceScene(TutorialRoomScene);
	}
}
IntroLayer::~IntroLayer(void){}

//MAIN MENU SCENE
bool IntroScene::init()
{
	if(CCScene::init()){
		this->_layer = IntroLayer::node();
		this->_layer->retain();
		this->addChild(_layer);
		//this->runAction(CCFollow::actionWithTarget(_layer->getCameraView()));
		return true;
	}
	return false;
}
void IntroScene::setFollowingLevel(int level){
	this->_layer->setFollowingLevel(level);
}
void IntroLayer::setFollowingLevel(int level){
	followingLevel = level;
}