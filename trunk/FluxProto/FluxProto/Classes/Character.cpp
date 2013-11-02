#include "pch.h"
#include "Character.h"

USING_NS_CC;

Character::Character(const char *spritePath, b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc, int startState, float charScale)
	:scale(charScale), state(startState), width(300), height(600), leftOrRight(false){
	//init(spritePath, initialWorld, creationRoom, PTM_RATIO);
}
void Character::init(const char *spritePath, b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc){
	
	setSprite(spritePath, 300, 600, loc);
	
	creationRoom->addChild(charSpriteSheet, 2);
	
	CreateBox2DObject(initialWorld, PTM_RATIO, loc);
}

void Character::setSprite(const char* spritePath, int width, int height, CCPoint* loc){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	
	charSpriteSheet = CCSpriteBatchNode::batchNodeWithFile(spritePath);
	charSprite = CCSprite::spriteWithBatchNode(charSpriteSheet, CCRectMake(0, 0, width, height));
	charSpriteSheet->addChild(charSprite, 2);
	
	CCAnimation *moveRightAnimation = CCAnimation::animation();
	CCAnimation *moveLeftAnimation = CCAnimation::animation();
	moveRightAnimation->setDelay(0.15f);
	moveLeftAnimation->setDelay(0.15f);

	//Adds the eight frames of the moving right animation to the animation object from the spritesheet
	for(int j=0; j<8; j++){
		CCSpriteFrame *frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(j*300, 0, 300, 600));
		moveRightAnimation->addFrame(frame);
	}
	//Adds the eight frames of the moving left animation to the animation object from the spritesheet
	for(int j=0; j<8; j++){
		CCSpriteFrame *frame = CCSpriteFrame::frameWithTexture(charSpriteSheet->getTexture(), CCRectMake(j*300, 610, 300, 600));
		moveLeftAnimation->addFrame(frame);
	}
	//Applied both animaitons to the guard at once, which ends up just applying the moving left one.  It is repeated infinitely.
	CCAnimate *moveRightAnimate = CCAnimate::actionWithAnimation(moveRightAnimation, true);
	CCAnimate *moveLeftAnimate = CCAnimate::actionWithAnimation(moveLeftAnimation, true);
	CCRepeatForever* repeatRightAction = CCRepeatForever::actionWithAction(moveRightAnimate);
	CCRepeatForever* repeatLeftAction = CCRepeatForever::actionWithAction(moveLeftAnimate);

	//Places guard on screen and makes him moonwalk like a pro
	charActions = new CCMutableArray<CCAction*>;
	charActions->addObject(repeatLeftAction);
	charActions->addObject(repeatRightAction);
	changeAction(1);
	charSpriteSheet->setPosition(*loc);
	charSpriteSheet->setScale(scale);
	
}

CCSprite* Character::getSprite(){
	return charSprite;
}

void Character::changeAction(int index){
	int count = charActions->count();
	CCAssert(index >= 0 && index < count, "");
	if(charSprite->numberOfRunningActions() > 0){
		charSprite->stopAllActions();
	}
	charSprite->setOpacity(255);
	charSprite->runAction(charActions->getObjectAtIndex(index));
}

void Character::CreateBox2DObject(b2World *initialWorld, float PTM_RATIO, CCPoint *loc){
	//Creates the Box 2D collision body for the character, sets its position on the screen and puts it in the level
	setBody(initialWorld, PTM_RATIO, loc);

	//Draw the box for collision to the right scale and around the character
	setFixture(PTM_RATIO);
}

void Character::setBody(b2World *initialWorld, float PTM_RATIO, CCPoint *loc){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	b2BodyDef charBodyDef;
	charBodyDef.type = b2_dynamicBody;
	startPoint = b2Vec2(loc->x/PTM_RATIO, loc->y/PTM_RATIO);
	charBodyDef.position = startPoint;
	charBodyDef.userData = charSpriteSheet;
	charBody = initialWorld->CreateBody(&charBodyDef);
}

b2Body* Character::getBody(){
	return charBody;
}

void Character::setFixture(float PTM_RATIO){
	float width = (scale*charSprite->getContentSize().width);
	float height = (scale*charSprite->getContentSize().height);

	collisionShape.SetAsBox(width/PTM_RATIO/2, height/PTM_RATIO/2);

	//Throws the box just created into a collision fixture so it can properly collide with things
	b2FixtureDef charShapeDef;
	charShapeDef.shape = &collisionShape;
	charShapeDef.density = 10.0f;
	charShapeDef.friction = 5.0f;
	charShapeDef.restitution = 0.0f;
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

void Character::addAction(CCAction* newAction){
	charActions->addObject(newAction);
}

b2Fixture* Character::getFixture(){
	return charFixture;
}

Character::~Character(void)
{
}

Character* Character::isSpriteTag(int tagCheck) {
	if(charSpriteSheet->getTag() == tagCheck) {
		return this;
	}
	else {
		return NULL;
	}
}

void Character::die(){
	living = false;
	charSprite->setIsVisible(false);
	charFixture->SetSensor(true);
	//startPoint = b2Vec2(charBody->GetPosition());
	side->SetSensor(true);
	b2Filter guardFilter = charFixture->GetFilterData();
	guardFilter.maskBits = BOUNDARY;
	side->SetFilterData(guardFilter);
	charFixture->SetFilterData(guardFilter);
}

void Character::respawn(){
	/*if(living){
		startPoint = b2Vec2(charBody->GetPosition());
	}*/
	living = true;
	charSprite->setIsVisible(true);
	charFixture->SetSensor(false);
	side->SetSensor(false);
	charBody->SetTransform(startPoint, 0);
	b2Filter guardFilter = charFixture->GetFilterData();
	guardFilter.maskBits = HERO | BOUNDARY | SENSOR;
	side->SetFilterData(guardFilter);
	charFixture->SetFilterData(guardFilter);
}

bool Character::isTouching(b2Fixture *fixture){
	return fixture ==  charFixture || fixture == side;
}