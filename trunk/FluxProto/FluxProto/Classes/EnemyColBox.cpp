#include "pch.h"
#include "EnemyColBox.h"

EnemyColBox::EnemyColBox(CCLayer *layer, b2World *world, float PTM_RATIO, float pixX, float pixY, float pixWidth, float pixHeight, bool debugMode, float setScale)
	:scale(setScale), width(pixWidth * setScale), height(pixHeight * setScale), debug(debugMode),
	location(b2Vec2(((pixX + pixWidth/2) * scale)/PTM_RATIO, ((pixY + pixHeight/2) * scale)/PTM_RATIO))
{
	init(layer, world, PTM_RATIO);
}


EnemyColBox::~EnemyColBox(void)
{
}

void EnemyColBox::init(CCLayer *layer, b2World *world, float PTM_RATIO){
	if(debug){
		createSprite();
		layer->addChild(boxSprite);
	}
	createBody(world, PTM_RATIO);

	createFixture(world, PTM_RATIO);
}

void EnemyColBox::createSprite(){
	boxSprite = CCSprite::spriteWithFile("BoxFace.png");
	boxSprite->setPosition( ccp(location.x*256, location.y*256) );
	boxSprite->setScale(scale);
}

void EnemyColBox::createBody(b2World* world, float PTM_RATIO){
	b2BodyDef charBodyDef;
	charBodyDef.type = b2_staticBody;
	charBodyDef.position = b2Vec2(location);
	if(debug)
		charBodyDef.userData = boxSprite;
	boxBody = world->CreateBody(&charBodyDef);
}

void EnemyColBox::createFixture(b2World* world, float PTM_RATIO){

	b2PolygonShape collisionShape;
	collisionShape.SetAsBox(width/PTM_RATIO/2, height/PTM_RATIO/2);

	//Throws the box just created into a collision fixture so it can properly collide with things
	b2FixtureDef charShapeDef;
	charShapeDef.shape = &collisionShape;
	charShapeDef.isSensor = true;
	charShapeDef.density = 10.0f;
	charShapeDef.restitution = 0.0f;
	charShapeDef.filter.categoryBits = SENSOR;
	charShapeDef.filter.maskBits = ENEMY;
	boxFixture = boxBody->CreateFixture(&charShapeDef);
}

bool EnemyColBox::isTouching(b2Fixture *fixtureA, b2Fixture *fixtureB){
	return boxFixture == fixtureA || boxFixture == fixtureB;
}