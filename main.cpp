#include "TextRender.h"
#include "MenuSystem.h"

#include <iostream>

int main()
{
	srand(time(NULL));


	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "Asteroids", nullptr, nullptr); // Windowed
	//GLFWwindow* window =
	//	glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); // Fullscreen

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	//mat4 m_projection = glm::perspective(glm::pi<float>() * 0.25f, 4.f / 3.f, .1f, 100.0f);
	mat4 m_projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 100.0f);
	//mat4 m_projection = glm::ortho(static_cast<GLfloat>(-SCREENWIDTH), static_cast<GLfloat>(SCREENWIDTH), static_cast<GLfloat>(-SCREENHEIGHT), static_cast<GLfloat>(SCREENHEIGHT), 0.0f, 100.0f);
	mat4 m_view = glm::lookAt(vec3(0, 0, 10), vec3(0), vec3(0, 1, 0));

	MenuSystem* menu = new MenuSystem();
	menu->MenuToCall = &MenuSystem::MainMenu; //MenuToCall = MainMenu also works 

	Gizmos::create(0xffff);

	TextRender::create(SCREENWIDTH, SCREENHEIGHT);
	TextRender::load("fonts/AtariST8x16SystemFont.ttf", 16);

	float alpha = 1.0f;
	bool forward = false;

	double lastFrame = glfwGetTime();

	// background sound
	//FMOD_RESULT result;
	//FMOD::Sound *sound;
	//FMOD::Channel *channel;
	//result = FModManager::System()->createSound("audio/background.wav", FMOD_LOOP_NORMAL, 0, &sound);
	//FModManager::System()->playSound(sound, 0, false, &channel);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// delta time setup
		double currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame; // prev of last frame
		lastFrame = currentFrame;

		// my update stuff
		Gizmos::clear();

		//Invoke the function pointer - initially calls the main menu         
		(menu->*(menu->MenuToCall))(deltaTime);

		//--------------------------------------------------------------------

		Gizmos::draw2D(m_projection * m_view);
		FModManager::System()->update();

		glfwSwapBuffers(window);

		// Close window
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
			menu->enterKeyPressed = true;

		//if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && menu->inGame && !menu->pauseGame)
		//	menu->pauseGame = true;
		//else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && menu->pauseGame)
		//	menu->pauseGame = false;
	}

	FModManager::System()->close();
	FModManager::System()->release();

	Gizmos::destroy();

	glfwTerminate();
}