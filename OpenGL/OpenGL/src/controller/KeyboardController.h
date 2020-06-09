#pragma once
class KeyboardController
{

public:
	KeyboardController();
	~KeyboardController();

	void addInputCommand();
	void UpdateInput(float dt);
};

