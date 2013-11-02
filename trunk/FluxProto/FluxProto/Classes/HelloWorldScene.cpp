#include "HelloWorldScene.h"
#include "Room.h"
#include "MainMenu.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
HelloWorld::HelloWorld(){
	
}

CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();


	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::node();


	// add layer as a child to scene
	scene->addChild(layer);


	// return the scene
	return scene;
}


// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}





	/////////////////////////////
	// 3. add your codes below...


	// add a label shows "Hello World"
	// create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::labelWithString("Click to Start", "Arial", 46);
	// ask director the window size
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	// position the label on the center of the screen
	pLabel->setPosition( ccp(size.width / 2, 100) );


	// add the label as a child to this layer
	this->addChild(pLabel, 1);


	// add "HelloWorld" splash screen"
	CCSprite* bgSprite = CCSprite::spriteWithFile("test_background.jpg");

	// position the sprite on the center of the screen
	bgSprite->setPosition( ccp(size.width/2, size.height/2) );


	// add the sprite as a child to this layer
	this->addChild(bgSprite, 0);

	// add "HelloWorld" splash screen"
	CCSprite* pSprite = CCSprite::spriteWithFile("FluxLogoLarge.png");


	// position the sprite on the center of the screen
	pSprite->setPosition( ccp(size.width/2, size.height/2) );

	//CCSize* logoSize = new CCSize(size.width/4, ((pSprite->getContentSize().height/pSprite->getContentSize().width) * size.width * 0.25));

	//pSprite->setContentSizeInPixels(*logoSize);
	pSprite->setScale((size.width * 0.75)/pSprite->getContentSize().width);

	// add the sprite as a child to this layer
	this->addChild(pSprite, 0);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("FluX.mp3", true);

	this->setIsTouchEnabled(true);
	this->setIsKeypadEnabled(true);


	return true;
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event){
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	MainMenuScene* RoomScene = MainMenuScene::node();
	CCDirector::sharedDirector()->replaceScene(RoomScene);
}