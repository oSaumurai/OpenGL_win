#pragma once
#include "ICommand.h"

class MoveBackCommand : public ICommand
{
public:
	MoveBackCommand(Camera* Camera) {
		camera = Camera;
	}
	
	void execute()
	{
		camera->MoveBack();
	}
private:
	Camera* camera;
};