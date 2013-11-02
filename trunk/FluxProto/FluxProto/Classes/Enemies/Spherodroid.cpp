#include "pch.h"
#include "Spherodroid.h"


Spherodroid::Spherodroid(b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc, float scale):
	Enemy("spriteSheet_spherodroid.png", initialWorld, creationRoom, PTM_RATIO, 300, 300, loc, 15, scale)
{
	totalMovements = 3;
	state = 1;
	leftOrRight = true;
	init("spriteSheet_spherodroid.png", initialWorld, creationRoom, PTM_RATIO, loc);

}
void Spherodroid::init(const char *spritePath, b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc){
	
	setSprite(spritePath, 300, 300, loc);
	creationRoom->addChild(charSpriteSheet,2);
	
	CreateBox2DObject(initialWorld, PTM_RATIO, loc);

	charSpriteSheet->setTag(tagID);
	tagID ++ ;

}

void Spherodroid::setSprite(const char* spritePath, int width, int height, CCPoint* loc){
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

	//Applies the idle animation to the spherodroid
	CCAnimate *idleAnimate = CCAnimate::actionWithAnimation(idleAnimation, true);
	CCAnimate *dieAnimate = CCAnimate::actionWithAnimation(dieAnimation, true);

	CCRepeatForever* repeatIdleAction = CCRepeatForever::actionWithAction(idleAnimate);
	//And the move left animation
	
	CCRepeatForever* repeatMoveLeftAction = CCRepeatForever::actionWithAction(idleAnimate);
	//Add the move right animation
	
	CCRepeatForever* repeatMoveRightAction = CCRepeatForever::actionWithAction(idleAnimate);

	CCFiniteTimeAction* die = CCCallFunc::actionWithTarget(this, callfunc_selector(Spherodroid::dead));
	CCFiniteTimeAction* dieAction = CCSequence::actions(dieAnimate, die, NULL);


	//Places the spherodroid on the screen and makes him hover
	charActions = new CCMutableArray<CCAction*>;
	addAction(repeatMoveRightAction);
	addAction(repeatMoveLeftAction);
	addAction(repeatIdleAction);
	addAction(dieAction);
	changeAction(0);
	charSpriteSheet->setPosition(*loc);
	charSpriteSheet->setScale(scale);
	
}

void Spherodroid::setBody(b2World *initialWorld, float PTM_RATIO, CCPoint *loc){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	b2BodyDef charBodyDef;
	charBodyDef.type = b2_dynamicBody;
	startPoint = b2Vec2(loc->x/PTM_RATIO, loc->y/PTM_RATIO);
	charBodyDef.position = startPoint;
	charBodyDef.userData = charSpriteSheet;
	charBody = initialWorld->CreateBody(&charBodyDef);
	charBody->SetGravityScale(0);
}

void Spherodroid::setFixture(float PTM_RATIO){
	float width = (scale*charSprite->getContentSize().width);
	float height = (scale*charSprite->getContentSize().height);

	collisionShape.SetAsBox(width/PTM_RATIO/2, height/PTM_RATIO/2);

	//Throws the box just created into a collision fixture so it can properly collide with things
	b2FixtureDef charShapeDef;
	charShapeDef.shape = &collisionShape;
	charShapeDef.density = 10.0f;
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
	side = charBody->CreateFixture(&sidesShapeDef);

	//Don't want the enemy falling over when he's hit, now, do we
	charBody->SetFixedRotation(true);
}


void Spherodroid::moveEnemy(){
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

void Spherodroid::moveLeft(){
	b2Vec2 velocity = charBody->GetLinearVelocity();
	float velocityChange = -0.664 - velocity.x;
	float impulse = charBody->GetMass() * velocityChange;
	charBody->ApplyLinearImpulse( b2Vec2(impulse,0), charBody->GetWorldCenter() );
}

void Spherodroid::moveRight(){
	b2Vec2 velocity = charBody->GetLinearVelocity();
	float velocityChange = 0.664 - velocity.x;
	float impulse = charBody->GetMass() * velocityChange;
	charBody->ApplyLinearImpulse( b2Vec2(impulse,0), charBody->GetWorldCenter() );
}
void Spherodroid::stop(){
	b2Vec2 velocity = charBody->GetLinearVelocity();
	float velocityChange = 0 - velocity.x;
	float impulse = charBody->GetMass() * velocityChange;
	charBody->ApplyLinearImpulse( b2Vec2(impulse,0), charBody->GetWorldCenter() );
}

void Spherodroid::die(){
	living = false;
	//charSprite->setIsVisible(false);
	state = 3;
	changeAction(3);
	charFixture->SetSensor(true);
	side->SetSensor(true);
	b2Filter guardFilter = charFixture->GetFilterData();
	guardFilter.maskBits = NOTHING;
	side->SetFilterData(guardFilter);
	charFixture->SetFilterData(guardFilter);
}

void Spherodroid::dead(){
	charSprite->setIsVisible(false);
}

Spherodroid::~Spherodroid(void)
{
}
