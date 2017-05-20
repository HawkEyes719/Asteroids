#pragma once

#include "Object.h"
#include "Bullet.h"
#include "FModManager.h"

class Player : public Object
{
public:
	Player();
	~Player();

	void update(float deltaTime);

	void draw();

	void input(float deltaTime);

	void rotate(float degree);

	void accelerate(float acceleration);

	void respawn();

	// Player sounds
	FMOD_RESULT result;
	FMOD::Sound *sound;
	FMOD::Channel *channel;

	FMOD::Sound *sound2;
	FMOD::Channel *channel2;

	float hyperspaceTimer = 0;
	bool inHyperspace = false;
	int lives;
	float respawnTimer = 0;
	bool hyperspace = false;
	bool fireBullet = false;
	Object* shootBullet();

	vec2 thrustPosition;
	mat4 thrustTransform;
	vec3 force = vec3(0);
	vec3 velocity;
	float direction;
	bool inSpawnArea = false;

private:

	float vX;
	float vY;
	float dragFactor;
	float maxVelocity = 1;

	bool fire;
	bool thrustOn;
	bool thrustSoundPause = true;
	bool playing = false;
	bool paused = true;
};

