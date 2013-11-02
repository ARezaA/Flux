#include "pch.h"
#include "MyContactListener.h"


MyContactListener::MyContactListener(void): _contacts()
{
}


MyContactListener::~MyContactListener(void)
{
}

void MyContactListener::BeginContact(b2Contact* contact){
	MyContact myContact = {contact->GetFixtureA(), contact->GetFixtureB()};
	_contacts.push_back(myContact);
}

void MyContactListener::EndContact(b2Contact* contact){
	MyContact myContact = {contact->GetFixtureA(), contact->GetFixtureB()};
	std::vector<MyContact>::iterator pos;
	pos = std::find(_contacts.begin(), _contacts.end(), myContact);
	if(pos != _contacts.end()){
		_contacts.erase(pos);
	}
}

void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){

}

void MyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){

}