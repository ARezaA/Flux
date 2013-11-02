#include "pch.h"
#include "Enemy.h"

int Enemy::tagID = 10;


void Enemy::init(const char *spritePath, b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, int width, int height, CCPoint* loc){
	setSprite(spritePath, width, height, loc);
	
	creationRoom->addChild(charSpriteSheet, 2);
	
	CreateBox2DObject(initialWorld, PTM_RATIO, loc);

	charSpriteSheet->setTag(tagID);
	tagID ++ ;
}

Enemy::Enemy(const char *spritePath, b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, int width, int height, CCPoint* loc, int attack,  float scale)
	:attackTimeout(attack), attackRange(NULL), Character(spritePath, initialWorld, creationRoom, PTM_RATIO, loc, 0, scale), movementCoolDown(0)
{
	//init(spritePath, initialWorld, creationRoom, PTM_RATIO, width, height, loc);
}


Enemy::~Enemy(void)
{
}

void Enemy::moveEnemy(){
	switch(state){
		case 0: moveRight();break;
		case 1: moveLeft();break;
		case 2: stop();break;
		default: break;
	}
}
