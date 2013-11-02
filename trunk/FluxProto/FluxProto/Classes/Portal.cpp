#include "pch.h"
#include "Portal.h"

Portal::Portal(CCLayer *layer, b2World *world, float PTM_RATIO, float pixX, float pixY, float pixWidth, float pixHeight, float p1x, float p1y, float p2x, float p2y, bool debugMode, bool hard, float setScale, bool isVisible)
	:scale(setScale), width(pixWidth * setScale), height(pixHeight * setScale), debug(debugMode), solid(hard), visible(isVisible),
	 location(b2Vec2(((pixX + pixWidth/2) * scale)/PTM_RATIO, ((pixY + pixHeight/2) * scale)/PTM_RATIO)), 
	 start(b2Vec2((p1x/PTM_RATIO) * scale, (p1y/PTM_RATIO) *scale)), end(b2Vec2((p2x/PTM_RATIO) * scale, (p2y/PTM_RATIO) *scale)), coolDown(0)
{
	init(layer, world, PTM_RATIO);
}


Portal::~Portal(void)
{
}

void Portal::init(CCLayer *layer, b2World *world, float PTM_RATIO){
	if(debug){
		createSprite(PTM_RATIO);
		layer->addChild(boxSprite);
	}

	createTexture(PTM_RATIO);
	layer->addChild(textureSprite);

	createBody(world, PTM_RATIO);

	createFixture(world, PTM_RATIO);
}

void Portal::createSprite(float PTM_RATIO){
	boxSprite = CCSprite::spriteWithFile("BoxFace.png");
	boxSprite->setPosition( ccp(location.x*PTM_RATIO, location.y*PTM_RATIO) );
	textureSprite->setScaleX((width)/(textureSprite->getContentSize().width));
	textureSprite->setScaleY((height)/(textureSprite->getContentSize().height));
}

void Portal::createTexture(float PTM_RATIO){
	textureSprite = CCSprite::spriteWithFile("teleTexture.png");
	textureSprite->setPosition( ccp(location.x*PTM_RATIO, location.y*PTM_RATIO) );
	textureSprite->setScaleX(width/(textureSprite->getContentSize().width));
	textureSprite->setScaleY(height/(textureSprite->getContentSize().height));
	textureSprite->setIsVisible(false);
}

void Portal::createBody(b2World* world, float PTM_RATIO){
	b2BodyDef charBodyDef;
	charBodyDef.type = b2_staticBody;
	charBodyDef.position = b2Vec2(location);
	if(debug)
		charBodyDef.userData = boxSprite;
	boxBody = world->CreateBody(&charBodyDef);
}

void Portal::createFixture(b2World* world, float PTM_RATIO){
	
	b2PolygonShape portalShape;

	portalShape.SetAsBox(width/PTM_RATIO/2, height/PTM_RATIO/2);

	//Throws the box just created into a collision fixture so it can properly collide with things
	b2FixtureDef charShapeDef;
	charShapeDef.shape = &portalShape;
	charShapeDef.isSensor = true;
	charShapeDef.density = 10.0f;
	charShapeDef.restitution = 0.0f;
	charShapeDef.filter.categoryBits = PORTAL;
	charShapeDef.filter.maskBits = TELEPORT;
	boxFixture = boxBody->CreateFixture(&charShapeDef);

	if(solid){
		b2PolygonShape collisionShape;

		collisionShape.SetAsBox(width/PTM_RATIO/2, height/PTM_RATIO/2);

		//Throws the box just created into a collision fixture so it can properly collide with things
		b2FixtureDef colShapeDef;
		colShapeDef.shape = &collisionShape;
		//charShapeDef.isSensor = true;
		colShapeDef.density = 10.0f;
		colShapeDef.restitution = 0.0f;
		colShapeDef.filter.categoryBits = BOUNDARY;
		colShapeDef.filter.maskBits = HERO;
		boxBody->CreateFixture(&colShapeDef);
	}

}

bool Portal::teleCollision(b2Fixture *fixtureA, b2Fixture *fixtureB){
	if(boxFixture == fixtureA || boxFixture == fixtureB){
		b2Filter portalFilter = boxFixture->GetFilterData();
		portalFilter.maskBits = NOTHING;
		boxFixture->SetFilterData(portalFilter);
		coolDown = 120;
		return true;
	}
	return false;
}

void Portal::resetPortal(){
	
	if(coolDown > 1){
		coolDown--;
	}else if(coolDown == 1){
		b2Filter portalFilter = boxFixture->GetFilterData();
		portalFilter.maskBits = TELEPORT;
		boxFixture->SetFilterData(portalFilter);
		coolDown--;
	}
}

void Portal::startAndEnd(b2Vec2 hero){
	if(sqrt((hero.x - start.x) * (hero.x - start.x) + (hero.y - start.y) * (hero.y - start.y)) > sqrt((hero.x - end.x) * (hero.x - end.x) + (hero.y - end.y) * (hero.y - end.y))){
		b2Vec2 temp = b2Vec2(start);
		start = b2Vec2(end);
		end = b2Vec2(temp);
	}
}

void Portal::showTexture(){
	if(visible)
	textureSprite->setIsVisible(true);
}

void Portal::hideTexture(){
	if(visible)
	textureSprite->setIsVisible(false);
}