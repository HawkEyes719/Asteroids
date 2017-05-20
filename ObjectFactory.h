#pragma once

#include "Player.h"
#include "Asteroids.h"
#include "Bullet.h"
#include "Saucer.h"

class ObjectFactory
{
public:
	// factory method
	static Object* CreateObject(const char* typeName)
	{
		if (strcmp(typeName, "Player") == 0)
			return new Player;
		if (strcmp(typeName, "Bullet") == 0)
			return new Bullet;
		if (strcmp(typeName, "Asteroids") == 0)
			return new Asteroids;
		if (strcmp(typeName, "Saucer") == 0)
			return new Saucer;
		if (strcmp(typeName, "LargeAsteroid") == 0)
			return new LargeAsteroid;
		if (strcmp(typeName, "MediumAsteroid") == 0)
			return new MediumAsteroid;
		if (strcmp(typeName, "SmallAsteroid") == 0)
			return new SmallAsteroid;

		//Oh no! Unknown type 
		cout << "Unknown type: " << typeName << endl;

		return 0;
	}
};