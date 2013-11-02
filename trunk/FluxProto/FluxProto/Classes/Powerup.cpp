#include "pch.h"
#include "Powerup.h"


Powerup::Powerup(CCLayer *layer, b2World *world, float PTM_RATIO, float posX, float posY, bool isUsed, float setScale)
	:scale(setScale),location(ccp(((posX + 256/2) * setScale), ((posY + 256/2) * setScale))), used(isUsed)
{
	init(layer, world, PTM_RATIO);
}


Powerup::~Powerup(void)
{
}

void Powerup::init(CCLayer *layer, b2World *world, float PTM_RATIO){
	createSprite();
	layer->addChild(powSprite);

	createBody(world, PTM_RATIO);

	createFixture(world, PTM_RATIO);
}

void Powerup::createSprite(){
	powSprite = CCSprite::spriteWithFile("cake.png");
	powSprite->setPosition( location );
	powSprite->setScale(scale);
}

void Powerup::createBody(b2World* world, float PTM_RATIO){
	b2BodyDef charBodyDef;
	charBodyDef.type = b2_staticBody;
	charBodyDef.position = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
	charBodyDef.userData = powSprite;
	powBody = world->CreateBody(&charBodyDef);
}

void Powerup::createFixture(b2World* world, float PTM_RATIO){
	
	float width = powSprite->getContentSize().width  * scale;
	float height = powSprite->getContentSize().height * scale;

	b2PolygonShape collisionShape;
	collisionShape.SetAsBox(width/PTM_RATIO/2, height/PTM_RATIO/2);

	//Throws the box just created into a collision fixture so it can properly collide with things
	b2FixtureDef charShapeDef;
	charShapeDef.shape = &collisionShape;
	charShapeDef.isSensor = true;
	charShapeDef.density = 10.0f;
	charShapeDef.friction = 5.0f;
	charShapeDef.restitution = 0.0f;
	charShapeDef.filter.categoryBits = POWERUP;
	charShapeDef.filter.maskBits = HERO | TELEPORT;
	powFixture = powBody->CreateFixture(&charShapeDef);
}

bool Powerup::isTouching(b2Fixture* fixtureA, b2Fixture* fixtureB){
	return fixtureA == powFixture || fixtureB == powFixture;
}

void Powerup::usePowerup(){
	powSprite->setIsVisible(false);
	powFixture->SetSensor(true);
	used = true;

	b2Filter powFilter = powFixture->GetFilterData();
	powFilter.maskBits = NOTHING;
	powFixture->SetFilterData(powFilter);
}

void Powerup::resetPowerup(){
	powSprite->setIsVisible(true);
	powFixture->SetSensor(false);
	used = false;

	b2Filter powFilter = powFixture->GetFilterData();
	powFilter.maskBits = HERO;
	powFixture->SetFilterData(powFilter);
}