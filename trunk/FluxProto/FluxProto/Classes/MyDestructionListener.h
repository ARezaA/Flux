#pragma once
#include "Box2D\Box2D.h"

class MyDestructionListener:public b2DestructionListener
{
public:
	MyDestructionListener(void);
	~MyDestructionListener(void);

	void SayGoodbye(b2Joint* joint){
		joint->GetBodyB()->GetWorld()->DestroyJoint(joint);
	}
};

