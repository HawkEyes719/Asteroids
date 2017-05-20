#pragma once

#include "GameScene.h"

class MenuSystem
{
public:
	MenuSystem();
	~MenuSystem();

	bool creditsButtonPressed = false; 
	bool backButtonPressed = false;
	bool enterKeyPressed = false;

	bool inGame = false;
	bool pauseGame = false;

	//return type (*FunctionPointerName)(parameters) 
	void(MenuSystem::*MenuToCall)(float deltaTime) = nullptr;

	void MainMenu(float deltaTime);

	void InGameScene(float deltaTime);

	void CreditsMenu();

	GameScene* newGameScene = nullptr;
};

