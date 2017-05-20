#include "MenuSystem.h"
#include "TextRender.h"

#include <iostream>

MenuSystem::MenuSystem()
{
	//MenuToCall = &MainMenu; //MenuToCall = MainMenu also works 
	//GameScene = false;
}


MenuSystem::~MenuSystem()
{
}

void MenuSystem::MainMenu(float deltaTime)
{
	//std::cout << "main menu" << std::endl;
	TextRender::render("Asteroids", SCREENWIDTH / 2, SCREENHEIGHT / 2, 1.0f, vec3(1));
	TextRender::render("Press Enter to Play", 0.0f, 0.0f, 1.0f, vec3(1));
	if (enterKeyPressed)
	{
		MenuToCall = &MenuSystem::InGameScene;
	}
}

void MenuSystem::InGameScene(float deltaTime)
{
	//std::cout << "game scene" << std::endl;

	if (!inGame)
	{
		newGameScene = new GameScene();
		inGame = true;
	}
		
	if (newGameScene != nullptr)
	{
		if(!pauseGame)
			newGameScene->update(deltaTime);

		newGameScene->draw();
	}
}

void MenuSystem::CreditsMenu()
{
	std::cout << "credits menu" << std::endl;
	if (backButtonPressed)
	{
		//MenuToCall = &MenuSystem::MainMenu;
	}
}