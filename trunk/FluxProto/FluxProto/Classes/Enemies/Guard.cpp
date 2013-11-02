#include "pch.h"
#include "Guard.h"


Guard::Guard(b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc, float scale):
	Enemy("spriteSheet_guard.png", initialWorld, creationRoom, PTM_RATIO, 300, 600, loc, 60, scale)
{
	totalMovements = 2;
	state = 1;
	leftOrRight = true;
	attackTimeout = 0;
	hit = false;
	init("spriteSheet_guard.png", initialWorld, creationRoom, PTM_RATIO, loc);
}

void Guard::init(const char *spritePath, b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc){
	
	setSprite(spritePath, 300, 600, loc);
	creationRoom->addChild(charSpriteSheet,2);
	
	CreateBox2DObject(initialWorld, PTM_RATIO, loc);

	charSpriteSheet->setTag(tagID);
	tagID ++ ;

}

Guard::~Guard(void)
{
}

void Guard::setSprite(const char* spritePath, int width, int height, CCPoint* loc){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	
	charSpriteSheet = CCSpriteBatchNode::batchNodeWithFile(spritePath);
	charSprite = CCSprite::spriteWithBatchNode(charSpriteSheet, CCRectMake(0, 0, width * 0.34, height * 0.34));
	//charSpriteSheet->setTag(3);
	charSpriteSheet->addChild(charSprite, 2);
	
	CCAnimation *moveRightAnimation = CCAnimation::animation();
	CCAnimation *moveLeftAnimation = CCAnimation::animation();
	CCAnimation *attackAnimation = CCAnimation::animation();
	CCAnimation *dieAnimation = CCAnimation::animation();
	moveRightAnimation->setDelay(0.125f);
	moveLeftAnimation->setDelay(0.125f);
	attackAnimation->setDelay(0.125f);
	dieAnimation->setDelay(0.125f);

	//Adds the eight frames of the moving right animation to the animation object from the spritesheet
	for(int j=0; j<8; j++){
		CCSpriteFrame *frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(j*306, 51, 306, 306));
		moveRightAnimation->addFrame(frame);
	}
	//Adds the eight frames of the moving left animation to the animation object from the spritesheet
	for(int j=0; j<8; j++){
		CCSpriteFrame *frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(j*306, 51 + 306, 306, 306));
		moveLeftAnimation->addFrame(frame);
	}
	for(int j=0; j<5; j++){
		CCSpriteFrame *frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(j*306, 51 + 306*2, 306, 306));
		attackAnimation->addFrame(frame);
	}
	for(int j=0; j<4; j++){
		CCSpriteFrame *frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(j*306, 51 + 306*3, 306, 306));
		dieAnimation->addFrame(frame);
	}

	//Applied both animaitons to the guard at once, which ends up just applying the moving left one.  It is repeated infinitely.
	CCAnimate *moveRightAnimate = CCAnimate::actionWithAnimation(moveRightAnimation, true);
	CCAnimate *moveLeftAnimate = CCAnimate::actionWithAnimation(moveLeftAnimation, true);
	CCAnimate *attackAnimate = CCAnimate::actionWithAnimation(attackAnimation, true);
	CCAnimate *dieAnimate = CCAnimate::actionWithAnimation(dieAnimation, true);
	CCRepeatForever* repeatRightAction = CCRepeatForever::actionWithAction(moveRightAnimate);
	CCRepeatForever* repeatLeftAction = CCRepeatForever::actionWithAction(moveLeftAnimate);
	//CCFiniteTimeAction* startAttack = CCCallFunc::actionWithTarget(this, callfunc_selector(Guard::attack));
	CCFiniteTimeAction* finishAttack = CCCallFunc::actionWithTarget(this, callfunc_selector(Guard::attackEnd));
	CCFiniteTimeAction* attackAction = CCSequence::actions(attackAnimate, finishAttack, NULL);
	CCFiniteTimeAction* die = CCCallFunc::actionWithTarget(this, callfunc_selector(Guard::dead));
	CCFiniteTimeAction* dieAction = CCSequence::actions(dieAnimate, die, NULL);

	//Places guard on screen and makes him moonwalk like a pro
	charActions = new CCMutableArray<CCAction*>;
	addAction(repeatRightAction);
	addAction(repeatLeftAction);
	addAction(attackAction);
	addAction(dieAction);
	changeAction(state);
	charSpriteSheet->setPosition(*loc);
	//charSpriteSheet->setPosition(ccp(loc->x, loc->y - 102));
	charSpriteSheet->setScale(scale/0.34);
}

void Guard::setFixture(float PTM_RATIO){

	width = width * scale;
	height = height * scale;


	collisionShape.SetAsBox(width/PTM_RATIO/2, height/PTM_RATIO/2);

	//Throws the box just created into a collision fixture so it can properly collide with things
	b2FixtureDef charShapeDef;
	charShapeDef.shape = &collisionShape;
	charShapeDef.density = 10.0f;
	charShapeDef.friction = 5.0f;
	charShapeDef.restitution = 0.0f;
	charShapeDef.filter.categoryBits = ENEMY;
	charShapeDef.filter.maskBits = BOUNDARY | SENSOR | HERO;
	charFixture = charBody->CreateFixture(&charShapeDef);

	//Creates two frictionless planes along the sides of the character to make sure it doesn't grab any other characters when rubbing against them.
	b2PolygonShape sides;
	sides.SetAsBox((width/PTM_RATIO/2)*1, (height - 5)/PTM_RATIO/2);
	b2FixtureDef sidesShapeDef;
	sidesShapeDef.shape = &sides;
	sidesShapeDef.friction = 0.0f;
	sidesShapeDef.filter.categoryBits = ENEMY;
	sidesShapeDef.filter.maskBits = BOUNDARY | SENSOR;
	side = charBody->CreateFixture(&sidesShapeDef);

	b2PolygonShape range;
	range.SetAsBox((width/PTM_RATIO) * 2.5/2, (height/PTM_RATIO) * 2.5/2);
	b2FixtureDef attackShapeDef;
	attackShapeDef.shape = &range;
	attackShapeDef.isSensor = true;
	attackShapeDef.filter.categoryBits = SENSOR;
	attackShapeDef.filter.maskBits = HERO;
	attackRange = charBody->CreateFixture(&attackShapeDef);

	//Don't want the enemy falling over when he's hit, now, do we
	charBody->SetFixedRotation(true);
}

void Guard::moveLeft(){
	b2Vec2 velocity = charBody->GetLinearVelocity();
	float velocityChange = -0.664 - velocity.x;
	float impulse = charBody->GetMass() * velocityChange;
	charBody->ApplyLinearImpulse( b2Vec2(impulse,0), charBody->GetWorldCenter() );
}

void Guard::moveRight(){
	b2Vec2 velocity = charBody->GetLinearVelocity();
	float velocityChange = 0.664 - velocity.x;
	float impulse = charBody->GetMass() * velocityChange;
	charBody->ApplyLinearImpulse( b2Vec2(impulse,0), charBody->GetWorldCenter() );
}
void Guard::stop(){
	b2Vec2 velocity = charBody->GetLinearVelocity();
	float velocityChange = 0 - velocity.x;
	float impulse = charBody->GetMass() * velocityChange;
	charBody->ApplyLinearImpulse( b2Vec2(impulse,0), charBody->GetWorldCenter() );
}

void Guard::attackEnd(){
	hit = true;
	if(charSprite->isFlipX()){
		state = 1;
		charSprite->setFlipX(false);
	}else{
		state = 0;
	}
	attackTimeout = 0;
	changeAction(state);
}

void Guard::moveEnemy(){
	if(movementCoolDown >0){
		movementCoolDown--;
	}
	switch(state){
		case 0: moveRight(); leftOrRight = true; break;
		case 1: moveLeft(); leftOrRight = false; break;
		default: break;
	}
}

void Guard::respawn(){
	/*if(living){
		startPoint = b2Vec2(charBody->GetPosition());
	}*/
	living = true;
	charSprite->setIsVisible(true);
	charFixture->SetSensor(false);
	side->SetSensor(false);
	charBody->SetTransform(startPoint, 0);
	b2Filter guardFilter = charFixture->GetFilterData();
	guardFilter.maskBits = BOUNDARY | SENSOR;
	side->SetFilterData(guardFilter);
	guardFilter.maskBits = HERO | BOUNDARY | SENSOR;
	charFixture->SetFilterData(guardFilter);
	attackTimeout = 0;
	movementCoolDown = 0;
	if(leftOrRight){
		state = 0;
	}else{
		state = 1;
	}
}

void Guard::attackHero(b2Vec2 heroLocation){
	state = 2;
	resetAttackTimeout();
	if(charBody->GetPosition().x - heroLocation.x > 0){
		charSprite->setFlipX(true);
	}
	changeAction(2);

}

bool Guard::madeHit(b2Vec2 heroLocation){
	/*if(hit && isInAttackRange(heroLocation)){
		if(charSprite->isFlipX()){
			if((charBody->GetPosition().x - heroLocation.x) * 256 < 200){
				return true;
			}
		}else{
			if((heroLocation.x - charBody->GetPosition().x) * 256 < 200){
				return true;
			}
		}
	}
	return false;*/
	return hit && isInAttackRange(heroLocation);
}

void Guard::die(){
	living = false;
	//charSprite->setIsVisible(false);
	state = 3;
	changeAction(3);
	charSprite->setFlipX(leftOrRight);
}

void Guard::dead(){
	charSprite->setIsVisible(false);
	charFixture->SetSensor(true);
	side->SetSensor(true);
	b2Filter guardFilter = charFixture->GetFilterData();
	guardFilter.maskBits = NOTHING;
	side->SetFilterData(guardFilter);
	charFixture->SetFilterData(guardFilter);
}

void Guard::playDeathSound(){
	char *soundEffect = (char*)malloc(30);
	sprintf(soundEffect,"Sounds\\guardDie%d.mp3", rand()%4+1);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(soundEffect, false);
	free(soundEffect);
}



