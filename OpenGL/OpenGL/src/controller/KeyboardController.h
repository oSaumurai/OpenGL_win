#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Controller.h"
#include "commands/ICommand.h"


class KeyboardController: public Controller
{
public:
	~KeyboardController();
	static KeyboardController* getInstance();

	//void AttachWindowInput(GLFWwindow* window);
	void RegisterCommand(int key, ICommand* command);
	void UnRegisterCommand();
	void updateInput() override;

private:
	std::unordered_set<int> keys;
	std::unordered_map<int, ICommand*> commands;
	static KeyboardController* instance;

private:
	KeyboardController();
	void ProcessInput();
};


