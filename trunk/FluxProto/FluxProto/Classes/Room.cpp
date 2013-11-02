#include "pch.h"
#include "Room.h"
#include "SimpleAudioEngine.h"
#include "MainMenu.h"

#define PTM_RATIO 256
#define TELECOOLDOWN 6

RoomLayer::RoomLayer(void)
	:world(NULL), 
	winSize(CCDirector::sharedDirector()->getWinSize()), 
	isTeleporting(false), direction(true), scale(1), direction2(0), direction2Director(-1), preTeleport(0), jointCount(0),
	groundBody(NULL), roomFixture(NULL), bottomFixture(NULL),
	hero(NULL), enemies(new CCMutableArray<Enemy*>), objects(new CCMutableArray<CollisionObject*>), 
	powerups(new CCMutableArray<Powerup*>), colBoxes(new CCMutableArray<CollisionBox*>), enemyColBox(new CCMutableArray<EnemyColBox*>),
	portalBox(new CCMutableArray<Portal*>), phaseBox(new CCMutableArray<PhaseBox*>), trapBox(new CCMutableArray<TrapBox*>),
	bullets(new CCMutableArray<Bullet*>),
	cameraView(NULL), contactListener(new MyContactListener()),
	outputText(new char(' ')),
	outputLog(new CCLabelTTF)
{
	roomWidth = winSize.width * 2;
	roomHeight = winSize.height;
	startPosition = b2Vec2(0, winSize.height/2/PTM_RATIO);
}


RoomLayer::~RoomLayer(void)
{
	if(world){
		delete world;
		world = NULL;
	}
	delete contactListener;
	contactListener = NULL;
}

RoomScene::~RoomScene(){
	if(_layer){
		_layer->release();
		_layer = NULL;
	}
}

bool RoomScene::init()
{
	if(CCScene::init()){
		this->_layer = RoomLayer::node();
		this->_layer->retain();
		this->addChild(_layer);
		//this->runAction(CCFollow::actionWithTarget(_layer->getCameraView()));
		return true;
	}
	return false;
}

bool RoomLayer::init(){
	if ( !CCLayer::init() )
	{
		return false;
	}

	srand(time(NULL));

	createWorld();
	createGround();
	createBackground();
	createBoxes();
	createHero();
	createCamera();
	//this->runAction(CCFollow::actionWithTarget(hero->getBatchNode()));
	this->runAction(CCFollow::actionWithTarget(cameraView->getView()));
	createEnemies();
	createPowerups();
	createBGMusic();
	createPortals();

	this->schedule(schedule_selector(RoomLayer::tick));
	this->schedule(schedule_selector(RoomLayer::update));

	this->setIsTouchEnabled(true);

	return true;

}

void RoomLayer::createBGMusic(){
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds//FLuXOST_Level01.mp3", true);
}

void RoomLayer::createCamera(){
	cameraView = new Camera(roomWidth, roomHeight, this);
}

void RoomLayer::createWorld(){
	//Create gravity acceleration vector
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.8f);
	world = new b2World(gravity);
	world->SetContactListener(contactListener);
	world->SetDestructionListener(destructionListener);
}

void RoomLayer::createGround(){
	//Creates the body for the ground (object that can have the ground collision put in it)
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0,0);
	groundBody = world->CreateBody(&groundBodyDef);

	//Creates the edge shape object that will be shaped to fit the edge of the screen. The fixture is the part that will have collision attached to it.
	b2EdgeShape groundBox;
	b2FixtureDef groundBoxDef;
	groundBoxDef.shape = &groundBox;

	//Draws the lines that surround the main screen for the ground body.
	groundBox.Set(b2Vec2(0, (winSize.height * 0.055)/PTM_RATIO), b2Vec2(winSize.width * 2/PTM_RATIO, (winSize.height * 0.055)/PTM_RATIO));
	//Fix the lines to the body fixture
	bottomFixture = groundBody->CreateFixture(&groundBoxDef);
	groundBox.Set(b2Vec2(0, (winSize.height * 0.055)/PTM_RATIO), b2Vec2(0, winSize.height/PTM_RATIO));
	groundBody->CreateFixture(&groundBoxDef);
	groundBox.Set(b2Vec2(0, winSize.height/PTM_RATIO), b2Vec2(winSize.width* 2/PTM_RATIO, winSize.height/PTM_RATIO));
	groundBody->CreateFixture(&groundBoxDef);
	groundBox.Set(b2Vec2(winSize.width* 2/PTM_RATIO, winSize.height/PTM_RATIO), b2Vec2(winSize.width* 2/PTM_RATIO, (winSize.height * 0.055)/PTM_RATIO));
	groundBody->CreateFixture(&groundBoxDef);

	b2PolygonShape roomBody;
	b2Vec2* roomBodyCenter = new b2Vec2(winSize.width/PTM_RATIO, (winSize.height/2 + winSize.height * (0.055))/PTM_RATIO);
	roomBody.SetAsBox(winSize.width/PTM_RATIO, (winSize.height * (1-0.055))/PTM_RATIO/2, *roomBodyCenter, 0.0f);
	groundBoxDef.shape = &roomBody;
	groundBoxDef.isSensor = true;
	groundBoxDef.filter.categoryBits = SENSOR;
	groundBoxDef.filter.maskBits = 0x0000;

	roomFixture = groundBody->CreateFixture(&groundBoxDef);

	b2PolygonShape doorBody;
	b2Vec2* doorBodyCenter = new b2Vec2((roomWidth-50)/PTM_RATIO, (70 + 200)/PTM_RATIO);
	doorBody.SetAsBox(25/PTM_RATIO, 200/PTM_RATIO, *doorBodyCenter, 0.0f);
	groundBoxDef.shape = &doorBody;
	groundBoxDef.isSensor = true;
	groundBoxDef.filter.categoryBits = BOUNDARY;
	groundBoxDef.filter.maskBits = HERO;

	doorFixture = groundBody->CreateFixture(&groundBoxDef);

}

void RoomLayer::createBackground(){
	//Creates the sprites for the background and forground
	CCSprite* bgSprite = sizeSprite("test_background.jpg");
	this->addChild(bgSprite);

	CCSprite* bgmSprite = sizeSprite("test_midBackground.png");
	this->addChild(bgmSprite);

	CCSprite* roomSprite = sizeSprite("test_level.png");
	this->addChild(roomSprite);

	CCSprite* bgSprite2 = sizeSprite("test_background.jpg");
	bgSprite2->setPosition(ccp(winSize.width *3/2, winSize.height/2));
	this->addChild(bgSprite2);

	CCSprite* bgmSprite2 = sizeSprite("test_midBackground.png");
	bgmSprite2->setPosition(ccp(winSize.width*3/2, winSize.height/2));
	this->addChild(bgmSprite2);

	CCSprite* roomSprite2 = sizeSprite("test_level.png");
	roomSprite2->setPosition(ccp(winSize.width*3/2, winSize.height/2));
	this->addChild(roomSprite2);

	*outputText = 0;
	outputLog->setPosition(ccp(100,winSize.height/2));
	outputLog->initWithString(outputText, "Arial", 30);
	
	this->addChild(outputLog);
}

CCSprite* RoomLayer::sizeSprite(char *myString){
	CCSprite* mySprite = CCSprite::spriteWithFile(myString);

	mySprite->setPosition( ccp(winSize.width/2, winSize.height/2) );

	mySprite->setScaleX(winSize.width/mySprite->getContentSize().width);
	mySprite->setScaleY(winSize.height/mySprite->getContentSize().height);

	return mySprite;
}

void RoomLayer::createBoxes(){
	bool debug = false;
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 405, 90, 295, 260, debug, 0.582));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 3298+405, 90, 295, 260, debug, 0.582));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 705, 70, 355, 490, debug, 0.582));
	colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, 3298+705, 70, 355, 490, debug, 0.582));
	//colBoxes->addObject(new CollisionBox(this, world, PTM_RATIO, roomWidth - 200, 70, 355, 490, true, 1));
}

void RoomLayer::createHero(){
	hero = new Hero(world, this, PTM_RATIO, &ccp(winSize.width/2, winSize.height - 200), 3);
}

void RoomLayer::createEnemies(){
	for(int i = 0; i < 2; i++){
		enemies->addObject(new Guard(world, this, PTM_RATIO, &ccp(300 + i*(400), 200)));
	}
	enemies->addObject(new Fearodroid(world, this, PTM_RATIO, &ccp(500, 900)));
	jointCount = 1;
}

void RoomLayer::createPowerups(){
	powerups->addObject(new Powerup(this, world, PTM_RATIO, 1500, 200));
}

void RoomLayer::createPortals(){
	portalBox->addObject(new Portal(this,world,PTM_RATIO,  3298, 770, 355, 200, 3298-150, 770+20, 3298+355+150, 770+20, false, false, 0.582));
}

void RoomLayer::tick(ccTime dt){
	world->Step(dt, 60, 10);

	hero->decrementJumpingTimeout();
	hero->decrementCoolDown();

	std::vector<MyContact>::iterator pos;
	for(pos = contactListener->_contacts.begin(); pos != contactListener->_contacts.end(); ++pos) {
		MyContact contact = *pos;
		
		if(contact.fixtureA == doorFixture || contact.fixtureB == doorFixture){
			if(hero->isTouching(contact.fixtureA) || hero->isTouching(contact.fixtureB)){
				goToNextRoom();
			}
		}else{

			EnemyColBox * eColBox;
			Enemy * enemy;
			for(int i = 0; i < enemyColBox->count(); i++){
				eColBox = enemyColBox->getObjectAtIndex(i);
				if(eColBox->isTouching(contact.fixtureA, contact.fixtureB)){
					for(int j = 0; j < enemies->count(); j++){
						enemy = enemies->getObjectAtIndex(j);
						if(enemy->isTouching(contact.fixtureA) || enemy->isTouching(contact.fixtureB)){
							swapDirection(enemy);
						}
					}
				}
			}

			static bool isStopped;
			static int countdown;
			if(!isTeleporting){
				countdown--;
				if(hero->isTouching(contact.fixtureA) || hero->isTouching(contact.fixtureB)){
					for(int j = 0; j < enemies->count(); j++){
						enemy = enemies->getObjectAtIndex(j);
						if(enemy->isTouching(contact.fixtureA) || enemy->isTouching(contact.fixtureB)){
							if(hero->getDirection() == 0 || hero->getDirection() == 2 || hero->getDirection() == 3){
								if(!isStopped && countdown <= 0){
									hero->setState(3);
									isStopped = true;
									countdown = 100;
									break;
								}
							}
						}
					}
					countdown--;
				}else{
					isStopped = false;
				}
			}
		
			if (hero->isFeetTouching(contact.fixtureA, contact.fixtureB) && hero->isJumping()) {
				hero->endJump();
				//swapDirection(ccTime());
			}

			if(isTeleporting){
				Portal *port;
				for(int i=0; i<portalBox->count(); i++){
					port = portalBox->getObjectAtIndex(i);
					port->resetPortal();
					if(port->teleCollision(contact.fixtureA, contact.fixtureB)){
						killEnemies(hero->getEndTeleLoc());
						port->startAndEnd(hero->getB2Location());
						hero->setStartTeleloc(port->getStart());
						hero->setEndTeleLoc(port->getEnd());
						hero->teleMove();
						//hero->finishTeleport();
						//startTeleporting();
						//endTeleporting();
					}
				}
			}

			Bullet* bullet;
			for(int i=0; i< bullets->count(); i++){
				bullet = bullets->getObjectAtIndex(i);
				if(bullet->isTouching(contact.fixtureA, contact.fixtureB)){
					if(hero->isTouching(contact.fixtureA) || hero->isTouching(contact.fixtureB) && hero->isAlive()){
						killHero();
					}
					//bullet->die();
				}
			}

			TrapBox *trap;
			for(int i=0; i < trapBox->count(); i++){
				trap = trapBox->getObjectAtIndex(i);
				if(trap->trapCollision(contact.fixtureA, contact.fixtureB)) {
					if(hero->isTouching(contact.fixtureA) || hero->isTouching(contact.fixtureB)){
						if(hero->isAlive()){
							killHero();
						}
					}
				}
			}

			Powerup *pow;
			for(int i = 0; i < powerups->count(); i++){
				pow = powerups->getObjectAtIndex(i);
				if (pow->isTouching(contact.fixtureA, contact.fixtureB)) {
					hero->superSaiyan();
					pow->usePowerup();
				}
			}
		}

		if(!isTeleporting){
			Enemy* e = NULL;
			b2Vec2 heroLocation = b2Vec2(hero->getBody()->GetPosition());
			for(int i = 0; i < enemies->count(); i++){
				e = enemies->getObjectAtIndex(i);
				if(e->getType() == 1 && e->isAlive()){
					if(e->madeHit(heroLocation) && !isTeleporting && hero->isAlive()){
						killHero();
						e->resetAttackTimeout();
						break;
					}else{
						e->resetHit();
					}
					if(((Guard*) e)->isInAttackRange(heroLocation) && e->getAttackTimeout() <= 0){
						e->playAttackSound();
						e->attackHero(heroLocation);
					}
				}
				if(e->getType() == 3){
					Fearodroid* f = (Fearodroid*) e;
					if(hero->isInRange(e->getB2Location())){
						f->attackHero(heroLocation);
						if(f->getCoolDown() == 0){
							f->resetCoolDown();
							bullets->addObject(new Bullet(this, world, PTM_RATIO, f->getB2gunLoc(), heroLocation, scale));
						}
					}else{
						f->setAngle(CC_DEGREES_TO_RADIANS(-90));
					}
				}
			}
		}
	}

	for(b2Body *b = world->GetBodyList(); b; b=b->GetNext()){
		if(b->GetUserData() != NULL){
			CCSprite *sprite = (CCSprite*)b->GetUserData();
			if(sprite->getTag() == 1){
				hero->moveHero();
				moveCamera();
			}
			//Guard movement code
			else if (sprite->getTag() >= 10 && !(sprite->getTag() >= 200)) {
				int index = sprite->getTag() - 10;
				enemies->getObjectAtIndex(index)->moveEnemy();
			}else if(sprite->getTag() >= 200){
				int index = sprite->getTag() - 200;
				bullets->getObjectAtIndex(index)->fireBullet();
			}

			sprite->setPosition(ccp(b->GetPosition().x * PTM_RATIO, b->GetPosition().y *PTM_RATIO));
			sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
	hero->getCircle()->setPosition(ccp(hero->getCocosLocation().x, hero->getCocosLocation().y));
	hero->getBall()->setPosition(ccp(hero->getCocosLocation().x, hero->getCocosLocation().y +  102 * scale));
	hero->setRotation(-1 * CC_RADIANS_TO_DEGREES(hero->getBody()->GetAngle()));
	//hero->finishTeleport();
}

void RoomLayer::update(cocos2d::ccTime dt){
	if(preTeleport > 0){
		preTeleport--;
	}
	endTeleporting();

	Bullet *b;
	for(int i = 0; i < bullets->count(); i++){
		b = bullets->getObjectAtIndex(i);
		if(!b->isAlive()){
			bullets->removeObjectAtIndex(i);
			world->DestroyBody(b->getBody());
		}
	}

	Fearodroid *f;
	for(int i=0; i < enemies->count(); i++){
		if(enemies->getObjectAtIndex(i)->getType() == 3){
			f = (Fearodroid*) enemies->getObjectAtIndex(i);
			f->decrementCoolDown();
		}
	}
}
 
void RoomLayer::swapDirection (Enemy *enemy) {
	if(enemy->getdirection() && enemy->canTurn()){
		enemy->setState(1);
		enemy->changeAction(1);
		enemy->resetMoveCoolDown();
	}else if(!(enemy->getdirection()) && enemy->canTurn()){
		enemy->setState(0);
		enemy->changeAction(0);
		enemy->resetMoveCoolDown();
	}
}

//Event handler that is a catchall for the click event (or the Windows Phone 8 touch event)
void RoomLayer::ccTouchesBegan(CCSet* touches, CCEvent* event){
	//Objects to handle the touch and find out which area of the screen the touch is in
	if(hero->isAlive()){
		CCTouch *touch = (CCTouch*)(touches->anyObject());
		CCPoint location = this->convertTouchToNodeSpace(touch);
		b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
		hero->setStartTeleloc(locationWorld);
		inputStart = location;

		if(hero->startTeleport(locationWorld, roomFixture) && preTeleport == 0){
			showPortals();
			return;
		}

		hero->isMoving(locationWorld);
	}
}
void RoomLayer::ccTouchesMoved(CCSet* touches, CCEvent* event){
	if(hero->isAlive()){
		if(hero->isTeleporting()){
			CCTouch *touch = (CCTouch*)(touches->anyObject());
			CCPoint location = this->convertTouchToNodeSpace(touch);
			b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);

			hero;
			if(!hero->isInRange(locationWorld) && roomFixture->TestPoint(locationWorld) && !isTeleporting){
				hidePortals();
				if(hero->endTeleport(locationWorld, roomFixture)){
					startTeleporting();
				}
			}
		}
	}

}
void RoomLayer::ccTouchesEnded(CCSet* touches, CCEvent* event){
	CCTouch *touch = (CCTouch*)(touches->anyObject());
	CCPoint location = this->convertTouchToNodeSpace(touch);
	b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
	
	if( !isTeleporting){
		hidePortals();
		if(hero->endTeleport(locationWorld, roomFixture)){
			startTeleporting();
		}
	}

	if(inputStart.y <= cameraView->getBottomPos()){
		if((location.y - hero->getStartTeleLoc().y) > winSize.height * 0.1){
			//swapDirection(ccTime());
		}
	}
}
void RoomLayer::ccTouchesCancelled(CCSet* touches, CCEvent* event){
	CCTouch *touch = (CCTouch*)(touches->anyObject());
	CCPoint location = this->convertTouchToNodeSpace(touch);
	b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
	
	if( !isTeleporting){
		hidePortals();
		if(hero->endTeleport(locationWorld, roomFixture)){
			startTeleporting();
		}
	}
}

void RoomLayer::startTeleporting(){
	if(world->GetJointCount() == jointCount){
		char *soundEffect = (char*)malloc(30);
		b2MouseJointDef md;
		md.bodyA = groundBody;
		md.bodyB = hero->getBody();
		md.target = hero->getB2Location();
		md.collideConnected = true;
		md.maxForce = 1000.0f * hero->getBody()->GetMass();
		//md.dampingRatio = 0;
		md.frequencyHz = 60;

		sprintf(soundEffect,"Sounds\\Flux_Jump_%d.mp3", rand()%3+1);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(soundEffect, false);

		mouseJoint = (b2MouseJoint *) world->CreateJoint(&md);
		hero->getBody()->SetAwake(true);
		isTeleporting = true;
		preTeleport = TELECOOLDOWN;
		free(soundEffect);
	}
}

void RoomLayer::endTeleporting(){
	if(preTeleport == 0){
		if(mouseJoint || isTeleporting){
			bool stopped = hero->hasStopped();
			if(hero->getDirection() != 3){
				stopped = hero->hasStopped();
			}
		
			if(stopped){
				finishTeleport();
			
			}else{
				mouseJoint->SetTarget(hero->getEndTeleLoc());
			}
		}else{
			if(world->GetJointCount() != 0){
				//mouseJoint = (b2MouseJoint*)world->GetJointList();
				if(mouseJoint != NULL){
					world->DestroyJoint(mouseJoint);
				}
				mouseJoint = NULL;
				hero->finishTeleport();
				isTeleporting = false;
			}
		}
	}
}

void RoomLayer::finishTeleport(){
	hero->setEndTeleLoc(hero->getB2Location());
	killEnemies(hero->getEndTeleLoc());
	if(mouseJoint != NULL){
		world->DestroyJoint(mouseJoint);
	}
	mouseJoint = NULL;
	hero->finishTeleport();
	isTeleporting = false;
}

void RoomLayer::killEnemies(b2Vec2 inputP2){

	b2RayCastInput input;
	input.p1 = hero->getStartTeleLoc();
	input.p2 = inputP2;
	input.maxFraction = 1.0f;

	Enemy* e = NULL;
	b2Fixture* f = NULL;
	float closestFraction = 1;
    b2Vec2 intersectionNormal(0,0);

	for(int i = 0; i < enemies->count(); i++){
		b2RayCastOutput outputRay;
		e = enemies->getObjectAtIndex(i);
		f = e->getFixture();
		if(!f->RayCast(&outputRay, input, 0))
			continue;
		if(outputRay.fraction < 1){
			e->playDeathSound();
			e->die();
			//output("enem ded");
		}
	}
}

void RoomLayer::killHero(){
	hero->setLiving(false);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Sounds\\deathSound.mp3", false);
	CCDelayTime *delayAction = CCDelayTime::actionWithDuration(2);
	CCFiniteTimeAction* restart = CCCallFunc::actionWithTarget(this, callfunc_selector(RoomLayer::restart));
	CCFiniteTimeAction* dieAnimation = CCCallFunc::actionWithTarget(hero, callfunc_selector(Hero::die));
	CCFiniteTimeAction* dieingAction = CCSequence::actions(dieAnimation, delayAction, restart, NULL);
	runAction(dieingAction);
}

void RoomLayer::restart(){
	hero->reset();
	cameraView->setPosition( ccp(winSize.width/2, winSize.height/2) );
	for(int i = 0; i < enemies->count(); i++){
		enemies->getObjectAtIndex(i)->respawn();
	}
	isTeleporting = false;
	//output("hero ded");
	hero->moveHero();
}

void RoomLayer::goToNextRoom(){
	enemies->getObjectAtIndex(0)->setTagID(10);
	RoomScene* DemoRoomScene = RoomScene::node();
	CCDirector::sharedDirector()->replaceScene(DemoRoomScene);
}

void RoomLayer::output(char *text){
	strcat(outputText, "\n");
	strcat(outputText, text);
	outputLog->setString(outputText);
}

void RoomLayer::showPortals(){
	
	Portal*  portal;

	for(int i = 0; i < portalBox->count(); i++){
		portal = portalBox->getObjectAtIndex(i);
		if(hero->isInRange(portal->getB2Location())){
			portal->showTexture();
		}
	}

	PhaseBox*  phase;

	for(int i = 0; i < phaseBox->count(); i++){
		phase = phaseBox->getObjectAtIndex(i);
		if(hero->isInRange(phase->getB2Location())){
			phase->showTexture();
		}
	}
}

void RoomLayer::hidePortals(){
	for(int i = 0; i < portalBox->count(); i++){
		portalBox->getObjectAtIndex(i)->hideTexture();
	}

	for(int i = 0; i < phaseBox->count(); i++){
		phaseBox->getObjectAtIndex(i)->hideTexture();
	}
}

bool MiniMenuLayer::init(){
	background = CCSprite::spriteWithFile("MiniMenuBG.png");
	BTNmainmenu = CCMenuItemImage::itemFromNormalImage("MainMenu.png","Selected.png", this,  menu_selector(MiniMenuLayer::goMainMenu));
	BTNsettings = CCMenuItemImage::itemFromNormalImage("Settings.png","Selected.png", this,  menu_selector(MiniMenuLayer::goSettings));
	BTNabout = CCMenuItemImage::itemFromNormalImage("About.png","Selected.png", this,  menu_selector(MiniMenuLayer::goAbout));
	miniMenu = CCMenu::menuWithItem(BTNmainmenu);
	miniMenu->addChild(BTNsettings, 1);
	miniMenu->addChild(BTNabout, 2);
	miniMenu->alignItemsHorizontally();
	miniMenu->setScale(0.34);
	miniMenu->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height-210));
	background->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height-210));
	this->addChild(background);
	this->addChild(miniMenu);
	return true;
}

void MiniMenuLayer::goMainMenu(cocos2d::CCObject*sender){
	MainMenuScene* MainMenuScene = MainMenuScene::node();
	CCDirector::sharedDirector()->replaceScene(MainMenuScene);
}
void MiniMenuLayer::goSettings(cocos2d::CCObject*sender){

}
void MiniMenuLayer::goAbout(cocos2d::CCObject*sender){

}