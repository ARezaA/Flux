#include "pch.h"
#include "Bullet.h"

int Bullet::tagID = 200;

Bullet::Bullet(CCLayer *layer, b2World *world, float PTM_RATIO, b2Vec2 loc, b2Vec2 heroLocation, float setScale)
	:scale(setScale), width(128 * setScale), height(20 * setScale), live(true), location(b2Vec2(loc)), heroLoc(b2Vec2(heroLocation))
{
	init(layer, world, PTM_RATIO);
}

Bullet::~Bullet(void)
{
}

void Bullet::init(CCLayer *layer, b2World *world, float PTM_RATIO){
	createSprite(PTM_RATIO);
	layer->addChild(bulletSprite, 100,tagID);
	tagID++;

	createBody(world, PTM_RATIO);

	createFixture(world, PTM_RATIO);

	angle = b2Atan2(heroLoc.y - boxBody->GetPosition().y, heroLoc.x - boxBody->GetPosition().x);

	boxBody->SetTransform(location, angle);
	fireBullet();
}

void Bullet::createSprite(float PTM_RATIO){

	/*bulletSpriteSheet = CCSpriteBatchNode::batchNodeWithFile("spritesheet_bullet.png");
	bulletSprite = CCSprite::spriteWithBatchNode(bulletSpriteSheet, CCRectMake(0, 0, 128, 128));
	bulletSpriteSheet->addChild(bulletSprite, 2);

	CCAnimation *bulletAnimation = CCAnimation::animation();
	bulletAnimation->setDelay(1.0f);

	for (int i=0; i<1; i++) {
		CCSpriteFrame *frame = CCSpriteFrame::frameWithTexture(bulletSpriteSheet->getTexture(), CCRectMake(128*3, 0, 128, 128));
		bulletAnimation->addFrame(frame);
	}

	CCAnimate *bulletAnimate = CCAnimate::actionWithAnimation(bulletAnimation, true);
	CCRepeatForever* repeatBulletAction = CCRepeatForever::actionWithAction(bulletAnimate);

	runAction(repeatBulletAction);
	bulletSpriteSheet->setScale(scale*3);
	bulletSpriteSheet->setPosition(location.x * PTM_RATIO, location.y * PTM_RATIO);*/

	bulletSprite = CCSprite::spriteWithFile("spritesheet_bullet.png", CCRectMake(128*3, 0, 128, 128));
	bulletSprite->setScale(scale);
	bulletSprite->setPosition(ccp(location.x * PTM_RATIO, location.y * PTM_RATIO));
	
}

void Bullet::createBody(b2World* world, float PTM_RATIO){
	b2BodyDef charBodyDef;
	charBodyDef.type = b2_dynamicBody;
	charBodyDef.position = b2Vec2(location);
	charBodyDef.userData = bulletSprite;
	charBodyDef.fixedRotation = true;
	//charBodyDef.gravityScale = 0;
	boxBody = world->CreateBody(&charBodyDef);
}

void Bullet::createFixture(b2World* world, float PTM_RATIO){

	b2PolygonShape collisionShape;
	collisionShape.SetAsBox(width/PTM_RATIO/2, height/PTM_RATIO/2);

	//Throws the box just created into a collision fixture so it can properly collide with things
	b2FixtureDef charShapeDef;
	charShapeDef.shape = &collisionShape;
	charShapeDef.density = 1.0f;
	charShapeDef.restitution = 0.0f;
	charShapeDef.filter.categoryBits = BULLET;
	charShapeDef.filter.maskBits = HERO | BOUNDARY;
	boxFixture = boxBody->CreateFixture(&charShapeDef);
}

void Bullet::fireBullet(){
	b2Vec2 velocity = boxBody->GetLinearVelocity();
	float speed = 1;
	float slope = (heroLoc.y - location.y) / (heroLoc.x - location.x);
	float velocityYChange = speed * sin(angle);
	float velocityXChange = speed * cos(angle);
	float impulseY = boxBody->GetMass() * velocityYChange;
	float impulseX = boxBody->GetMass() * velocityXChange;
	boxBody->ApplyLinearImpulse( b2Vec2(impulseX, impulseY), boxBody->GetWorldCenter() );
}

void Bullet::die(){
	live = false;
	bulletSprite->setIsVisible(false);
	boxFixture->SetSensor(true);
	b2Filter guardFilter = boxFixture->GetFilterData();
	guardFilter.maskBits = NOTHING;
	boxFixture->SetFilterData(guardFilter);
	tagID--;
}