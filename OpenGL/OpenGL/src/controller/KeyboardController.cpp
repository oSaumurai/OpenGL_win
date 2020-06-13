#include "KeyboardController.h"
KeyboardController* KeyboardController::instance = NULL;

KeyboardController* KeyboardController::getInstance()
{
	if (!instance) {
		instance = new KeyboardController();
	}
	return instance;
}

KeyboardController::KeyboardController()
{
}

void KeyboardController::ProcessInput()
{
}

KeyboardController::~KeyboardController()
{
	for (auto it : commands) 
	{
		delete it.second;
	}
}


void KeyboardController::RegisterCommand(int key, ICommand* command)
{
	if (keys.find(key) == keys.end())
	{
		keys.insert(key);
		commands[key] = command;
	}
}

void KeyboardController::updateInput()
{
	for (int key : keys) 
	{
		if (glfwGetKey(window, key) == GLFW_PRESS)
		{
			commands[key]->execute();
		}
	}
}
