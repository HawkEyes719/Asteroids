#include "Saucer.h"
#include "ObjectFactory.h"


Saucer::Saucer()
{
	transform = mat4(1.0f);
	
	alive = true;
	result = FModManager::System()->createSound("audio/saucerBig.wav", FMOD_LOOP_NORMAL, 0, &sound);
	result = FModManager::System()->createSound("audio/fire.wav", FMOD_DEFAULT, 0, &sound2);
	FModManager::System()->playSound(sound, 0, false, &channel);
	shootTimer = 0;
	randLoc = linearRand(1, 2);
	movementDecision();

	randSize = linearRand(1, 2);
	if (randSize == 1)
	{
		name = "LargeSaucer";
		extent = vec2(0.25f, 0.25f);
		velocity = linearRand(1.0f, 1.5f);
	}
	else
	{
		name = "SmallSaucer";
		extent = vec2(0.1f, 0.1f);
		velocity = linearRand(1.5f, 2.0f);
	}

	if (randLoc == 1)
		setPosition(vec2(-5.0f, linearRand(-5.0f, 5.0f)));
	else
		setPosition(vec2(5.0f, linearRand(-5.0f, 5.0f)));
}


Saucer::~Saucer()
{
	sound->release();
	sound2->release();
}

void Saucer::movementDecision()
{
	decision = linearRand(1, 3);

	// traveling left
	if (randLoc == 2)
	{
		switch (decision)
		{
		case 1:
			direction = vec3(-1, 0, 0);
			break;
		case 2:
			direction = vec3(-1, -1, 0);
			break;
		case 3:
			direction = vec3(-1, 1, 0);
			break;
		}
	}

	// traveling right
	if (randLoc == 1)
	{
		switch (decision)
		{
		case 1:
			direction = vec3(1, 0, 0);
			break;
		case 2:
			direction = vec3(1, -1, 0);
			break;
		case 3:
			direction = vec3(1, 1, 0);
			break;
		}
	}

	changeDecision = false;
}

Object* Saucer::shootBullet(float deltaTime, vec2 playerPos, bool playerIsAlive)
{
	Object* bulletObj = nullptr;
	
	if (alive)
	{
		shootTimer += 1.0f * deltaTime;

		if (shootTimer >= shootTime)
		{
			shootTimer = 0;
			shootTime = linearRand(1.0f, 1.5f);

			bulletObj = ObjectFactory::CreateObject("Bullet");
			bulletObj->setPosition(getPosition());

			vec2 dir = vec2(1);
			if (name == "SmallSaucer" && playerIsAlive == true)
				dir = normalize(playerPos - this->getPosition());
			else
				dir = normalize(linearRand(-5.0f, 5.0f) - this->getPosition());


			bulletObj->direction = vec3(dir, 0);
			bulletObj->getMin();
			bulletObj->getMax();
			FModManager::System()->playSound(sound2, 0, false, &channel2);
		}
	}
	else
		shootTimer = 0;
	
	return bulletObj;
}

void Saucer::update(float deltaTime)
{
	if (alive)
	{
		result = channel->setPaused(false);
		decisionTimer += 0.5f * deltaTime;
		transform = translate(transform, direction * deltaTime * velocity);
		bool goAway = appearOtherSide(this);

		if (decisionTimer >= 3.0)
		{
			changeDecision = true;
			decisionTimer = 0;
		}
		if (changeDecision)
			movementDecision();

		if (transform[3].x <= -5.0f || transform[3].x >= 5.0f)
			alive = false;
	}
	
	if(!alive)
		result = channel->setPaused(true);
}

void Saucer::draw()
{
	if (alive)
	{
		Gizmos::add2DTriangle(vec2(transform[3].x, transform[3].y), extent, vec4(0, 1, 0, 0), &transform);
		//Gizmos::add2DAABB(vec2(transform[3].x, transform[3].y), extent, vec4(0, 1, 1, 0), &transform);
	}

}