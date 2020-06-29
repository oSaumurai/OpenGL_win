#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	UnLoad();
}

void ObjectManager::AddObject(AbstractObject*& object)
{
	cubeObjects.emplace_back(object);
}

void ObjectManager::Load()
{
	skybox = new Skybox("res/object/skybox");
	grass = new Grass();
	alice_cube = new Cube();
}

void ObjectManager::UnLoad()
{
	delete skybox;
	delete grass;
	delete alice_cube;
}

void ObjectManager::Update()
{
}

void ObjectManager::Draw()
{
	//skybox->Draw();
	//grass->Draw();
	//alice_cube->Draw();
}
