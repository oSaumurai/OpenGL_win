#pragma once
#include "Camera/Camera.h"
class ICommand 
{
public: 
	virtual void execute() = 0;
};