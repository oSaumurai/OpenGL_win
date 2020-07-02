#pragma once
#include "ICommand.h"
class MoveForwardCommand : public ICommand
{
public:
	MoveForwardCommand(Camera*& Camera) {
		camera = Camera;
	}

	void execute()
	{
		camera->MoveForward();
	}
private:
	Camera* camera;

};