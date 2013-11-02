#include "pch.h"
#include "CollisionObject.h"


CollisionObject::CollisionObject(b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc, CCSprite *sprite, int type, float width, float height, float scale):scale(scale), type(type), room(creationRoom){
	init(sprite, initialWorld, PTM_RATIO, loc, width, height);
}
void CollisionObject::init(CCSprite *sprite, b2World * initialWorld, float PTM_RATIO, CCPoint* loc, float width, float height){
	sprites = new CCMutableArray<CCSprite*>;
	addSprite(sprite, width, height, loc);
	
	CreateBox2DObject(initialWorld, PTM_RATIO, loc, width, height);

}

void CollisionObject::addSprite(CCSprite *sprite, float width, float height, CCPoint* loc){
	width *= 300 ;
	height *= 300 ;

	sprite->setPosition(*loc);
	sprite->setScale(scale);
	room->addChild(sprite, 2);
	sprites->addObject(sprite);
}

void CollisionObject::CreateBox2DObject(b2World *initialWorld, float PTM_RATIO, CCPoint *loc, float width, float height){
	//Creates the Box 2D collision body for the character, sets its position on the screen and puts it in the level
	setBody(initialWorld, PTM_RATIO, loc);

	//Draw the box for collision to the right scale and around the character
	setFixture(PTM_RATIO, width, height);
}

void CollisionObject::setBody(b2World *initialWorld, float PTM_RATIO, CCPoint *loc){
	b2BodyDef objectBodyDef;
	objectBodyDef.type = b2_staticBody;
	objectBodyDef.position.Set(loc->x/PTM_RATIO, loc->y/PTM_RATIO);
	objectBodyDef.userData = sprites->getObjectAtIndex(0);
	objectBody = initialWorld->CreateBody(&objectBodyDef);
}

b2Body* CollisionObject::getBody(){
	return objectBody;
}

void CollisionObject::setFixture(float PTM_RATIO, float width, float height){
	width *= 300 * scale ;
	height = 300 *scale ;
	collisionShape.SetAsBox(width/PTM_RATIO/2, height/PTM_RATIO/2);

	//Throws the box just created into a collision fixture so it can properly collide with things
	b2FixtureDef objectShapeDef;				
	objectShapeDef.shape = &collisionShape;
	objectShapeDef.density = 10.0f;
	objectShapeDef.friction = 5.0f;
	objectShapeDef.restitution = 0.0f;
	objectFixture = objectBody->CreateFixture(&objectShapeDef);

	//No rotatey platforms
	objectBody->SetFixedRotation(true);
}

b2Fixture* CollisionObject::getFixture(){
	return objectFixture;
}

CollisionObject::~CollisionObject(void)
{
}
