#include "pch.h"
#include "Fearodroid.h"


Fearodroid::Fearodroid(b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc, float scale):
	Enemy("spriteSheet_Fearodroid.png", initialWorld, creationRoom, PTM_RATIO, 300, 300, loc, 15, scale), bulletCoolDown(240)
{
	totalMovements = 3;
	state = 1;
	leftOrRight = true;
	init("spriteSheet_Fearodroid.png", initialWorld, creationRoom, PTM_RATIO, loc);

}
void Fearodroid::init(const char *spritePath, b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc){
	
	setSprite(spritePath, 300, 300, loc);
	creationRoom->addChild(charSpriteSheet,2);
	creationRoom->addChild(gun,3);
	
	CreateBox2DObject(initialWorld, PTM_RATIO, loc);

	charSpriteSheet->setTag(tagID);
	gun->setTag(tagID);
	tagID ++ ;

}

void Fearodroid::setSprite(const char* spritePath, int width, int height, CCPoint* loc){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	charSpriteSheet = CCSpriteBatchNode::batchNodeWithFile(spritePath);
	charSprite = CCSprite::spriteWithBatchNode(charSpriteSheet, CCRectMake(0, 0, width, height));
	charSpriteSheet->addChild(charSprite, 2);
	
	CCAnimation *idleAnimation = CCAnimation::animation();
	CCAnimation *dieAnimation =  CCAnimation::animation();
	idleAnimation->setDelay(2.0/24.0f);
	dieAnimation->setDelay(0.125f);
	
	//Adds the eight frames of the idle animation to the animation object from the spritesheet
	for (int i=0; i<4; i++) {
		for(int j=0; j<6; j++){
			CCSpriteFrame *frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(j*300, i*300, 300, 300));
			idleAnimation->addFrame(frame);
		}
	}

	for(int i=0; i<4; i++){
		CCSpriteFrame *frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(i*300, 4*300, 300, 300));
		dieAnimation->addFrame(frame);
	}

	//Applies the idle animation to the Fearodroid
	CCAnimate *idleAnimate = CCAnimate::actionWithAnimation(idleAnimation, true);
	CCAnimate *dieAnimate = CCAnimate::actionWithAnimation(dieAnimation, true);

	CCRepeatForever* repeatIdleAction = CCRepeatForever::actionWithAction(idleAnimate);
	//And the move left animation
	
	CCRepeatForever* repeatMoveLeftAction = CCRepeatForever::actionWithAction(idleAnimate);
	//Add the move right animation
	
	CCRepeatForever* repeatMoveRightAction = CCRepeatForever::actionWithAction(idleAnimate);

	CCFiniteTimeAction* die = CCCallFunc::actionWithTarget(this, callfunc_selector(Fearodroid::dead));
	CCFiniteTimeAction* dieAction = CCSequence::actions(dieAnimate, die, NULL);


	//Places the Fearodroid on the screen and makes him hover
	charActions = new CCMutableArray<CCAction*>;
	addAction(repeatMoveRightAction);
	addAction(repeatMoveLeftAction);
	addAction(repeatIdleAction);
	addAction(dieAction);
	changeAction(0);
	charSpriteSheet->setPosition(*loc);
	charSpriteSheet->setScale(scale);

	gun = CCSprite::spriteWithFile("FearodroidGun.png");
	gun->setScale(2*scale);
	gun->setPosition(ccp(loc->x, loc->y - 110 * scale));
	
}

void Fearodroid::setBody(b2World *initialWorld, float PTM_RATIO, CCPoint *loc){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	b2BodyDef charBodyDef;
	charBodyDef.type = b2_dynamicBody;
	startPoint = b2Vec2(loc->x/PTM_RATIO, loc->y/PTM_RATIO);
	charBodyDef.position = startPoint;
	charBodyDef.userData = charSpriteSheet;
	charBody = initialWorld->CreateBody(&charBodyDef);
	charBody->SetGravityScale(0);

	b2BodyDef gunBodyDef;
	gunBodyDef.type = b2_dynamicBody;
	startPoint = b2Vec2(loc->x/PTM_RATIO, (loc->y - (110 + 32) *scale)/PTM_RATIO);
	gunBodyDef.position = startPoint;
	gunBodyDef.userData = gun;
	gunBody = initialWorld->CreateBody(&gunBodyDef);
	gunBody->SetGravityScale(0);

	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = charBody;
	revoluteJointDef.bodyB = gunBody;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.localAnchorA.Set(0, (0 - 110 *scale)/PTM_RATIO);
	revoluteJointDef.localAnchorB.Set((-64*scale)/PTM_RATIO, 0);
	revoluteJointDef.referenceAngle = 0;

	revoluteJointDef.enableLimit = true;
	revoluteJointDef.enableMotor = true;
	revoluteJointDef.maxMotorTorque = 3;
	revoluteJointDef.upperAngle = CC_DEGREES_TO_RADIANS(-90);
	revoluteJointDef.lowerAngle = CC_DEGREES_TO_RADIANS(-90);

	gunJoint = (b2RevoluteJoint*) initialWorld->CreateJoint(&revoluteJointDef);

}

void Fearodroid::setFixture(float PTM_RATIO){
	float width = (scale*charSprite->getContentSize().width);
	float height = (scale*charSprite->getContentSize().height);

	collisionShape.SetAsBox(width/PTM_RATIO/2, height/PTM_RATIO/2);

	//Throws the box just created into a collision fixture so it can properly collide with things
	b2FixtureDef charShapeDef;
	charShapeDef.shape = &collisionShape;
	charShapeDef.density = 9.0f;
	charShapeDef.friction = 5.0f;
	charShapeDef.restitution = 0.0f;
	charShapeDef.filter.categoryBits = ENEMY;
	charShapeDef.filter.maskBits = HERO | BOUNDARY | SENSOR;
	charFixture = charBody->CreateFixture(&charShapeDef);

	//Creates two frictionless planes along the sides of the character to make sure it doesn't grab any other characters when rubbing against them.
	b2PolygonShape sides;
	sides.SetAsBox((width/PTM_RATIO/2)*1, (height - 5)/PTM_RATIO/2);
	b2FixtureDef sidesShapeDef;
	sidesShapeDef.shape = &sides;
	sidesShapeDef.friction = 0.0f;
	charShapeDef.filter.categoryBits = ENEMY;
	side = charBody->CreateFixture(&sidesShapeDef);

	//Don't want the enemy falling over when he's hit, now, do we
	charBody->SetFixedRotation(true);

	b2PolygonShape gunShape;
	gunShape.SetAsBox((64 *2 * scale)/PTM_RATIO/2, (35 *2 * scale)/PTM_RATIO/2);
	b2FixtureDef gunShapeDef;
	gunShapeDef.shape = &gunShape;
	gunShapeDef.isSensor = true;
	gunShapeDef.friction = 0.0f;
	gunShapeDef.density = 1.0f;
	gunShapeDef.restitution = 0.0f;
	gunShapeDef.filter.categoryBits = SENSOR;
	gunShapeDef.filter.maskBits = HERO | BOUNDARY | SENSOR;

	gunFixture = gunBody->CreateFixture(&gunShapeDef);
}


void Fearodroid::moveEnemy(){
	if(movementCoolDown >0){
		movementCoolDown--;
	}
	switch(state){
		case 0: moveRight(); leftOrRight = true; break;
		case 1: moveLeft(); leftOrRight = false; break;
		case 2: stop();break;
		default: break;
	}
}

void Fearodroid::moveLeft(){
	b2Vec2 velocity = charBody->GetLinearVelocity();
	float velocityChange = -0.664 - velocity.x;
	float impulse = charBody->GetMass() * velocityChange;
	charBody->ApplyLinearImpulse( b2Vec2(impulse,0), charBody->GetWorldCenter() );
}

void Fearodroid::moveRight(){
	b2Vec2 velocity = charBody->GetLinearVelocity();
	float velocityChange = 0.664 - velocity.x;
	float impulse = charBody->GetMass() * velocityChange;
	charBody->ApplyLinearImpulse( b2Vec2(impulse,0), charBody->GetWorldCenter() );
}
void Fearodroid::stop(){
	b2Vec2 velocity = charBody->GetLinearVelocity();
	float velocityChange = 0 - velocity.x;
	float impulse = charBody->GetMass() * velocityChange;
	charBody->ApplyLinearImpulse( b2Vec2(impulse,0), charBody->GetWorldCenter() );
}

void Fearodroid::die(){
	living = false;
	//charSprite->setIsVisible(false);
	gun->setIsVisible(false);
	state = 3;
	changeAction(3);
	charFixture->SetSensor(true);
	side->SetSensor(true);
	gunFixture->SetSensor(true);
	b2Filter guardFilter = charFixture->GetFilterData();
	guardFilter.maskBits = NOTHING;
	side->SetFilterData(guardFilter);
	gunFixture->SetFilterData(guardFilter);
	charFixture->SetFilterData(guardFilter);
}

void Fearodroid::dead(){
	charSprite->setIsVisible(false);
}

void Fearodroid::respawn(){
	/*if(living){
		startPoint = b2Vec2(charBody->GetPosition());
	}*/
	living = true;
	charSprite->setIsVisible(true);
	gun->setIsVisible(true);
	charFixture->SetSensor(false);
	gunFixture->SetSensor(false);
	side->SetSensor(false);
	charBody->SetTransform(startPoint, 0);
	b2Filter guardFilter = charFixture->GetFilterData();
	guardFilter.maskBits = HERO | BOUNDARY | SENSOR;
	side->SetFilterData(guardFilter);
	charFixture->SetFilterData(guardFilter);
	gunFixture->SetFilterData(guardFilter);
}

void Fearodroid::setAngle(float angle){
	gunJoint->SetLimits(angle, angle);
}

void Fearodroid::attackHero(b2Vec2 heroLoc){
	float angle = b2Atan2(heroLoc.y - charBody->GetPosition().y, heroLoc.x - charBody->GetPosition().x);
	setAngle(angle);
}

Fearodroid::~Fearodroid(void)
{
}
