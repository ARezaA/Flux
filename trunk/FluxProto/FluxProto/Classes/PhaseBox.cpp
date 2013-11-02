#include "pch.h"
#include "PhaseBox.h"

PhaseBox::PhaseBox(CCLayer *layer, b2World *world, float PTM_RATIO, float pixX, float pixY, float pixWidth, float pixHeight, bool debugMode, float setScale, bool isVisible)
	:scale(setScale), width(pixWidth * setScale), height(pixHeight * setScale), debug(debugMode), visible(isVisible),
	location(b2Vec2(((pixX + pixWidth/2) * scale)/PTM_RATIO, ((pixY + pixHeight/2) * scale)/PTM_RATIO))
{
	init(layer, world, PTM_RATIO);
}


PhaseBox::~PhaseBox(void)
{
}

void PhaseBox::init(CCLayer *layer, b2World *world, float PTM_RATIO){
	if(debug){
		createSprite(PTM_RATIO);
		layer->addChild(boxSprite);
	}

	createTexture(PTM_RATIO);
	layer->addChild(textureSprite);

	createBody(world, PTM_RATIO);

	createFixture(world, PTM_RATIO);
}

void PhaseBox::createSprite(float PTM_RATIO){
	boxSprite = CCSprite::spriteWithFile("BoxFace.png");
	boxSprite->setPosition( ccp(location.x*PTM_RATIO, location.y*PTM_RATIO) );
	textureSprite->setScaleX(width/(textureSprite->getContentSize().width));
	textureSprite->setScaleY(height/(textureSprite->getContentSize().height));
	boxSprite->setScale(scale);
}

void PhaseBox::createTexture(float PTM_RATIO){
	textureSprite = CCSprite::spriteWithFile("teleTexture.png");
	textureSprite->setPosition( ccp(location.x*PTM_RATIO, location.y*PTM_RATIO) );
	textureSprite->setScaleX(width/(textureSprite->getContentSize().width));
	textureSprite->setScaleY(height/(textureSprite->getContentSize().height));
	textureSprite->setIsVisible(false);
}

void PhaseBox::createBody(b2World* world, float PTM_RATIO){
	b2BodyDef charBodyDef;
	charBodyDef.type = b2_staticBody;
	charBodyDef.position = b2Vec2(location);
	if(debug)
		charBodyDef.userData = boxSprite;
	boxBody = world->CreateBody(&charBodyDef);
}

void PhaseBox::createFixture(b2World* world, float PTM_RATIO){

	b2PolygonShape collisionShape;
	collisionShape.SetAsBox(width/PTM_RATIO/2, height/PTM_RATIO/2);

	//Throws the box just created into a collision fixture so it can properly collide with things
	b2FixtureDef charShapeDef;
	charShapeDef.shape = &collisionShape;
	charShapeDef.density = 10.0f;
	charShapeDef.restitution = 0.0f;
	charShapeDef.filter.categoryBits = BOUNDARY;
	charShapeDef.filter.maskBits = HERO | ENEMY | SENSOR;
	boxFixture = boxBody->CreateFixture(&charShapeDef);
}

void PhaseBox::showTexture(){
	if(visible)
	textureSprite->setIsVisible(true);
}

void PhaseBox::hideTexture(){
	if(visible)
	textureSprite->setIsVisible(false);
}