#include "pch.h"
#include "TutorialRoom.h"
#include "SimpleAudioEngine.h"

#define PTM_RATIO 256
#define ROOMSCALE 1.8

bool TutorialRoomScene::init()
{
	if(CCScene::init()){
		this->_layer = TutorialRoomLayer::node();
		this->_layer->retain();
		this->addChild(_layer);
		return true;
	}
	return false;
}

TutorialRoomLayer::TutorialRoomLayer(void):RoomLayer()
{
	scale = 0.5;
	roomHeight = 600 * ROOMSCALE;//winSize.height * 1.05;
	roomWidth =  6600 * ROOMSCALE;//winSize.width*1.5;
}

void TutorialRoomLayer::createCamera(){
	cameraView = new Camera(roomWidth, roomHeight, this);
}

void TutorialRoomLayer::createWorld(){
	//Create gravity acceleration vector
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.8f);
	world = new b2World(gravity);
	world->SetContactListener(contactListener);
	world->SetDestructionListener(destructionListener);
}

void TutorialRoomLayer::createGround(){
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0,0);
	groundBody = world->CreateBody(&groundBodyDef);

	b2FixtureDef groundBoxDef;

	b2PolygonShape doorBody;
	doorBody.SetAsBox((60/PTM_RATIO/2), (380/PTM_RATIO/2), b2Vec2(((6600 -30)*ROOMSCALE)/PTM_RATIO, ((300)*ROOMSCALE)/PTM_RATIO), 0);
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

void TutorialRoomLayer::createBackground(){
	//Creates the sprites for the background and forground
	winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite* bgSprite = CCSprite::spriteWithFile("Level0_Tutorial.jpg");
	bgSprite->setPosition(ccp(roomWidth/2,roomHeight/2));
	bgSprite->setScale(roomWidth/bgSprite->getContentSize().width);
	this->addChild(bgSprite);

	/**outputText = 0;
	outputLog->setPosition(ccp(100,winSize.height/2));
	outputLog->initWithString(outputText, "Arial", 30);*/
	
	/*this->addChild(outputLog);*/
}

void TutorialRoomLayer::createHero(){
	CCPoint location = ccp(200*ROOMSCALE, 200*ROOMSCALE);
	hero = new Hero(world, this, PTM_RATIO, &location, 3, 3*scale);
}

void TutorialRoomLayer::createEnemies(){
	//(int i = 0; i < 2; i++){
	enemies->addObject(new Spherodroid(world, this, PTM_RATIO, &ccp(5100 * ROOMSCALE, 350 * ROOMSCALE), scale));
	//}
	//enemies->addObject(new Spherodroid(world, this, PTM_RATIO, &ccp(500, 900)));
}

void TutorialRoomLayer::createPowerups(){
	//powerups->addObject(new Powerup(this, world, PTM_RATIO, 1500*ROOMSCALE, 300 *ROOMSCALE, scale));
}
void TutorialRoomLayer::createBGMusic(){
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.9);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds//FLuXOST_Tutorial.mp3", true);
}
void TutorialRoomLayer::createBoxes(){
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 0, 2590, 195, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 2725, 0, 6600 - 2850, 195, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 0, 100, 650, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 6600, 0, 50, 650, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 600, 6700, 100, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 2850, 150, 100, 350-200, false, ROOMSCALE));

	trapBox->addObject(new TrapBox(this, world, PTM_RATIO, 2500, -100, 400, 20, false, ROOMSCALE));

	enemyColBox->addObject(new EnemyColBox(this,world,PTM_RATIO, 4440, 200, 20, 400, false, ROOMSCALE));
	enemyColBox->addObject(new EnemyColBox(this,world,PTM_RATIO, 5200, 200, 20, 400, false, ROOMSCALE));
}

void TutorialRoomLayer::createPortals(){
	phaseBox->addObject(new PhaseBox(this, world, PTM_RATIO, 4235, 200, 40, 400, false, ROOMSCALE));
}

void TutorialRoomLayer::goToNextRoom(){
	enemies->getObjectAtIndex(0)->setTagID(10);
	Lvl1_1Scene* Lvl1_1 = Lvl1_1Scene::node();
	CCDirector::sharedDirector()->replaceScene(Lvl1_1);
}

TutorialRoomLayer::~TutorialRoomLayer(void)
{
}
