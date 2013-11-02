#include "pch.h"
#include "Lvl1_Win.h"
#include "SimpleAudioEngine.h"

#define PTM_RATIO 256
#define ROOMSCALE 1.0

bool Lvl1_WinScene::init()
{
	if(CCScene::init()){
		this->_layer = Lvl1_WinLayer::node();
		this->_layer->retain();
		this->addChild(_layer);
		return true;
	}
	return false;
}

Lvl1_WinLayer::Lvl1_WinLayer(void):RoomLayer()
{
	scale = 0.5;
	roomHeight = 1865 * ROOMSCALE;//winSize.height * 1.05;
	roomWidth =  3880 * ROOMSCALE;//winSize.width*1.5;
}

void Lvl1_WinLayer::createCamera(){
	cameraView = new Camera(roomWidth, roomHeight, this);
}

void Lvl1_WinLayer::createWorld(){
	//Create gravity acceleration vector
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.8f);
	world = new b2World(gravity);
	world->SetContactListener(contactListener);
	world->SetDestructionListener(destructionListener);
}

void Lvl1_WinLayer::createGround(){
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0,0);
	groundBody = world->CreateBody(&groundBodyDef);

	b2FixtureDef groundBoxDef;

	b2PolygonShape doorBody;
	doorBody.SetAsBox((60/PTM_RATIO/2), (380/PTM_RATIO/2), b2Vec2(((3900 -30)*ROOMSCALE)/PTM_RATIO, ((300)*ROOMSCALE)/PTM_RATIO), 0);
	groundBoxDef.shape = &doorBody;
	groundBoxDef.isSensor = true;
	groundBoxDef.filter.categoryBits = BOUNDARY;
	groundBoxDef.filter.maskBits = HERO;

	doorFixture = groundBody->CreateFixture(&groundBoxDef);

	b2PolygonShape roomBody;
	b2Vec2 roomBodyCenter = b2Vec2(roomWidth/2/PTM_RATIO, (roomHeight + 500)/2/PTM_RATIO);
	roomBody.SetAsBox(roomWidth/2/PTM_RATIO, (roomHeight + 500 - winSize.height * (0.055))/2/PTM_RATIO, roomBodyCenter, 0.0f);
	groundBoxDef.shape = &roomBody;
	groundBoxDef.isSensor = true;
	groundBoxDef.filter.categoryBits = SENSOR;
	groundBoxDef.filter.maskBits = 0x0000;

	roomFixture = groundBody->CreateFixture(&groundBoxDef);
}

void Lvl1_WinLayer::createBackground(){
	//Creates the sprites for the background and forground
	winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite* bgSprite = CCSprite::spriteWithFile("Level1_RoomEndNoShack.jpg");
	bgSprite->setPosition(ccp(roomWidth/2,roomHeight/2));
	bgSprite->setScale(roomWidth/bgSprite->getContentSize().width);
	this->addChild(bgSprite);

	/**outputText = 0;
	outputLog->setPosition(ccp(100,winSize.height/2));
	outputLog->initWithString(outputText, "Arial", 30);*/
	
	/*this->addChild(outputLog);*/
}

void Lvl1_WinLayer::createHero(){
	CCPoint location = ccp(350*ROOMSCALE, 200*ROOMSCALE);
	hero = new Hero(world, this, PTM_RATIO, &location, 3, 3*scale);

	CCSprite* bgSprite = CCSprite::spriteWithFile("Level1_RoomEnd_Shack.png");
	bgSprite->setPosition(ccp((100 + 450)*ROOMSCALE, (200 + 250*1.5)*ROOMSCALE));
	bgSprite->setScale(1.5);
	this->addChild(bgSprite);
}

void Lvl1_WinLayer::createEnemies(){
	//(int i = 0; i < 2; i++){
	//enemies->addObject(new Spherodroid(world, this, PTM_RATIO, &ccp(5100 * ROOMSCALE, 350 * ROOMSCALE), scale));
	//}
	//enemies->addObject(new Spherodroid(world, this, PTM_RATIO, &ccp(500, 900)));
}

void Lvl1_WinLayer::createPowerups(){
	powerups->addObject(new Powerup(this, world, PTM_RATIO, 3224/scale, 200/scale, false, scale));
}

void Lvl1_WinLayer::createBoxes(){
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 0, 6600, 200, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 0, 100, 650, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 6600, 0, 50, 650, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 1500, 6700, 100, false, ROOMSCALE));
}

void Lvl1_WinLayer::createPortals(){
	phaseBox->addObject(new PhaseBox(this, world, PTM_RATIO, 2700, 200, 40, 1500, false, ROOMSCALE));
}

void Lvl1_WinLayer::goToNextRoom(){
	MainMenuScene * nextRoom = MainMenuScene::node();
	CCDirector::sharedDirector()->replaceScene(nextRoom);
}

Lvl1_WinLayer::~Lvl1_WinLayer(void)
{
}
