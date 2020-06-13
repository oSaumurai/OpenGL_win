#pragma once
#include "ICommand.h"

class MoveUpCommand : public ICommand
{
public:
	MoveUpCommand(Camera* Camera) {
		camera = Camera;
	}

	void execute()
	{
		camera->MoveUp();
	}
private:
	Camera* camera;
};