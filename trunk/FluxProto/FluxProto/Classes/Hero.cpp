#include "pch.h"
#include "Hero.h"
 

Hero::Hero(b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc, int startState, float scale):
	Character("heroSpriteSheet.png", initialWorld, creationRoom, PTM_RATIO, loc, startState, scale), 
	teleport(false), teleporting(false), jumping(true), 
	jumpingTimeout(0), teleTimeout(0), teleDistance((2296*0.5821710127349909)/2 * scale), numFootContacts(0),
	originNumOfTele(5), numOfTeleports(5), originTeleCoolDown(60 * 4), teleportCoolDown(60 * 4), b2Radius(0),
	originalState(3), circle(NULL)
{
	init("heroSpriteSheet.png", initialWorld, creationRoom, PTM_RATIO, loc);
}


Hero::~Hero(void)
{
}

void Hero::init(const char *spritePath, b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc){
	setSprite(spritePath, loc);
	creationRoom->addChild(charSpriteSheet);
	creationRoom->addChild(circle);
	creationRoom->addChild(ballSpriteSheet);

	CreateBox2DObject(initialWorld, PTM_RATIO, loc);

	//charActions = new CCMutableArray<CCAction*>;
}

void Hero::setSprite(const char *spritePath, CCPoint* loc){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	/*charSprite = CCSprite::spriteWithFile(spritePath, CCRectMake(0,51, 306, 306));
	charSprite->setPosition(*loc);
	charSprite->setTag(1);*/

	charSpriteSheet = CCSpriteBatchNode::batchNodeWithFile(spritePath);
	charSprite = CCSprite::spriteWithBatchNode(charSpriteSheet, CCRectMake(0, 51, 306, 306));
	charSpriteSheet->addChild(charSprite);
	
	CCAnimation *idleAnimation = CCAnimation::animation();
	CCAnimation *teleIdleAnimation = CCAnimation::animation();
	CCAnimation	*jumpAnimation = CCAnimation::animation();
	CCAnimation *teleMoveAnimation = CCAnimation::animation();
	CCAnimation *delayTeleAnimation = CCAnimation::animation();
	CCAnimation *moveRightAnimation = CCAnimation::animation();
	CCAnimation *crouchAnimation = CCAnimation::animation();
	CCAnimation *preTeleAnimation = CCAnimation::animation();
	CCAnimation *dieAnimation = CCAnimation::animation();
	idleAnimation->setDelay(1.0);
	teleIdleAnimation->setDelay(1.0);
	teleMoveAnimation->setDelay(1.0/180.0);
	delayTeleAnimation->setDelay(1.0/180.0);
	moveRightAnimation->setDelay(1.0/15.0);
	jumpAnimation->setDelay(1.0/15.0);
	crouchAnimation->setDelay(1.0);
	preTeleAnimation->setDelay(1.0/30.0);
	dieAnimation->setDelay(1.0);

	CCSpriteFrame *frame;
	for(int j=0; j<1; j++){
		frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(j*306, 51, 306, 306));
		idleAnimation->addFrame(frame);
	}
	for(int j=1; j<2; j++){
		frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(j*306, 51, 306, 306));
		teleIdleAnimation->addFrame(frame);
	}
	for(int j=1; j<6; j++){
		frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(j*306, 51, 306, 306));
		teleMoveAnimation->addFrame(frame);
	}
	for(int j=0; j<20; j++){
		frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(5*306, 51, 306, 306));
		teleMoveAnimation->addFrame(frame);
	}
	for(int j=1; j<9; j++){
		frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(5*306, 51, 306, 306));
		delayTeleAnimation->addFrame(frame);
	}
	for(int j=0; j<20; j++){
		frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(5*306, 51, 306, 306));
		delayTeleAnimation->addFrame(frame);
	}
	//Adds the eight frames of the moving right animation to the animation object from the spritesheet
	for(int j=1; j<3; j++){
		for(int i =0; i<6; i++){
			frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(i*306, 51 + 306*j, 306, 306));
			moveRightAnimation->addFrame(frame);
		}
	}

	for(int j=0; j<1; j++){
		frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(j*306, 51 + 306 * 3, 306, 306));
		crouchAnimation->addFrame(frame);
	}

	for(int j=1; j<4; j = j++){
		frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(j*306, 51 + 306 * 3, 306, 306));
		preTeleAnimation->addFrame(frame);
	}

	for(int j=0; j<3; j++){
		frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(4*306, 51 + 306 * 3, 306, 306));
		dieAnimation->addFrame(frame);
	}

	for(int j=0; j<2; j++){
		frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(j*306, 51 + 306 * 4, 306, 306));
		jumpAnimation->addFrame(frame);
	}

	//Applied both animaitons to the guard at once, which ends up just applying the moving left one.  It is repeated infinitely.
	CCAnimate *idleAnimate = CCAnimate::actionWithAnimation(idleAnimation, true);
	CCAnimate *teleIdleAnimate = CCAnimate::actionWithAnimation(teleIdleAnimation, true);
	CCAnimate *jumpAnimate = CCAnimate::actionWithAnimation(jumpAnimation, true);
	CCAnimate *teleMoveAnimate = CCAnimate::actionWithAnimation(teleMoveAnimation, true);
	CCAnimate *delayTeleAnimate = CCAnimate::actionWithAnimation(delayTeleAnimation, true);
	CCAnimate *moveRightAnimate = CCAnimate::actionWithAnimation(moveRightAnimation, true);
	CCAnimate *crouchAnimate = CCAnimate::actionWithAnimation(crouchAnimation, true);
	CCAnimate *preTeleAnimate = CCAnimate::actionWithAnimation(preTeleAnimation, true);
	CCAnimate *dieAnimate = CCAnimate::actionWithAnimation(dieAnimation, true);
	CCRepeatForever* repeatIdleAction = CCRepeatForever::actionWithAction(idleAnimate);
	CCRepeatForever* repeatTeleIdleAction = CCRepeatForever::actionWithAction(teleIdleAnimate);
	CCRepeatForever* repeatTeleMoveAction = CCRepeatForever::actionWithAction(teleMoveAnimate);
	CCRepeatForever* repeatJumpAction = CCRepeatForever::actionWithAction(jumpAnimate);
	CCRepeatForever* repeatRightAction = CCRepeatForever::actionWithAction(moveRightAnimate);
	CCRepeatForever* repeatCrouchAction = CCRepeatForever::actionWithAction(crouchAnimate);
	CCRepeatForever* repeatPreTeleAction = CCRepeatForever::actionWithAction(preTeleAnimate);
	CCFiniteTimeAction* rotateHeroDone = CCCallFunc::actionWithTarget(this, callfunc_selector(Hero::teleMove));
	CCFiniteTimeAction* teleportAction = CCSequence::actions(preTeleAnimate, rotateHeroDone, delayTeleAnimate, NULL);
	CCFiniteTimeAction* dieAction = CCSequence::actions(dieAnimate, NULL);

	//Places guard on screen and makes him moonwalk like a pro
	charActions = new CCMutableArray<CCAction*>;
	charActions->addObject(repeatRightAction);
	charActions->addObject(repeatJumpAction);
	charActions->addObject(repeatRightAction);
	charActions->addObject(repeatIdleAction);
	charActions->addObject(repeatCrouchAction);
	charActions->addObject(repeatTeleIdleAction);
	charActions->addObject(repeatTeleMoveAction);
	charActions->addObject(repeatPreTeleAction);
	charActions->addObject(teleportAction);
	charActions->addObject(dieAction);
	changeAction(state);
	charSpriteSheet->setPosition(*loc);
	charSpriteSheet->setScale(scale);
	charSpriteSheet->setTag(1);

	circle = CCSprite::spriteWithFile("circle.png");
	circle->setScale(0.5821710127349909 * scale);
	circle->setPosition(charSprite->getPosition());
	circle->setIsVisible(false);

	ballSpriteSheet = CCSpriteBatchNode::batchNodeWithFile("teleRefill.png");
	ball = CCSprite::spriteWithBatchNode(ballSpriteSheet, CCRectMake(0, 0, 128, 128));
	ballSpriteSheet->addChild(ball);

	CCAnimation *refillAnimation = CCAnimation::animation();
	refillAnimation->setDelay(1.0/25.0f);

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			frame = CCSpriteFrame::frameWithTexture(ballSpriteSheet->getTexture(), CCRectMake(j*128, i*128, 128, 128));
			
			if(i*j != 4){
				refillAnimation->addFrame(frame);
			}
		}
	}


	CCAnimate *refillAnimate = CCAnimate::actionWithAnimation(refillAnimation, true);

	CCFiniteTimeAction* refillDone = CCCallFunc::actionWithTarget(this, callfunc_selector(Hero::hideBall));
	CCFiniteTimeAction* refillAction = CCSequence::actions(refillAnimate, refillAnimate, refillAnimate, refillDone, NULL);
	charActions->addObject(refillAction);

	ball->setPosition(charSprite->getPosition());
	ball->setScale(scale/2);
	ball->setIsVisible(false);
}

void Hero::setBody(b2World *initialWorld, float PTM_RATIO, CCPoint* loc){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	b2BodyDef charBodyDef;
	charBodyDef.type = b2_dynamicBody;
	startPoint = b2Vec2(loc->x/PTM_RATIO, loc->y/PTM_RATIO);
	charBodyDef.position = startPoint;
	charBodyDef.userData = charSpriteSheet;
	charBody = initialWorld->CreateBody(&charBodyDef);
}

void Hero::setFixture(float PTM_RATIO){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	width = 102 * scale;
	height = 204 * scale;

	collisionShape.SetAsBox((width-20)/PTM_RATIO/2, height/PTM_RATIO/2);

	//Throws the box just created into a collision fixture so it can properly collide with things
	b2FixtureDef charShapeDef;
	charShapeDef.shape = &collisionShape;
	charShapeDef.density = 10.0f;
	charShapeDef.friction = 5.0f;
	charShapeDef.restitution = 0.0f;
	charShapeDef.filter.categoryBits = HERO;
	charShapeDef.filter.maskBits = ENEMY | BOUNDARY | SENSOR | POWERUP | BULLET;
	charFixture = charBody->CreateFixture(&charShapeDef);

	//Creates two frictionless planes along the sides of the character to make sure it doesn't grab any other characters when rubbing against them.
	b2PolygonShape sides;
	sides.SetAsBox(((width-10)/PTM_RATIO/2), (height - 5)/PTM_RATIO/2);
	b2FixtureDef sidesShapeDef;
	sidesShapeDef.shape = &sides;
	sidesShapeDef.friction = 0.0f;
	sidesShapeDef.filter.categoryBits = HERO;
	sidesShapeDef.filter.maskBits = ~ENEMY;
	side = charBody->CreateFixture(&sidesShapeDef);

	//The hero shouldn't be falling facefirst
	charBody->SetFixedRotation(true);

	//Create a new radius for the quarter circles that are used for click detection for the controls
	float radius = winSize.width/PTM_RATIO;
	b2PolygonShape polygonShape;

	//Creates an array of semicircle shapes that can be referred to to decide whether character is moving, stopping, or jumping after a click.
	b2Vec2 vertices[8];
	for(int i = 0; i < 4; i++){
		vertices[0].Set(0,0);
		for(int j = 0; j < 7; j++){
			float angle = CC_DEGREES_TO_RADIANS((j - 3 + (i*6))/6.0 * 90);
			vertices[j+1].Set(radius * cosf(angle), radius * sinf(angle));
		}
		polygonShape.Set(vertices, 8);
		charShapeDef.shape = &polygonShape;
		charShapeDef.isSensor = true;
		charShapeDef.filter.categoryBits = SENSOR;
		charShapeDef.filter.maskBits = BOUNDARY;
		//Fixes them to the hero
		sensor[i] = charBody->CreateFixture(&charShapeDef);
	}

	//Circle that defines the distance the hero can teleport
	b2CircleShape teleDistShape;
	teleDistShape.m_radius = teleDistance/PTM_RATIO;
	b2Radius = teleDistance/PTM_RATIO;
	
	charShapeDef.shape = &teleDistShape;
	charShapeDef.isSensor = true;
	charShapeDef.filter.categoryBits = SENSOR;
	charShapeDef.filter.maskBits = EVERYTHING;
	charShapeDef.density = 0.0f;
	teleportDistance = charBody->CreateFixture(&charShapeDef);

	b2PolygonShape footSensor;
	footSensor.SetAsBox(((width-20)/PTM_RATIO/2), (width/PTM_RATIO/2)*0.05,
						b2Vec2(0, (-1*height)/PTM_RATIO/2), 0);
	b2FixtureDef	feetShapeDef;
	feetShapeDef.shape = &footSensor;
	feetShapeDef.isSensor = true;
	feetShapeDef.filter.categoryBits = SENSOR;
	feetShapeDef.filter.maskBits = BOUNDARY;
	feetShapeDef.density = 0.0f;
	feet = charBody->CreateFixture(&feetShapeDef);

	b2PolygonShape teleShape;
	teleShape.SetAsBox((1/PTM_RATIO/2), (1/PTM_RATIO/2));

	b2FixtureDef teleShapeDef;
	teleShapeDef.shape = &teleShape;
	feetShapeDef.density = 1.0f;
	feetShapeDef.friction = 0.0f;
	feetShapeDef.restitution = 0.0f;
	teleShapeDef.filter.categoryBits = TELEPORT;
	teleShapeDef.filter.maskBits = BOUNDARY | POWERUP | PORTAL;
	teleportFixture = charBody->CreateFixture(&teleShapeDef);

}

bool Hero::startTeleport(b2Vec2 checkLoc, b2Fixture* roomFixture){
	if(charFixture->TestPoint(checkLoc) && roomFixture->TestPoint(checkLoc) && canTeleport()){
		teleport = true;
		circle->setIsVisible(true);
		if(state != 1 || state != 20) 
			originalState = state;
		state = 4;
		changeAction(state);
		preTeleport();
		return true;
	}
	return false;
}

bool Hero::endTeleport(b2Vec2 checkLoc, b2Fixture* roomFixture){
	circle->setIsVisible(false);
	if(teleport){
		if(roomFixture->TestPoint(checkLoc)){
			if(!(charFixture->TestPoint(checkLoc))){
				startTeleLoc = b2Vec2(charBody->GetPosition());
				endTeleLoc = b2Vec2(checkLoc);
				if(b2Radius < sqrt((checkLoc.x - charBody->GetPosition().x) * (checkLoc.x - charBody->GetPosition().x) + (checkLoc.y - charBody->GetPosition().y) * (checkLoc.y - charBody->GetPosition().y))){
					teleMax();
				}
				charBody->SetGravityScale(0);
				teleporting = true;
				numOfTeleports--;
				teleTimeout = 3;
				state = 8;
				moveHero();
				changeAction(state);
				beginTeleporting();
				return true;
			}else{
				originalState = (originalState > 3)? 3: originalState;
				state = originalState;
				changeAction(state);
			}
		}else{
			originalState = (originalState > 3)? 3: originalState;
			state = originalState;
			changeAction(state);
		}
	}
	teleport = false;
	return false;
}

void Hero::teleMax(){
	float angle = b2Atan2(endTeleLoc.y - charBody->GetPosition().y, endTeleLoc.x - charBody->GetPosition().x);

	float x = charBody->GetPosition().x + teleportDistance->GetShape()->m_radius * cos(angle);
	float y = charBody->GetPosition().y + teleportDistance->GetShape()->m_radius * sin(angle);

	endTeleLoc = b2Vec2(x,y);
}

void Hero::beginTeleporting(){
	charFixture->SetSensor(true);
	side->SetSensor(true);
}

void Hero::finishTeleport(){ 
	if(teleporting){
		charBody->SetTransform(charBody->GetPosition(), 0);
		charBody->SetGravityScale(1);
		fall();
		teleporting= false;
		teleport = false;
		originalState = (originalState > 3)? 3: originalState;
		state = originalState;
		changeAction(state);
		charSprite->setRotation(0);
		charSprite->setFlipX(leftOrRight);
		moveHero();
		charFixture->SetSensor(false);
		side->SetSensor(false);
	}
}

void Hero::isMoving(b2Vec2 checkLoc){
	if(living){
		for(int i = 0; i<4; i++){
			if(sensor[i]->TestPoint(checkLoc)){
				if(i == 1){
					if(!jumping){
						originalState = state;
						state = 1;
					}
				}else{
					originalState = state;
					state = i;
				}
			}
		}

		if(state != 20 && state != originalState){
			changeAction(state);
		}
	}
}

void Hero::moveHero(){

	if(!teleporting && living){
		switch(state){
			case 0: moveRight();break;
			case 1: if(!jumping)startJump(); break;
			case 2: moveLeft();break;
			case 3: if(!jumping)stop(); break;
			case 4: preTeleport();break;
			case 5: teleIdle();break;
			case 6: teleMove();break;
			case 7: preTeleport();break;
			case 8: preTeleport();break;
			default: break;
		}
	}
}

void Hero::moveRight(){
	b2Vec2 velocity = charBody->GetLinearVelocity();
	float velocityChange = 1.45 * 1.25 - velocity.x;
	float impulse = (charBody->GetMass() * velocityChange);
	leftOrRight = false;
	charSprite->setFlipX(leftOrRight);
	charBody->ApplyLinearImpulse( b2Vec2(impulse,0), charBody->GetWorldCenter() );
}
void Hero::startJump(){
	if(!jumping){
		char *soundEffect = (char*)malloc(30);
		jumping = true;
		sprintf(soundEffect,"Sounds\\jumpVoice%d.mp3", rand()%3+1);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(soundEffect, false);
		b2Vec2 velocity = charBody->GetLinearVelocity();
		float velocityYChange = 4.0 - velocity.y;
		float velocityXChange = velocity.x * 0.5;
		float impulseY = charBody->GetMass() * velocityYChange;
		float impulseX = charBody->GetMass() * velocityXChange;
		charSprite->setFlipX(leftOrRight);
		charBody->ApplyLinearImpulse( b2Vec2(impulseX, impulseY), charBody->GetWorldCenter() );
		state = 20;
		jumpingTimeout = 30;
		free(soundEffect);
	}
}

void Hero::endJump(){
	if(originalState < 4)
		state = originalState;
	jumping = false;
	changeAction(state);
}

void Hero::moveLeft(){
	b2Vec2 velocity = charBody->GetLinearVelocity();
	float velocityChange = -1.45 * 1.25 - velocity.x;
	float impulse = charBody->GetMass() * velocityChange;
	leftOrRight = true;
	charSprite->setFlipX(leftOrRight);
	charBody->ApplyLinearImpulse( b2Vec2(impulse,0), charBody->GetWorldCenter() );
}
void Hero::stop(){
	b2Vec2 velocity = charBody->GetLinearVelocity();
	float velocityChangeX = 0 - velocity.x;
	float velocityChangeY = 0 - velocity.y;
	float impulseX = charBody->GetMass() * velocityChangeX;
	float impulseY = charBody->GetMass() * velocityChangeY;
	charSprite->setFlipX(leftOrRight);
	if(teleporting){
		impulseY = impulseY + 100;
	}else{
		impulseY = 0;
	}

	charBody->ApplyLinearImpulse( b2Vec2(impulseX,impulseY), charBody->GetWorldCenter() );
}

void Hero::preTeleport(){
	b2Vec2 velocity = charBody->GetLinearVelocity();
	float velocityYChange = 0 - velocity.y;
	float velocityXChange = 0 - velocity.x;
	float impulseY;// = charBody->GetMass() * velocityYChange;
	float impulseX = charBody->GetMass() * velocityXChange;
	impulseY = 0;
	charSprite->setFlipX(leftOrRight);
	charBody->ApplyLinearImpulse( b2Vec2(impulseX, impulseY), charBody->GetWorldCenter() );
}

void Hero::teleIdle(){
}

void Hero::teleMove(){
	float angle = b2Atan2(endTeleLoc.y - charBody->GetPosition().y, endTeleLoc.x - charBody->GetPosition().x);
	charBody->SetTransform(charBody->GetPosition(), angle);
	charSprite->setFlipX(false);
}

void Hero::fall(){
	if(teleporting){
		b2Vec2 velocity = charBody->GetLinearVelocity();
		float velocityYChange = 2 - velocity.y;
		float velocityXChange = 0 - velocity.x;
		float impulseY = charBody->GetMass() * velocityYChange;
		float impulseX = charBody->GetMass() * velocityXChange;
		charSprite->setFlipX(leftOrRight);
		charBody->ApplyLinearImpulse( b2Vec2(impulseX, impulseY), charBody->GetWorldCenter() );
	}
}

bool Hero::hasStopped(){
	b2Vec2 velocity = charBody->GetLinearVelocity();

	teleTimeout--;

	return (abs(velocity.x) < 1 && abs(velocity.y) < 1) && teleTimeout <= 0; 
}

void Hero::decrementCoolDown(){
	if(!canTeleport()){
		if(teleportCoolDown > 0){
			teleportCoolDown--;
		}else{
			resetTeleCoolDown();
		}
	}
}

void Hero::decrementTeleport(){
	if(canTeleport()){
		numOfTeleports--;
	}
}

void Hero::reset(){
	charBody->SetTransform(startPoint, 0);
	charBody->ApplyLinearImpulse(b2Vec2(0, 100), charBody->GetWorldCenter());
	living = true;
	state = 3;
	originalState = 3;
	changeAction(state);
	resetTeleCoolDown();
	moveHero();
}

bool Hero::isTouching(b2Fixture *fixture){
	return fixture == charFixture || fixture == side || fixture == feet;
}

bool Hero::teleCollision(b2Fixture *fixtureA, b2Fixture *fixtureB){
	return teleportFixture == fixtureA || teleportFixture == fixtureB;
}

void Hero::die(){
	living = false;
	changeAction(9);
}

void Hero::showBall(){
	ball->setIsVisible(true);
	charSprite->setOpacity(255);
	ball->runAction(charActions->getObjectAtIndex(10));
}

void Hero::hideBall(){
	ball->stopAllActions();
	ball->setIsVisible(false);
}

void Hero::resetTeleCoolDown(){
	numOfTeleports = originNumOfTele; 
	teleportCoolDown = originTeleCoolDown;
	showBall();
}
   