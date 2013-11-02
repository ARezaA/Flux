#include "pch.h"
#include "Lvl1_2.h"
#include "SimpleAudioEngine.h"

#define PTM_RATIO 256
#define ROOMSCALE 0.8

bool Lvl1_2Scene::init()
{
	if(CCScene::init()){
		this->_layer = Lvl1_2Layer::node();
		this->_layer->retain();
		this->addChild(_layer);
		return true;
	}
	return false;
}

Lvl1_2Layer::Lvl1_2Layer(void):RoomLayer()
{
	scale = 0.4;
	roomHeight = 3611 * ROOMSCALE;//winSize.height * 1.05;
	roomWidth =  6175 * ROOMSCALE;//winSize.width*1.5;
}

void Lvl1_2Layer::createCamera(){
	cameraView = new Camera(roomWidth, roomHeight, this);
}

void Lvl1_2Layer::createWorld(){
	//Create gravity acceleration vector
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.8f);
	world = new b2World(gravity);
	world->SetContactListener(contactListener);
	world->SetDestructionListener(destructionListener);
}

void Lvl1_2Layer::createGround(){
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0,0);
	groundBody = world->CreateBody(&groundBodyDef);

	b2FixtureDef groundBoxDef;

	b2PolygonShape doorBody;
	doorBody.SetAsBox((60/PTM_RATIO/2), (380/PTM_RATIO/2), b2Vec2(((5410.0 -30)*ROOMSCALE)/PTM_RATIO, ((3190.0 +190)*ROOMSCALE)/PTM_RATIO), 0);
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

void Lvl1_2Layer::createBackground(){
	//Creates the sprites for the background and forground
	winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite* bgSprite = CCSprite::spriteWithFile("Level1_Room2.jpg");
	bgSprite->setPosition(ccp(roomWidth/2,roomHeight/2));
	bgSprite->setScale(roomWidth/bgSprite->getContentSize().width);
	this->addChild(bgSprite);

	CCSpriteBatchNode* spriteSheet = CCSpriteBatchNode::batchNodeWithFile("Level1_Room2_Fan2.png");
	CCSprite* fanSprite = CCSprite::spriteWithBatchNode(spriteSheet, CCRectMake(0, 0, 512, 118));
	spriteSheet->addChild(fanSprite);

	CCAnimation *fanAnimation = CCAnimation::animation();
	fanAnimation->setDelay(1.0/15.0);

	CCSpriteFrame *frame;
	for(int j=0; j<2; j++){
		frame = CCSpriteFrame::frameWithTexture(spriteSheet->getTexture(), CCRectMake(j*532, 0, 512, 118));
		fanAnimation->addFrame(frame);
	}

	CCAnimate *fanAnimate = CCAnimate::actionWithAnimation(fanAnimation, true);
	CCRepeatForever* repeatFanAction = CCRepeatForever::actionWithAction(fanAnimate);

	spriteSheet->setPosition(ccp(4430 * ROOMSCALE, 1250 *ROOMSCALE));
	spriteSheet->setScale(1.1518);
	spriteSheet->setTag(1);

	addChild(spriteSheet);

	fanSprite->runAction(repeatFanAction);

	*outputText = 0;
	outputLog->setPosition(ccp(100,winSize.height/2));
	outputLog->initWithString(outputText, "Arial", 30);
	
	this->addChild(outputLog);
}

void Lvl1_2Layer::createHero(){
	CCPoint location = ccp(900*scale, 360*scale);
	hero = new Hero(world, this, PTM_RATIO, &location, 3, 3*scale);
}

void Lvl1_2Layer::createEnemies(){
	for(int i = 0; i < 20; i++){
		enemies->addObject(new Guard(world, this, PTM_RATIO, &ccp(1500 + (160 * i), 112), scale));
	}
	//enemies->addObject(new Spherodroid(world, this, PTM_RATIO, &ccp(500, 900)));
}

void Lvl1_2Layer::createPowerups(){
	powerups->addObject(new Powerup(this, world, PTM_RATIO, (4400*ROOMSCALE)/scale, (800*ROOMSCALE)/scale, false, scale));
}
void Lvl1_2Layer::createBoxes(){
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 0, 6210, 55, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 0, 14, 550, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 6160, 0, 50, 550, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 500, 150, 700-500, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 650, 1060, 790-650, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 740, 750, 1210-740, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 1180, 2700, 3711-1180, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 2500, 2150, 4050-2500, 3711-2150, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 4000, 3580, 5460-4000, 100, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 5410, 3140, 6210-5410, 3611-3140, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 4500, 3110, 6210-4500, 3190-3110, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 4800, 650, 6210-4800, 3160-650, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 1510, 650, 4900-1510, 790-650, false, ROOMSCALE));


	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 3020, 1160, 1040, 535, false, ROOMSCALE));
	trapBox->addObject(new TrapBox(this, world, PTM_RATIO, 4060, 1160, 740, 172, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 4650, 2000, 160, 145, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 4060, 2600, 300, 145, false, ROOMSCALE));

	enemyColBox->addObject(new EnemyColBox(this,world,PTM_RATIO, 20, 55, 20, 500, false, ROOMSCALE));
	enemyColBox->addObject(new EnemyColBox(this,world,PTM_RATIO, 6130, 55, 20, 500, false, ROOMSCALE));
}

void Lvl1_2Layer::createPortals(){
	phaseBox->addObject(new PhaseBox(this, world, PTM_RATIO, 1065, 645, 440, 25, false, ROOMSCALE));
	phaseBox->addObject(new PhaseBox(this, world, PTM_RATIO, 3005, 800, 60, 350, false, ROOMSCALE));
	phaseBox->addObject(new PhaseBox(this, world, PTM_RATIO, 2710, 1670, 300, 25, false, ROOMSCALE));
}

void Lvl1_2Layer::goToNextRoom(){
	enemies->getObjectAtIndex(0)->setTagID(10);
	Lvl1_3Scene* Lvl1_3 = Lvl1_3Scene::node();
	CCDirector::sharedDirector()->replaceScene(Lvl1_3);
}

Lvl1_2Layer::~Lvl1_2Layer(void)
{
}
