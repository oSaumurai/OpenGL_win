#pragma once
#include "ICommand.h"

class MoveDownCommand: public ICommand
{
public:
	MoveDownCommand(Camera* Camera) {
		camera = Camera;
	}

	void execute()
	{
		camera->MoveDown();
	}
private:
	Camera* camera;
};