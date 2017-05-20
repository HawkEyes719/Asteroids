#include "Asteroids.h"
#include "Collision.h"
#include "ObjectFactory.h"

Asteroids::Asteroids()
{ 
	transform = mat4(1.0f);
	extent = vec2(0.4f);
	speed = 1.0f;
	alive = true;
	name = "Asteroid";

	direction = vec3(linearRand(-1.0f, 1.0f), linearRand(-1.0f, 1.0f), 0);
	setPosition(vec2(linearRand(-4.5f, 4.5f), linearRand(-4.5f, 4.5f)));
}


Asteroids::~Asteroids()
{
}

void Asteroids::update(float deltaTime)
{
	if (alive)
	{
		transform = translate(transform, direction * deltaTime * speed);
		appearOtherSide(this);
	}
}

void Asteroids::draw()
{
	if (alive)
	{
		Gizmos::add2DCircle(vec2(transform[3].x, transform[3].y), extent.x, 10, vec4(1, 1, 1, 0), &transform);
		//Gizmos::add2DAABB(vec2(transform[3].x, transform[3].y), extent, vec4(0, 1, 1, 0), &transform);
	}
}

//--------------------------------------------------------------------------------------------------------

LargeAsteroid::LargeAsteroid()
{
	extent = vec2(0.5f);
	name = "LargeAsteroid";
}

LargeAsteroid::~LargeAsteroid()
{
}

//--------------------------------------------------------------------------------------------------------

MediumAsteroid::MediumAsteroid()
{
	transform = mat4(1.0f);
	extent = vec2(0.25f);
	score = 50;
	name = "MediumAsteroid";

	//direction = vec3(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), 0);
	//transform = translate(transform, vec3(RandomFloat(-4.5f, 4.5f), RandomFloat(-4.5f, 4.5f), 0));
	//transform = glm::rotate(transform, radians(RandomFloat(0.0f, 3.12f)), vec3(0.0f, 0.0f, 1.0f));
}

MediumAsteroid::~MediumAsteroid()
{
}

//--------------------------------------------------------------------------------------------------------

SmallAsteroid::SmallAsteroid()
{
	transform = mat4(1.0f);
	extent = vec2(0.1f);
	score = 100;
	name = "SmallAsteroid";

	//direction = vec3(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), 0);
	//transform = translate(transform, vec3(RandomFloat(-4.5f, 4.5f), RandomFloat(-4.5f, 4.5f), 0));
	//transform = glm::rotate(transform, radians(RandomFloat(0.0f, 3.12f)), vec3(0.0f, 0.0f, 1.0f));
}

SmallAsteroid::~SmallAsteroid()
{
}