#include "pch.h"
#include "Lvl1_1.h"
#include "SimpleAudioEngine.h"

#define PTM_RATIO 256

bool Lvl1_1Scene::init()
{
	if(CCScene::init()){
		this->_layer = Lvl1_1Layer::node();
		this->_layer->retain();
		this->addChild(_layer);
		return true;
	}
	return false;
}

Lvl1_1Layer::Lvl1_1Layer(void):RoomLayer()
{
	scale = 0.5;
	roomHeight = winSize.height * 1.05;
	roomWidth = winSize.width*1.15;
}

void Lvl1_1Layer::createCamera(){
	cameraView = new Camera(roomWidth, roomHeight, this);
}

void Lvl1_1Layer::createWorld(){
	//Create gravity acceleration vector
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.8f);
	world = new b2World(gravity);
	world->SetContactListener(contactListener);
	world->SetDestructionListener(destructionListener);
}

void Lvl1_1Layer::createGround(){
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0,0);
	groundBody = world->CreateBody(&groundBodyDef);

	b2FixtureDef groundBoxDef;

	b2PolygonShape roomBody;
	b2Vec2 roomBodyCenter = b2Vec2(winSize.width/PTM_RATIO, (winSize.height + 100)/2/PTM_RATIO);
	roomBody.SetAsBox(winSize.width/PTM_RATIO, (winSize.height * (1-0.055))/PTM_RATIO/2, roomBodyCenter, 0.0f);
	groundBoxDef.shape = &roomBody;
	groundBoxDef.isSensor = true;
	groundBoxDef.filter.categoryBits = SENSOR;
	groundBoxDef.filter.maskBits = 0x0000;

	roomFixture = groundBody->CreateFixture(&groundBoxDef);

	b2PolygonShape doorBody;
	b2Vec2* doorBodyCenter = new b2Vec2(((4122+30)*scale)/PTM_RATIO, ((105 + 750/2)*scale)/PTM_RATIO);
	doorBody.SetAsBox(30/PTM_RATIO, 375/PTM_RATIO, *doorBodyCenter, 0.0f);
	groundBoxDef.shape = &doorBody;
	groundBoxDef.isSensor = true;
	groundBoxDef.filter.categoryBits = BOUNDARY;
	groundBoxDef.filter.maskBits = HERO;

	doorFixture = groundBody->CreateFixture(&groundBoxDef);

}

void Lvl1_1Layer::createBackground(){
	//Creates the sprites for the background and forground
	winSize = CCDirector::sharedDirector()->getWinSize();

	CCSprite* bgSprite = CCSprite::spriteWithFile("Level1_Room1.jpg");

	bgSprite->setPosition( ccp(bgSprite->getContentSize().width/2 *scale + 20, bgSprite->getContentSize().height/2 *scale));

	bgSprite->setScaleX(scale);
	bgSprite->setScaleY(scale);

	this->addChild(bgSprite);

	CCSprite *lightOverLay = CCSprite::spriteWithFile("Room1LIGHTINGLAYER.png");
	lightOverLay->setScale(scale * 2);

	lightOverLay->setPosition( ccp(bgSprite->getContentSize().width/2 *scale + 20, bgSprite->getContentSize().height/2 *scale));

	this->addChild(lightOverLay, 100);


	*outputText = 0;
	outputLog->setPosition(ccp(100,winSize.height/2));
	outputLog->initWithString(outputText, "Arial", 30);
	
	this->addChild(outputLog);
}

void Lvl1_1Layer::createHero(){
	CCPoint location = ccp(450*scale, 412*scale);
	hero = new Hero(world, this, PTM_RATIO, &location, 3, 3*scale);
}

void Lvl1_1Layer::createEnemies(){
	//(int i = 0; i < 2; i++){
		enemies->addObject(new Guard(world, this, PTM_RATIO, &ccp(800, 112), scale));
	//}
	//enemies->addObject(new Spherodroid(world, this, PTM_RATIO, &ccp(500, 900)));
}

void Lvl1_1Layer::createPowerups(){
	powerups->addObject(new Powerup(this, world, PTM_RATIO, 680, 1620, false, scale));
}
void Lvl1_1Layer::createBoxes(){

	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 0, 4250, 105, false, scale));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 0, 50, 1050, false, scale));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 1000, 615, 2350-1000, false, scale));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 300, 1000, 900 - 300, 1615-1000, false, scale));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 700, 1300, 1515 - 700, 1615-1300, false, scale));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 4200, 0, 50, 1050, false, scale));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 1780, 1300, 4250 - 1780, 2350 - 1300, false, scale));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 300, 2350, 2000 - 300, 100, false, scale));

	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 870, 670, 60, 320, false, scale));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 870, 115, 50, 390, false, scale));

	enemyColBox->addObject(new EnemyColBox(this, world, PTM_RATIO, 1214, 114, 10, 1000, false, scale));
	enemyColBox->addObject(new EnemyColBox(this, world, PTM_RATIO, 3900, 114, 10, 1000, false, scale));
	
}

void Lvl1_1Layer::createPortals(){
	phaseBox->addObject(new PhaseBox(this, world, PTM_RATIO, 900, 505, 60, 150, false, scale));
	//portalBox->addObject(new Portal(this, world, PTM_RATIO, 870, 505, 60, 160, 870 - 60 - 100, 505 + 80, 870+60+100, 505+80, false, true, scale));
	portalBox->addObject(new Portal(this, world, PTM_RATIO, 1560, 1285, 250, 30, 1550 + 130, 1290-300, 1550 + 130, 1290+350+300, false, true, scale, false));
	portalBox->addObject(new Portal(this, world, PTM_RATIO, 1560, 1590, 250, 30, 1550 + 130, 1290-300, 1550 + 130 - 90, 1290+350+300, false, true, scale, false));
}

void Lvl1_1Layer::goToNextRoom(){
	enemies->getObjectAtIndex(0)->setTagID(10);
	Lvl1_2Scene* Lvl1_2 = Lvl1_2Scene::node();
	CCDirector::sharedDirector()->replaceScene(Lvl1_2);
}

Lvl1_1Layer::~Lvl1_1Layer(void)
{
}
