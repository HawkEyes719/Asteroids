#pragma once

#include "Object.h"

class Asteroids : public Object
{
public:
	Asteroids();
	~Asteroids();

	void update(float deltaTime);

	void draw();

protected:
	
	int score;
};

class LargeAsteroid : public Asteroids
{
public:
	LargeAsteroid();
	~LargeAsteroid();

private:
	
};

class MediumAsteroid : public Asteroids
{
public:
	MediumAsteroid();
	~MediumAsteroid();

private:

};

class SmallAsteroid : public Asteroids
{
public:
	SmallAsteroid();
	~SmallAsteroid();

private:

};