#pragma once
#include "Test.h"
#include <memory>
#include <fstream>
#include <strstream>

#include "Camera/Camera.h"
#include "controller/KeyboardController.h"
#include "controller/commands/Command_Include.h"

#include "gameOjbects/Skybox/Skybox.h"
#include "gameOjbects/BlockObject/Cube.h"
#include "gameOjbects/BlockObject/Sphere.h"
#include "gameOjbects/SpriteObject/Grass.h"
#include "gameOjbects/BlockObject/Plane.h"
#include "gameOjbects/SpriteObject/Quad_NDC.h"
#include "levelLoader/Model.h"

#include "window/Window.h"
namespace test {

	class TestLoader :public Test
	{
	public:
		TestLoader();
		~TestLoader();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		unsigned int depthMapFBO;
		unsigned int depthMap;

		Shader m_shader{ "res/shader/Assimp.shader" };
		Shader skybox_shader{ "res/shader/skybox.shader" };
		Shader Grass_shader{ "res/shader/Grass.shader" };
		Shader debug_quad{"res/shader/debug_quad.shader"};
		Shader DepthShader{ "res/shader/shadowmap_depth.shader" };
		Shader shadowMap_shader{ "res/shader/shadowmap.shader" };
		//Shader PBR_shader{ "res/shader/PBR.shader " };

		//Texture albedo   { "res/object/PBR/albedo.png" };
		//Texture normal   { "res/object/PBR/normal.png" };
		//Texture metallic { "res/object/PBR/metal.png" };
		//Texture roughness{ "res/object/PBR/roughness.png" };
		//Texture ao       { "res/object/PBR/ao.png" };

		Camera* camera;
		//GLFWwindow*& window;
		//std::unique_ptr<Grass> grass;
		std::unique_ptr<Skybox> skybox;
		std::unique_ptr<Model>  kouhai;
		std::unique_ptr<Cube>   alice;
		std::unique_ptr<Sphere> sphere;
		std::unique_ptr<Plane> plane;
		std::unique_ptr<Quad_NDC> quad_NDC;

		float viewAngle = 60.0f;
		glm::mat4 m_View{ glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6.0f)) };
		glm::mat4 m_Proj{ glm::perspective(glm::radians(viewAngle), 1280.0f / 720.0f, 0.1f, 100.0f) };
		glm::vec3 m_Translation;
		glm::vec3 m_Rotation;
		glm::vec3 light_Pos{ glm::vec3(2.0, 9.0, 4.0) };

		KeyboardController* keyboardController;
		void RenderScene(Shader& shader);
		void ResetPosition();
		void InitController();
		void UnRegisterController();
	};
}