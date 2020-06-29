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

		Shader m_shader;
		Shader skybox_shader;
		Shader Grass_shader;
		Shader debug_quad;
		Shader DepthShader;
		Shader shadowMap_shader;

		Camera* camera;
		//GLFWwindow*& window;
		std::unique_ptr<Skybox> skybox;
		std::unique_ptr<Model> model;
		std::unique_ptr<Cube> alice;
		//std::unique_ptr<Grass> grass;
		std::unique_ptr<Plane> plane;
		std::unique_ptr<Quad_NDC> quad_NDC;

		float viewAngle = 60.0f;
		glm::mat4 m_View, m_Proj;
		glm::vec3 m_Translation;
		glm::vec3 m_Rotation;
		glm::vec3 light_Pos;

		KeyboardController* keyboardController;
		void RenderScene(Shader& shader);
		void ResetPosition();
		void InitController();
		void UnRegisterController();
	};

}