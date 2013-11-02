#include "pch.h"
#include "Lvl1_4.h"
#include "SimpleAudioEngine.h"

#define PTM_RATIO 256
#define ROOMSCALE 0.8

bool Lvl1_4Scene::init()
{
	if(CCScene::init()){
		this->_layer = Lvl1_4Layer::node();
		this->_layer->retain();
		this->addChild(_layer);
		return true;
	}
	return false;
}

Lvl1_4Layer::Lvl1_4Layer(void):RoomLayer()
{
	scale = 0.4;
	roomHeight = 4800 *ROOMSCALE;
	roomWidth = 5700 *ROOMSCALE;
}

void Lvl1_4Layer::createCamera(){
	cameraView = new Camera(roomWidth, roomHeight, this);
}

void Lvl1_4Layer::createWorld(){
	//Create gravity acceleration vector
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.8f);
	world = new b2World(gravity);
	world->SetContactListener(contactListener);
	world->SetDestructionListener(destructionListener);
}

void Lvl1_4Layer::createGround(){
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
	doorBody.SetAsBox((40/PTM_RATIO/2), (450/PTM_RATIO/2), b2Vec2(((5370+ 40/2)*ROOMSCALE)/PTM_RATIO, ((3600 + 450/2)*ROOMSCALE)/PTM_RATIO), 0);
	groundBoxDef.shape = &doorBody;
	groundBoxDef.isSensor = true;
	groundBoxDef.filter.categoryBits = BOUNDARY;
	groundBoxDef.filter.maskBits = HERO;

	doorFixture = groundBody->CreateFixture(&groundBoxDef);

}

void Lvl1_4Layer::createBackground(){
	//Creates the sprites for the background and forground
	winSize = CCDirector::sharedDirector()->getWinSize();

	CCSprite* bgSprite = CCSprite::spriteWithFile("Level1_Room4.jpg");

	bgSprite->setPosition( ccp(roomWidth/2, roomHeight/2));
	bgSprite->setScale( 2 * ROOMSCALE);

	this->addChild(bgSprite);


	/**outputText = 0;
	outputLog->setPosition(ccp(100,winSize.height/2));
	outputLog->initWithString(outputText, "Arial", 30);
	
	this->addChild(outputLog);*/
}

void Lvl1_4Layer::createHero(){
	CCPoint location = ccp(1350 * ROOMSCALE, 5000 * ROOMSCALE);
	hero = new Hero(world, this, PTM_RATIO, &location, 3, 3*scale);
}

void Lvl1_4Layer::createEnemies(){
	enemies->addObject(new Guard(world, this, PTM_RATIO, &ccp(250*ROOMSCALE, 1910*ROOMSCALE), scale));
	
	enemies->addObject(new Spherodroid(world, this, PTM_RATIO, &ccp(3800*ROOMSCALE, 4050*ROOMSCALE), scale));
	enemies->addObject(new Spherodroid(world, this, PTM_RATIO, &ccp(3400*ROOMSCALE, 2625*ROOMSCALE), scale));
	enemies->addObject(new Spherodroid(world, this, PTM_RATIO, &ccp(4100*ROOMSCALE, 1300*ROOMSCALE), scale));
}

void Lvl1_4Layer::createPowerups(){
	powerups->addObject(new Powerup(this, world, PTM_RATIO, 5100/0.5, 590/0.5, false, scale));
}

void Lvl1_4Layer::createBoxes(){
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 0, 1940, 1795, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 1700, 150, 1000, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 0, 2560, 1210, 4800-2560, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 1490, 2560, 460, 4800-2560, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 1650, 4650, 3300, 150, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 4655, 4050, 5700-4655, 4800-4050, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 4655, 1190, 5700-4655, 2395, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 5410, 3500, 290, 600, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 4655, 900, 290, 400, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 4655, 0, 290, 420, false, ROOMSCALE));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 5560, 370, 140, 900, false, ROOMSCALE));

	trapBox->addObject(new TrapBox(this, world, PTM_RATIO, 1800, -400, 4600, 20, false, ROOMSCALE));

	enemyColBox->addObject(new EnemyColBox(this,world,PTM_RATIO, 200, 1800, 20, 375, false, ROOMSCALE));
	enemyColBox->addObject(new EnemyColBox(this,world,PTM_RATIO, 1775, 1800, 10, 900, false, ROOMSCALE));
	enemyColBox->addObject(new EnemyColBox(this,world,PTM_RATIO, 2100, 0, 10, 4600, false, ROOMSCALE));
	enemyColBox->addObject(new EnemyColBox(this,world,PTM_RATIO, 4500, 0, 10, 4600, false, ROOMSCALE));
	enemyColBox->addObject(new EnemyColBox(this,world,PTM_RATIO, 2625, 2100, 10, 800, false, ROOMSCALE));
}

void Lvl1_4Layer::createPortals(){
	//portalBox->addObject(new Portal(this, world, PTM_RATIO, 870, 505, 60, 160, 870 - 60 - 100, 505 + 80, 870+60+100, 505+80, false, true, scale));
	phaseBox->addObject(new PhaseBox(this, world, PTM_RATIO, 0, 1560, 2540, 240, false, ROOMSCALE, false));
	phaseBox->addObject(new PhaseBox(this, world, PTM_RATIO, 3040, 2080, 600, 115, false, ROOMSCALE, false));
	phaseBox->addObject(new PhaseBox(this, world, PTM_RATIO, 3080, 930, 600, 115, false, ROOMSCALE, false));
	phaseBox->addObject(new PhaseBox(this, world, PTM_RATIO, 1530, 2560, 1000, 140, false, ROOMSCALE, false));
	phaseBox->addObject(new PhaseBox(this, world, PTM_RATIO, 4060, 3375, 1635, 215, false, ROOMSCALE, false));
	phaseBox->addObject(new PhaseBox(this, world, PTM_RATIO, 4040, 290, 1660, 150, false, ROOMSCALE, false));
	phaseBox->addObject(new PhaseBox(this, world, PTM_RATIO, 2530, 2910, 1635, 115, false, ROOMSCALE, false));
	phaseBox->addObject(new PhaseBox(this, world, PTM_RATIO, 4050, 2240, 900, 160, false, ROOMSCALE, false));

	phaseBox->addObject(new PhaseBox(this, world, PTM_RATIO, 4650, 450, 60, 450, false, ROOMSCALE));
	phaseBox->addObject(new PhaseBox(this, world, PTM_RATIO, 1220, 2550, 275, 25, false, ROOMSCALE));
}

void Lvl1_4Layer::goToNextRoom(){
	enemies->getObjectAtIndex(0)->setTagID(10);
	Lvl1_WinScene* win = Lvl1_WinScene::node();
	CCDirector::sharedDirector()->replaceScene(win);
}

Lvl1_4Layer::~Lvl1_4Layer(void)
{
}
