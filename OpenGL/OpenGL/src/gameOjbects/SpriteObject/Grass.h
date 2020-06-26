#pragma once
#include "Quad.h"

class Grass :public Quad
{
public:
	Grass();
	~Grass();

	void Update() override;
private:

};