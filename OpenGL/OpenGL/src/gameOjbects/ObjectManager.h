#pragma once
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Skybox/Skybox.h"
#include "SpriteObject/Grass.h"
#include "BlockObject/Cube.h"
#include "levelLoader/Model.h"

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	std::vector<AbstractObject*> cubeObjects;
	Skybox* skybox;
	Grass* grass;
	Cube* alice_cube;
	
	void AddObject(AbstractObject*&);
	void Load();
	void UnLoad();
	void Update();
	void Draw();
private:
	float viewAngle = 60.0f;
	glm::mat4 m_View, m_Proj;
	glm::vec3 m_Translation;
	glm::vec3 m_Rotation;

};

