#pragma once
#include "Character.h"

class Hero :
	public Character
{ 
protected:
	bool				teleport;
	bool				teleporting;
	bool				jumping;
	int					originalState;
	int					jumpingTimeout;
	int					teleDistance;
	int					numFootContacts;
	int					numOfTeleports;
	int					originNumOfTele;
	int					teleportCoolDown;
	int					originTeleCoolDown;
	int					teleTimeout;
	float				b2Radius;
	CCSprite*			circle;
	CCSprite*			ball;
	CCSpriteBatchNode*	ballSpriteSheet;
	b2Fixture*			sensor[4];
	b2Fixture*			teleportDistance;
	b2Fixture*			feet;
	b2Fixture*			teleportFixture;
	b2Vec2				endTeleLoc;
	b2Vec2				startTeleLoc;

public:
	Hero(b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc, int startState = 3, float scale=1.0);
	~Hero(void);

	virtual void init(const char *spritePath, b2World * initialWorld, CCLayer* creationRoom, float PTM_RATIO, CCPoint* loc);

	virtual void setSprite(const char *spritePath, CCPoint* loc);
	virtual void setBody(b2World *initialWorld, float PTM_RATIO, CCPoint* loc);
	virtual void setFixture(float PTM_RATIO);

	bool startTeleport(b2Vec2 checkLoc, b2Fixture* roomFixture);
	bool endTeleport(b2Vec2 checkLoc, b2Fixture* roomFixture);
	void teleMax();
	void finishTeleport();
	void beginTeleporting();

	void isMoving(b2Vec2 checkLoc);
	void moveHero();
	void moveRight();
	void startJump();
	void endJump();
	void moveLeft();
	void stop();

	void preTeleport();
	void teleIdle();
	void teleMove();
	void fall();

	bool hasStopped();
	
	bool isTeleporting(){return teleport;}
	bool isTeleporting(b2Vec2 checkLoc){ return teleport && !charFixture->TestPoint(checkLoc) && teleportDistance->TestPoint(checkLoc);}
	bool canTeleport(){return numOfTeleports > 0;}
	bool hasTeleported(){return charFixture->TestPoint(endTeleLoc);}
	bool nowTeleporting(){return teleporting;}
	void decrementCoolDown();
	void decrementTeleport();
	void resetTeleCoolDown();
	int getNumberOfTeleports(){return numOfTeleports;}
	b2Vec2 getEndTeleLoc(){return endTeleLoc;}
	void   setEndTeleLoc(b2Vec2 endLoc){ endTeleLoc = b2Vec2(endLoc);}
	b2Vec2 getStartTeleLoc(){return startTeleLoc;}
	void   setStartTeleloc(b2Vec2 endLoc){ startTeleLoc = b2Vec2(endLoc);}

	bool isJumping(){return jumping && (jumpingTimeout == 0);}
	int getJumpingTimeout(){return jumpingTimeout;}
	void decrementJumpingTimeout(){if(jumpingTimeout != 0)jumpingTimeout--;}
	int resetJumpingTimeout(){jumpingTimeout = 15;}

	int getDirection(){return state;}
	int getOriginalState(){return originalState;}

	int getType(){return 3;}
	bool isInRange(b2Vec2 checkLoc){return teleportDistance->TestPoint(checkLoc);}
	bool isFeetTouching(b2Fixture* fixtureA, b2Fixture* fixtureB){return fixtureA == feet || fixtureB == feet;}
	CCSprite* getCircle(){return circle;}
	CCSprite* getBall(){return ball;}

	bool isTouching(b2Fixture* fixture);
	bool teleCollision(b2Fixture *fixtureA, b2Fixture *fixtureB);
	void setOriginalState(int oState){originalState = oState;}
	void setState(int myState){originalState = state; state = myState;}

	bool isMovingLeft(){return leftOrRight;}
	
	void reset();

	void die();

	void showBall();
	void hideBall();

	void superSaiyan(){}
};

