#include "GameScene.h"

#include "Player.h"

void GameScene::createNewAsteroids()
{
	int numOfAsteroids;
	if (round > 5)
		numOfAsteroids = 6;
	else
		numOfAsteroids = linearRand(4, 6);

	for (int i = 0; i < numOfAsteroids; ++i)
	{
		Object* asteroidObj = ObjectFactory::CreateObject("LargeAsteroid");
		vector_asteroids.push_back(asteroidObj);
	}
}

GameScene::GameScene()
{
	result = FModManager::System()->createSound("audio/bangSmall.wav", FMOD_DEFAULT, 0, &sound);
	result = FModManager::System()->createSound("audio/extraShip.wav", FMOD_DEFAULT, 0, &sound2);
	playerObj = new Player();
	playerObj->setTransform(mat4(1));

	spawnZone.setPosition(vec2(0));
	spawnZone.setSize(vec2(0.75f));
}


GameScene::~GameScene()
{
	sound->release();
	sound2->release();

	delete playerObj;
	delete saucerObj;
}

void GameScene::update(float deltaTime)
{
	//TextRender::render(std::to_string(degrees(playerObj->direction)), SCREENWIDTH / 2, SCREENHEIGHT - 20.0f, 1.0f, vec3(1));
	//TextRender::render(std::to_string(radians(playerObj->direction)), SCREENWIDTH / 2, SCREENHEIGHT - 40.0f, 1.0f, vec3(1));
	//TextRender::render(std::to_string(playerObj->direction), SCREENWIDTH / 2, SCREENHEIGHT - 60.0f, 1.0f, vec3(1));
	//TextRender::render(std::to_string(degrees(playerObj->getPosition().x)) + ", " + std::to_string(degrees(playerObj->getPosition().y)), 
	//					20 , SCREENHEIGHT - 60.0f, 1.0f, vec3(1));
	//TextRender::render(std::to_string(degrees(playerObj->velocity .x)) + ", " + std::to_string(degrees(playerObj->velocity.y)),
	//	SCREENWIDTH / 2, SCREENHEIGHT - 40.0f, 1.0f, vec3(1));
	//TextRender::render(std::to_string(playerObj->speed), SCREENWIDTH / 2, SCREENHEIGHT - 40.0f, 1.0f, vec3(1));

	if (!gameOver)
	{
		//--------------------------------------------------------------------
		// Objects update
		if (playerObj != nullptr && playerObj->inSpawnArea == false)
			playerObj->update(deltaTime);

		// saucer spawn handling
		if(saucerObj == nullptr)
			spawnTimer += 0.5f * deltaTime;
		if (spawnTimer >= spawnTime && saucerObj == nullptr)
		{
			saucerObj = new Saucer();
			spawnTimer = 0;
			spawnTime = linearRand(5.0f, 10.0f);
		}

		if (saucerObj != nullptr)
		{
			saucerObj->update(deltaTime);
		
			if (saucerObj->getAlive() == false)
				saucerObj = nullptr;
		}

		//--------------------------------------------------------------------
		// bullet shooting from player
		Object* bulletObj = nullptr;
		if (vector_bullets.size() < 4 && playerObj != nullptr)
			bulletObj = playerObj->shootBullet();
		if (bulletObj != nullptr)
			vector_bullets.push_back(bulletObj);

		//--------------------------------------------------------------------
		// bullet shooting from saucer
		Object* bulletObj2 = nullptr;
		if(saucerObj != nullptr)
			bulletObj2 = saucerObj->shootBullet(deltaTime, playerObj->getPosition(), playerObj->getAlive());
		if (bulletObj2 != nullptr)
			vector_enemy_bullets.push_back(bulletObj2);

		TextRender::render(std::to_string(gameScore), 100.0f, SCREENHEIGHT - 20.0f, 1.0f, vec3(1));
		TextRender::render(std::to_string(round), 400.0f, SCREENHEIGHT - 20.0f, 1.0f, vec3(1));

		if (playerObj->lives == 0)
			gameOver = true;

		if (scoreCounter >= 10000)
		{
			scoreCounter = 0;
			playerObj->lives++;
			FModManager::System()->playSound(sound2, 0, false, &channel2);
		}

		if (vector_asteroids.size() == 0 && saucerObj == nullptr)
		{
			spawnAsteroidsTimer += 0.5f * deltaTime;
			if (spawnAsteroidsTimer >= spawnAsteroidsTime)
			{
				round++;
				if (round > 1)
					asteroidSpeed += 0.5f;

				createNewAsteroids();
				spawnAsteroidsTimer = 0;
			}
		}
	}
	else
	{
		if (saucerObj != nullptr)
			saucerObj->result = saucerObj->channel->setPaused(true);
		if (playerObj != nullptr)
			playerObj->result = playerObj->channel2->setPaused(true);

		saucerObj = nullptr;
	
		TextRender::render("Game Over", SCREENWIDTH / 2 - 35, SCREENHEIGHT / 2, 1.0f, vec3(1));
	
		if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_PRESS)
		{
			reset();
		}
	}

	vectorObjManagement(deltaTime);
	collisionManagement();
}

void GameScene::reset()
{
	saucerObj = nullptr;
	playerObj = nullptr;

	gameOver = false;
	gameScore = 0;
	scoreCounter = 0;
	round = 0;
	asteroidSpeed = 1;
	spawnTimer = 0;
	
	playerObj = new Player();
	playerObj->setPosition(vec2(0));
	playerObj->lives = 3;
	playerObj->setTransform(mat4(1));
	
	vector_asteroids.clear();
	vector_bullets.clear();
	vector_enemy_bullets.clear();
	vector_emitter.clear();
}

void GameScene::draw()
{
	//--------------------------------------------------------------------
	// Objects draw
	if(playerObj != nullptr)
		playerObj->draw();

	if (saucerObj != nullptr)
		saucerObj->draw();

	// draws spawnZone
	//Gizmos::add2DAABB(spawnZone.getPosition(), spawnZone.getSize() , vec4(1));
}

void GameScene::vectorObjManagement(float deltaTime)
{
	//--------------------------------------------------------------------
	// Asteroids
	vector<Object*>::iterator itasteroids = vector_asteroids.begin();
	while (itasteroids != vector_asteroids.end())
	{
		(*itasteroids)->speed = asteroidSpeed;

		(*itasteroids)->update(deltaTime);
		(*itasteroids)->draw();

		if ((*itasteroids)->getAlive() == false)
		{
			delete (*itasteroids);
			itasteroids = vector_bullets.erase(itasteroids);
		}
		else
		{
			++itasteroids;
		}
	}

	//--------------------------------------------------------------------
	// Bullets
	vector<Object*>::iterator itbullet = vector_bullets.begin();
	while (itbullet != vector_bullets.end())
	{
		(*itbullet)->update(deltaTime);
		(*itbullet)->draw();

		if ((*itbullet)->getAlive() == false)
		{
			delete (*itbullet);
			itbullet = vector_bullets.erase(itbullet);
		}
		else
		{
			++itbullet;
		}
	}

	vector<Object*>::iterator itenemybullet = vector_enemy_bullets.begin();
	while (itenemybullet != vector_enemy_bullets.end())
	{
		(*itenemybullet)->update(deltaTime);
		(*itenemybullet)->draw();

		if ((*itenemybullet)->getAlive() == false)
		{
			delete (*itenemybullet);
			itenemybullet = vector_enemy_bullets.erase(itenemybullet);
		}
		else
		{
			++itenemybullet;
		}
	}

	//--------------------------------------------------------------------
	// Particle Emitters
	mat4 m_projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 100.0f);
	mat4 m_view = glm::lookAt(vec3(0, 0, 10), vec3(0), vec3(0, 1, 0));

	vector<ParticleEmitter*>::iterator itemitter = vector_emitter.begin();
	while (itemitter != vector_emitter.end())
	{
		(*itemitter)->update(deltaTime, m_projection);
		(*itemitter)->draw(m_projection);

		if ((*itemitter)->allDead == true)
		{
			delete (*itemitter);
			itemitter = vector_emitter.erase(itemitter);
		}
		else
		{
			++itemitter;
		}
	}
}

void GameScene::collisionManagement()
{
	//--------------------------------------------------------------------
	// collision checks

	// iterates through a vector of asteroids
	vector<Object*>::iterator itasteroids = vector_asteroids.begin();
	while (itasteroids != vector_asteroids.end())
	{
		// to check if asteroids are too close to player spawn
		if (playerObj != nullptr)
			playerObj->inSpawnArea = Collision::AABB(spawnZone.getMin(), spawnZone.getMax(), (*itasteroids)->getMin(), (*itasteroids)->getMax());

		//cout << playerObj->inSpawnArea << endl;

		//--------------------------------------------------------------------
		// For Bullets
		// iterates through a vector of bullets
		vector<Object*>::iterator itbullets = vector_bullets.begin();
		while (itbullets != vector_bullets.end())
		{
			// checks if both objects are alive
			if ((*itasteroids) != nullptr && (*itbullets) != nullptr &&
				(*itasteroids)->getAlive() == true && (*itbullets)->getAlive() == true)
			{
				// checks collisions
				bool collided = Collision::AABB((*itbullets)->getMin(), (*itbullets)->getMax(),
					(*itasteroids)->getMin(), (*itasteroids)->getMax());
				if (collided)
				{
					(*itasteroids)->setAlive(false);
					(*itbullets)->setAlive(false);

					// gets the collided asteroid data
					collidedObjName = (*itasteroids)->getName();
					collidedObjPos = (*itasteroids)->getPosition();
					collidedObjTrans = (*itasteroids)->getTransform();

					// set it to break apart and add score
					if ((*itasteroids)->getName() == "LargeAsteroid" || (*itasteroids)->getName() == "MediumAsteroid")
						breakAsteroidsApart = true;
					else // small asteroids are destroyed and don't break apart
					{
						gameScore += 100;
						scoreCounter += 100;
					}

					// emit particles when asteroids get destroyed
					emitParticle();
				}
			}

			// checks if bullet is dead
			if ((*itbullets)->getAlive() == false)
			{
				delete (*itbullets);
				itbullets = vector_bullets.erase(itbullets);
			}
			else
			{
				++itbullets;
			}
		}

		// enemy bullets
		vector<Object*>::iterator itenemybullets = vector_enemy_bullets.begin();
		while (itenemybullets != vector_enemy_bullets.end())
		{
			// checks if both objects are alive
			if ((*itasteroids) != nullptr && (*itenemybullets) != nullptr &&
				(*itasteroids)->getAlive() == true && (*itenemybullets)->getAlive() == true)
			{
				// checks collisions
				bool collided = Collision::AABB((*itenemybullets)->getMin(), (*itenemybullets)->getMax(),
					(*itasteroids)->getMin(), (*itasteroids)->getMax());
				if (collided)
				{
					(*itasteroids)->setAlive(false);
					(*itenemybullets)->setAlive(false);

					// gets the collided asteroid data
					collidedObjName = (*itasteroids)->getName();
					collidedObjPos = (*itasteroids)->getPosition();
					collidedObjTrans = (*itasteroids)->getTransform();

					// set it to break apart and add score
					if ((*itasteroids)->getName() == "LargeAsteroid" || (*itasteroids)->getName() == "MediumAsteroid")
						breakAsteroidsApart = true;

					// emit particles when asteroids get destroyed
					emitParticle();
				}
			}

			// checks if bullet is dead
			if ((*itenemybullets)->getAlive() == false)
			{
				delete (*itenemybullets);
				itenemybullets = vector_enemy_bullets.erase(itenemybullets);
			}
			else
			{
				++itenemybullets;
			}
		}

		//--------------------------------------------------------------------
		// For Player
		// checks if both objects are alive
		if ((*itasteroids) != nullptr && playerObj != nullptr &&
			(*itasteroids)->getAlive() == true && playerObj->getAlive() == true)
		{
			// checks collisions
			bool collided = Collision::AABB(playerObj->getMin(), playerObj->getMax(),
				(*itasteroids)->getMin(), (*itasteroids)->getMax());
			if (collided)
			{
				// set player alive to false
				playerObj->setAlive(false);
				(*itasteroids)->setAlive(false);

				// decrease player life
				if (playerObj->lives != 0)
					playerObj->lives -= 1;

				// gets the collided playerObj data
				collidedObjPos = playerObj->getPosition();
				collidedObjTrans = playerObj->getTransform();

				// set it to break apart and add score
				if ((*itasteroids)->getName() == "LargeAsteroid" || (*itasteroids)->getName() == "MediumAsteroid")
					breakAsteroidsApart = true;
				
				// emit particles when asteroids get destroyed
				emitParticle();

				// gets the collided asteroidsObj data
				collidedObjName = (*itasteroids)->getName();
				collidedObjPos = (*itasteroids)->getPosition();
				collidedObjTrans = (*itasteroids)->getTransform();

				// emit particles when asteroids get destroyed
				emitParticle();
			}
		}

		//--------------------------------------------------------------------
		// For Saucer
		// checks if both objects are alive
		if ((*itasteroids) != nullptr && saucerObj != nullptr &&
			(*itasteroids)->getAlive() == true && saucerObj->getAlive() == true)
		{
			// checks collisions
			bool collided = Collision::AABB(saucerObj->getMin(), saucerObj->getMax(),
				(*itasteroids)->getMin(), (*itasteroids)->getMax());
			if (collided)
			{
				dontAddScore = true;

				// set player alive to false
				saucerObj->setAlive(false);
				saucerObj->result = saucerObj->channel->setPaused(true);
				(*itasteroids)->setAlive(false);

				// gets the collided saucerObj data
				collidedObjPos = saucerObj->getPosition();
				collidedObjTrans = saucerObj->getTransform();

				// set it to break apart depending on asteroid size and add score
				if ((*itasteroids)->getName() == "LargeAsteroid" || (*itasteroids)->getName() == "MediumAsteroid")
					breakAsteroidsApart = true;

				// emit particles when saucers get destroyed
				emitParticle();

				// changes these variables with collided asteroidsObj data
				collidedObjName = (*itasteroids)->getName();
				collidedObjPos = (*itasteroids)->getPosition();
				collidedObjTrans = (*itasteroids)->getTransform();

				// emit particles when asteroids get destroyed
				emitParticle();

				saucerObj = nullptr;
			}
		}

		// checks if asteroid is dead
		if ((*itasteroids)->getAlive() == false)
		{
			FModManager::System()->playSound(sound, 0, false, &channel);
			delete (*itasteroids);
			itasteroids = vector_asteroids.erase(itasteroids);
		}
		else
		{
			++itasteroids;
		}
	}

	// player bullets to saucer
	vector<Object*>::iterator itbullets = vector_bullets.begin();
	while (itbullets != vector_bullets.end())
	{
		if (saucerObj != nullptr)
		{
			bool collidedWithSaucer = Collision::AABB(saucerObj->getMin(), saucerObj->getMax(),
				(*itbullets)->getMin(), (*itbullets)->getMax());

			if (collidedWithSaucer)
			{
				// gets the collided asteroid data
				collidedObjName = saucerObj->getName();
				collidedObjPos = saucerObj->getPosition();
				collidedObjTrans = saucerObj->getTransform();

				// emit particles when asteroids get destroyed
				emitParticle();

				if (saucerObj->getName() == "LargeSaucer")
				{
					gameScore += 200;
					scoreCounter += 200;
				}
				else if (saucerObj->getName() == "SmallSaucer")
				{
					gameScore += 1000;
					scoreCounter += 1000;
				}

				(*itbullets)->setAlive(false);
				saucerObj->setAlive(false);
				saucerObj->result = saucerObj->channel->setPaused(true);
				saucerObj = nullptr;
			}
		}
		// checks if bullet is dead
		if ((*itbullets)->getAlive() == false)
		{
			delete (*itbullets);
			itbullets = vector_bullets.erase(itbullets);
		}
		else
		{
			++itbullets;
		}
	}

	// enemy bullets to player
	vector<Object*>::iterator itenemybullets = vector_enemy_bullets.begin();
	while (itenemybullets != vector_enemy_bullets.end())
	{
		if (playerObj != nullptr)
		{
			// checks collisions
			bool collidedWithPlayer = Collision::AABB((*itenemybullets)->getMin(), (*itenemybullets)->getMax(),
				playerObj->getMin(), playerObj->getMax());

			if (collidedWithPlayer)
			{
				(*itenemybullets)->setAlive(false);
				playerObj->setAlive(false);
				playerObj->result = playerObj->channel2->setPaused(true);

				// decrease player life
				if (playerObj->lives != 0)
					playerObj->lives -= 1;

				// gets the collided asteroid data
				collidedObjPos = playerObj->getPosition();
				collidedObjTrans = playerObj->getTransform();

				// emit particles when asteroids get destroyed
				emitParticle();
			}
		}

		// checks if bullet is dead
		if ((*itenemybullets)->getAlive() == false)
		{
			delete (*itenemybullets);
			itenemybullets = vector_enemy_bullets.erase(itenemybullets);
		}
		else
		{
			++itenemybullets;
		}
	}

	// handles collision between player and saucer
	if (playerObj != nullptr && saucerObj != nullptr)
	{
		//playerObj->inSpawnArea = Collision::AABB(spawnZone.getMin(), spawnZone.getMax(), saucerObj->getMin(), saucerObj->getMax());

		// checks collisions
		bool PlayerCollidedWithSaucer = Collision::AABB(saucerObj->getMin(), saucerObj->getMax(),
														playerObj->getMin(), playerObj->getMax());

		if (PlayerCollidedWithSaucer && playerObj->getAlive() == true)
		{
			// gets the collided asteroid data
			collidedObjPos = playerObj->getPosition();
			collidedObjTrans = playerObj->getTransform();

			playerObj->setAlive(false);
			playerObj->result = playerObj->channel2->setPaused(true);

			// decrease player life
			if (playerObj->lives != 0)
				playerObj->lives -= 1;

			emitParticle();
		}
	}

	if(breakAsteroidsApart)
		breakAsteroids();
}

void GameScene::emitParticle()
{
	// emit particles when asteroids get destroyed
	ParticleEmitter* m_emitter = new ParticleEmitter();
	int randNumber = linearRand(5, 6);
	m_emitter->initalise(randNumber, 50,
		0.5f, 1.0f,
		1, 2,
		0.1f, 0.1f,
		glm::vec4(1), glm::vec4(1));
	m_emitter->m_position = vec3(collidedObjPos, 0);
	vector_emitter.push_back(m_emitter);


	// plays when player dies
	FModManager::System()->playSound(sound, 0, false, &channel);
}

void GameScene::breakAsteroids()
{
	//--------------------------------------------------------------------
	// breaking apart asteroids
	if (breakAsteroidsApart && collidedObjName == "LargeAsteroid")
	{
		if (!dontAddScore)
		{
			gameScore += 20;
			scoreCounter += 20;
		}

		Object* astObj = ObjectFactory::CreateObject("MediumAsteroid");
		astObj->setTransform(collidedObjTrans * glm::rotate(collidedObjTrans, radians(-90.0f), vec3(0.0f, 0.0f, 1.0f)));
		astObj->setPosition(collidedObjPos);
		vector_asteroids.push_back(astObj);

		Object* astObj2 = ObjectFactory::CreateObject("MediumAsteroid");
		astObj2->setTransform(collidedObjTrans * glm::rotate(collidedObjTrans, radians(90.0f), vec3(0.0f, 0.0f, 1.0f)));
		astObj2->setPosition(collidedObjPos);
		vector_asteroids.push_back(astObj2);

		astObj = nullptr;
		astObj2 = nullptr;

		breakAsteroidsApart = false;
	}
	else if (breakAsteroidsApart && collidedObjName == "MediumAsteroid")
	{
		if (!dontAddScore)
		{
			gameScore += 50;
			scoreCounter += 50;
		}

		Object* astObj = ObjectFactory::CreateObject("SmallAsteroid");
		astObj->setTransform(collidedObjTrans * glm::rotate(collidedObjTrans, radians(-90.0f), vec3(0.0f, 0.0f, 1.0f)));
		astObj->setPosition(collidedObjPos);
		vector_asteroids.push_back(astObj);

		Object* astObj2 = ObjectFactory::CreateObject("SmallAsteroid");
		astObj2->setTransform(collidedObjTrans * glm::rotate(collidedObjTrans, radians(90.0f), vec3(0.0f, 0.0f, 1.0f)));
		astObj2->setPosition(collidedObjPos);
		vector_asteroids.push_back(astObj2);

		astObj = nullptr;
		astObj2 = nullptr;

		breakAsteroidsApart = false;
	}

	if (dontAddScore == true)
		dontAddScore = false;
}