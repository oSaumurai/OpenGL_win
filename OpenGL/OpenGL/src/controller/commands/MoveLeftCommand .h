#pragma once
#include "ICommand.h"

class MoveLeftCommand : public ICommand
{
public:
	MoveLeftCommand(Camera*& Camera) {
		camera = Camera;
	}

	void execute()
	{
		camera->MoveLeft();
	}
private:
	Camera* camera;
};