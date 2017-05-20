#pragma once

#include "Object.h"

class Bullet : public Object
{
public:
	Bullet();
	~Bullet();

	void update(float deltaTime);

	void draw();

protected:

	float timer;
	
	float velocity;
	
};

