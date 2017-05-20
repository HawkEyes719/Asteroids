#pragma once

#include "ObjectFactory.h"
#include "FModManager.h"
#include "Collision.h"
#include "Particles.h"
#include "TextRender.h"
#include "Saucer.h"
#include <thread>

class GameScene
{
public:
	GameScene();
	~GameScene();

	void update(float deltaTime);
	void draw();

	void reset();
	void createNewAsteroids();

	void vectorObjManagement(float deltaTime);
	void collisionManagement();

	void breakAsteroids();
	void emitParticle();

	vector<Object*> vector_asteroids;
	vector<Object*> vector_bullets;
	vector<Object*> vector_enemy_bullets;
	vector<ParticleEmitter*> vector_emitter;
	Player* playerObj;
	Saucer* saucerObj;
	Object spawnZone;

	float spawnTimer = 0;
	float spawnTime = linearRand(5.0f, 10.0f);
	float spawnAsteroidsTimer = 0;
	float spawnAsteroidsTime = 1;
	int gameScore = 0;
	int scoreCounter = 0;
	int round = 0;
	bool isShooting = false;

	// for collision
	mat4 collidedObjTrans = mat4(1);
	vec2 collidedObjPos = vec2(0);
	string collidedObjName;
	float asteroidSpeed = 1;
	bool breakAsteroidsApart = false;
	bool collisionFromSaucer = false;
	bool gameOver = true;
	bool spawnSaucer = false;
	bool dontAddScore = false;

	FMOD_RESULT result;
	FMOD::Sound *sound;
	FMOD::Channel *channel;

	FMOD::Sound *sound2;
	FMOD::Channel *channel2;

private:

};

