#pragma once

#include "Object.h"
#include "Bullet.h"
#include "FModManager.h"

class Saucer : public Object
{
public:
	Saucer();
	~Saucer();

	void update(float deltaTime);
	void draw();

	void movementDecision();
	Object* shootBullet(float deltaTime, vec2 playerPos, bool playerIsAlive);

	float velocity;

	float decisionTimer = 0.0f;
	float shootTimer = 0.0f;
	float shootTime = linearRand(1.0f, 1.5f);
	int decision;
	int randLoc;
	int randSize;
	bool despawn = false;
	bool changeDecision = false;
	

	// Saucer sounds
	FMOD_RESULT result;
	FMOD::Sound *sound;
	FMOD::Channel *channel;

	FMOD::Sound *sound2;
	FMOD::Channel *channel2;
};

