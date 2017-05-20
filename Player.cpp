#include "Player.h"
#include "Collision.h"
#include "TextRender.h"
#include "ObjectFactory.h"
#include <glm/gtx/euler_angles.hpp>

Player::Player()
{
	transform		= mat4(1.0f);
	extent			= vec2(0.1f, 0.2f);
	lives			= 3;
	fire			= false;
	alive			= true;
	result			= FModManager::System()->createSound("audio/fire.wav", FMOD_DEFAULT, 0, &sound);
	result			= FModManager::System()->createSound("audio/thrust.wav", FMOD_LOOP_NORMAL, 0, &sound2);
	name			= "Player";
	dragFactor		= 0.25f;
	direction		= 90.f;

	thrustPosition = vec2(1);;
	thrustTransform = mat4(1);
	thrustOn = false;
	FModManager::System()->playSound(sound2, 0, false, &channel2);
}


Player::~Player()
{
	sound->release();
	sound2->release();
}

void Player::rotate(float degree)
{
	// rotate on z axis
	direction += degree;

	if (direction <= 0)
		direction = 360;
	else if (direction >= 360)
		direction = 0;

	float newDirection = radians(degree);
	transform = glm::rotate(transform, newDirection, vec3(0.0f, 0.0f, 1.0f));

	thrustTransform = glm::rotate(thrustTransform, newDirection, vec3(0.0f, 0.0f, 1.0f));
}

void Player::accelerate(float acceleration)
{
	float angle = (3.14 / 180) * (direction);
	force.x = std::cos(angle) * acceleration;
	force.y = std::sin(angle) * acceleration;

	velocity.x += force.x / 500;
	velocity.y += force.y / 500;

	// thruster transform
	thrustTransform[3].x = transform[3].x - (std::cos(angle) * 0.3f);
	thrustTransform[3].y = transform[3].y - (std::sin(angle) * 0.3f);
}

void Player::respawn()
{
	respawnTimer = 0;
	setTransform(mat4(1.0f));
	setAlive(true);
}

void Player::update(float deltaTime)
{
	if (alive)
	{
		input(deltaTime);

		if (inHyperspace)
		{
			velocity = vec3(0);
			hyperspaceTimer += 2.0f * deltaTime;

			if (hyperspaceTimer >= 1.0f)
			{
				inHyperspace = false;
				transform[3].x = linearRand(-5.0f, 5.0f);
				transform[3].y = linearRand(-5.0f, 5.0f);
				hyperspaceTimer = 0;
			}
		}

		if(hyperspace == false)
			appearOtherSide(this);

		// Update position based on movement speed (velocity vector)
		transform[3].x += velocity.x;
		transform[3].y += velocity.y;

		// Use Stokes' law to apply drag to the ship
		velocity.x = velocity.x - velocity.x * (dragFactor * deltaTime);
		velocity.y = velocity.y - velocity.y * (dragFactor * deltaTime);

		if (velocity.x == 0.0f || velocity.y == 0.0f)
			cout << "velocity = 0?" << endl;
	}

	if (!alive && !inSpawnArea)
	{
		setPosition(vec2(100));
		thrustTransform = mat4(1);
		direction = 90;
		velocity = vec3(0);
		result = channel2->setPaused(true);
		respawnTimer += 3.0f * deltaTime;

		if(respawnTimer >= 3.0f)
			respawn();
	}
}

void Player::input(float deltaTime)
{
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotate(deltaTime * -360.0f);
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS)
		rotate(deltaTime * 360.0f);

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS)
	{
		accelerate(deltaTime * 1.0f);
 		result = channel2->setPaused(false);
		thrustOn = true;
	}
	else if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_RELEASE)
	{
		result = channel2->setPaused(true);
		thrustOn = false;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS && hyperspace == false)
	{
		transform[3].x = 100;
		transform[3].y = 100;
		inHyperspace = true;
		hyperspace = true;
	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
		hyperspace = false;
}

Object* Player::shootBullet()
{
	Object* bulletObj = nullptr;

	//--------------------------------------------------------------------
	// bullet shooting from player
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS && fireBullet == false && alive)
	{
		bulletObj = ObjectFactory::CreateObject("Bullet");
		bulletObj->setPosition(getPosition());
		vec3 dir = vec3(getTransform()[1].x, getTransform()[1].y, 0);
		bulletObj->direction = dir;
		bulletObj->getMin();
		bulletObj->getMax();

		FModManager::System()->playSound(sound, 0, false, &channel);
		fireBullet = true;
	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_RELEASE)
		fireBullet = false;

	return bulletObj;
}

void Player::draw()
{
	//-------
	vec2 center = vec2(transform[3].x, transform[3].y);
	//-------

	if (alive)
	{
		Gizmos::add2DTriangle(center, extent, vec4(1, 1, 1, 0), &transform);
		//Gizmos::add2DAABB(vec2(transform[3].x, transform[3].y), extent, vec4(0, 1, 1, 0), &transform);

		if (thrustOn)
		{
			Gizmos::add2DTriangle(vec2(thrustTransform[3].x, thrustTransform[3].y), vec2(extent.x, extent.y - 0.25f), vec4(1, 0, 0, 0), &thrustTransform);			
		}
	}

	for(int i = 0; i < lives; ++i)
		Gizmos::add2DTriangle(vec2(-4.5f + (i * 0.25f), 4.5f), vec2(0.1f, 0.2f), vec4(1, 1, 1, 0));

	
}

