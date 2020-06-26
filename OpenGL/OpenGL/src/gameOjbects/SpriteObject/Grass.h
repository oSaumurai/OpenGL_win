#pragma once
#include "Quad.h"

class Grass :public Quad
{
public:
	Grass() { BindTexture("res/textures/grass.png"); };
	~Grass() {};

	void Update() override {};
};