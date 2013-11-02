#include "pch.h"
#include "Lvl1_3.h"
#include "SimpleAudioEngine.h"

#define PTM_RATIO 256
#define ROOMSCALE 1.0

bool Lvl1_3Scene::init()
{
	if(CCScene::init()){
		this->_layer = Lvl1_3Layer::node();
		this->_layer->retain();
		this->addChild(_layer);
		return true;
	}
	return false;
}

Lvl1_3Layer::Lvl1_3Layer(void):RoomLayer()
{
	scale = 0.5;
	roomHeight = 2400;
	roomWidth = 5698;
}

void Lvl1_3Layer::createCamera(){
	cameraView = new Camera(roomWidth, roomHeight, this);
}

void Lvl1_3Layer::createWorld(){
	//Create gravity acceleration vector
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.8f);
	world = new b2World(gravity);
	world->SetContactListener(contactListener);
	world->SetDestructionListener(destructionListener);
}

void Lvl1_3Layer::createGround(){
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0,0);
	groundBody = world->CreateBody(&groundBodyDef);
	
	b2FixtureDef groundBoxDef;

	b2PolygonShape roomBody;
	b2Vec2 roomBodyCenter = b2Vec2(roomWidth/2/PTM_RATIO, roomHeight/2/PTM_RATIO);
	roomBody.SetAsBox(roomWidth/2/PTM_RATIO, (roomHeight - winSize.height * (0.055))/2/PTM_RATIO, roomBodyCenter, 0.0f);
	groundBoxDef.shape = &roomBody;
	groundBoxDef.isSensor = true;
	groundBoxDef.filter.categoryBits = SENSOR;
	groundBoxDef.filter.maskBits = 0x0000;

	roomFixture = groundBody->CreateFixture(&groundBoxDef);

	b2PolygonShape doorBody;
	doorBody.SetAsBox((300/PTM_RATIO/2), (50/PTM_RATIO/2), b2Vec2(((5100.0 + 150)*ROOMSCALE)/PTM_RATIO, ((-200.0)*ROOMSCALE)/PTM_RATIO), 0);
	groundBoxDef.shape = &doorBody;
	groundBoxDef.isSensor = true;
	groundBoxDef.filter.categoryBits = BOUNDARY;
	groundBoxDef.filter.maskBits = HERO;

	doorFixture = groundBody->CreateFixture(&groundBoxDef);

}

void Lvl1_3Layer::createBackground(){
	//Creates the sprites for the background and forground
	winSize = CCDirector::sharedDirector()->getWinSize();

	CCSprite* bgSprite = CCSprite::spriteWithFile("Level1_Room3.jpg");

	bgSprite->setPosition( ccp(roomWidth/2, roomHeight/2));
	bgSprite->setScale(roomWidth/bgSprite->getContentSize().width);
	this->addChild(bgSprite);

	/**outputText = 0;
	outputLog->setPosition(ccp(100,winSize.height/2));
	outputLog->initWithString(outputText, "Arial", 30);
	
	this->addChild(outputLog);*/
}

void Lvl1_3Layer::createHero(){
	CCPoint location = ccp(450*scale, 1300);
	hero = new Hero(world, this, PTM_RATIO, &location, 3, 3*scale);
}

void Lvl1_3Layer::createEnemies(){
	//(int i = 0; i < 2; i++){
		enemies->addObject(new Guard(world, this, PTM_RATIO, &ccp(900, 1300), scale));
	//}
	for(int i =0; i<3; i++){
		enemies->addObject(new Spherodroid(world, this, PTM_RATIO, &ccp(1200 + (900*i), 1800), scale));
	}
}

void Lvl1_3Layer::createPowerups(){
	//powerups->addObject(new Powerup(this, world, PTM_RATIO, 1500, 200, scale));
}

void Lvl1_3Layer::createBoxes(){
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 0, 4900, 1200, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 4650, 0, 5100-4650, 1500, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 5400, 0, 5698-5400, 2400, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 4650, 1820, 5600-4650, 2400 - 1800, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 2120, 4750, 2400 - 2100, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 1670, 600, 2200 - 1650, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 1520, 100, 1750 - 1500, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 500, 1520, 100, 1750 - 1500, false, ROOMSCALE));

	enemyColBox->addObject(new EnemyColBox(this,world,PTM_RATIO, 800, 1200, 10, 900, false, ROOMSCALE));
	enemyColBox->addObject(new EnemyColBox(this,world,PTM_RATIO, 1300, 1200, 20, 900, false, ROOMSCALE));
	enemyColBox->addObject(new EnemyColBox(this,world,PTM_RATIO, 2600, 1200, 20, 900, false, ROOMSCALE));
	enemyColBox->addObject(new EnemyColBox(this,world,PTM_RATIO, 4050, 1200, 20, 900, false, ROOMSCALE));
}

void Lvl1_3Layer::createPortals(){
	//portalBox->addObject(new Portal(this, world, PTM_RATIO, 870, 505, 60, 160, 870 - 60 - 100, 505 + 80, 870+60+100, 505+80, false, true, scale));
}

void Lvl1_3Layer::goToNextRoom(){
	enemies->getObjectAtIndex(0)->setTagID(10);
	Lvl1_4Scene* nextRoom = Lvl1_4Scene::node();
	CCDirector::sharedDirector()->replaceScene(nextRoom);
}

Lvl1_3Layer::~Lvl1_3Layer(void)
{
}
