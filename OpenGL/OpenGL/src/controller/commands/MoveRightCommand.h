#pragma once
#include "ICommand.h"

class MoveRightCommand : public ICommand
{
public:
	MoveRightCommand(Camera* Camera) {
		camera = Camera;
	}

	void execute()
	{
		camera->MoveRight();
	}
private:
	Camera* camera;
};