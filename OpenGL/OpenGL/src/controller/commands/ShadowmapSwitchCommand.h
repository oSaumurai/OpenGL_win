#pragma once
#include "ICommand.h"

class ShadowmapSwitchCommand : public ICommand
{
public:
	ShadowmapSwitchCommand(bool*& state) {
		isOn = state;
	}

	void execute()
	{
		*isOn = !*isOn;
	}
private:
	bool* isOn;
};