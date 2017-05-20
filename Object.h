#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <ctime>

using namespace glm;
using namespace std;

#include "Gizmos.h"

#define SCREENWIDTH 800
#define SCREENHEIGHT 600

class Object
{
public:
	virtual void update(float deltaTime) {}
	virtual void draw() {}

	mat4 getTransform() { return transform; }
	vec2 getPosition() { return transform[3]; }
	vec2 getSize() { return extent; }
	vec2 getMin() { return min = vec2(getPosition()) - getSize(); }
	vec2 getMax() { return max = vec2(getPosition()) + getSize(); }
	string getName() { return name; }
	bool getAlive() { return alive; }

	void setTransform(mat4 transform)
	{ 
		this->transform = transform; 
	}

	void setPosition(vec2 position) 
	{ 
		transform[3].x = position.x;
		transform[3].y = position.y;
	}

	void setSize(vec2 size)
	{
		this->extent = size;
	}

	void setPositionX(float positionX) { transform[3].x = positionX; }
	void setPositionY(float positionY) { transform[3].y = positionY; }
	void setAlive(bool isAlive) { alive = isAlive; }

	//----------------------------------------------------------------------------------------------------------------

	bool appearOtherSide(Object* object, float left = -5.0f, float right = 5.0f, float bottom = -5.0f, float top = 5.0f)
	{
		if (object->getPosition().x > right + extent.x)
		{
			object->setPositionX(left - extent.x);
			return true;
		}
		if (object->getPosition().x < left - extent.x)
		{
			object->setPositionX(right + extent.x);
			return true;
		}
		if (object->getPosition().y < bottom - extent.y)
		{
			object->setPositionY(top + extent.y);
			return true;
		}
		if (object->getPosition().y > top + extent.y)
		{
			object->setPositionY(bottom - extent.y);
			return true;
		}

		return false;
	}

	float RandomFloat(float a, float b)
	{
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = b - a;
		float r = diff * random;
		return a + r;
	}

	float speed;
	vec3 direction;

protected:

	mat4 transform;
	vec2 extent;

	// for collision box
	string name;
	vec2 min;
	vec2 max;
	bool alive;
	
};

