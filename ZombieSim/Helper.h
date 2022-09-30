#pragma once
#include "ZS_PropertyBag.h"
#include "ClearVector.h"
#include "Management.h"
#include "stdafx.h"


//We set the collision detection category in the Human object.
//Scenery and IMMUNE does not collide with anything.
//Human only collides with Zombie and vice versa

#define ERR_NOERROR 0


class CGlobal
{
public:
	CZombieSimPropertyBag propertyBag;
	CManagement management;
	float fElapsedTimeProperty; //on purpose in public, due to repeat adjustments.
	boost::posix_time::ptime m_startTime;
	


	CGlobal() 
	{
		m_startTime = boost::posix_time::second_clock::local_time();
		//b2Vec2 gravity(0.0f, -10.0f);
		//world = new b2World(gravity);


		//Needs to set one first
		//b2BodyDef groundBodyDef;
		//groundBodyDef.position.Set(0.0f, -10.0f);

		//b2Body* groundBody = world->CreateBody(&groundBodyDef);

		////Set Gravity to zero, since we don't do physical collisions only collision detection.
		//world->SetGravity(b2Vec2(0.0,0.0));

	};
	~CGlobal() {};


	

};

extern CGlobal glb;


#define CHECK_RETURNVALUE(value)        \
if(value!=ERR_NOERROR)                  \
{                                       \
   glb.management.ErrorChecker(value);  \
   return (int) value;                        \
}                              

#define CHECK_RETURNVALUE_REL_PTR(value, ptr)        \
if(value!=ERR_NOERROR)                  \
{                                       \
    delete ptr;                         \
     glb.management.ErrorChecker(value); \
   return (int) value;                  \
}

