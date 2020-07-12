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

	class TestHDR :public Test
	{
	public:
		TestHDR();
		~TestHDR();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		const unsigned int SCR_WIDTH = 1280, SCR_HEIGHT = 720;
		unsigned int depthMapFBO;
		unsigned int depthMap;
		unsigned int rboDepth;
		unsigned int hdrFBO;
		unsigned int colorBuffer;

		Shader m_shader;
		Shader shader;
		Shader hdr_shader;

		const std::vector<glm::vec3> lightPositions =	{ 
			glm::vec3(0.0f,  0.0f, 49.5f), 
			glm::vec3(-1.4f, -1.9f, 9.0f), 
			glm::vec3(0.0f, -1.8f, 4.0f),
			glm::vec3(0.8f, -1.7f, 6.0f)
		};
		const std::vector<glm::vec3> lightColors = { 
			glm::vec3(200.0f, 200.0f, 200.0f), 
			glm::vec3(0.1f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.2f),
			glm::vec3(0.0f, 0.1f, 0.0f)
		};

		Camera* camera;

		std::unique_ptr<Skybox> skybox;
		std::unique_ptr<Cube> alice;
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