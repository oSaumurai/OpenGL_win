#pragma once
#include "Test.h"
#include "levelLoader/Model.h"
#include <memory>
#include <fstream>
#include <strstream>

#include "Camera/Camera.h"
#include "controller/KeyboardController.h"
#include "controller/commands/Command_Include.h"
#include "renderOjbects/Skybox.h"
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
		Shader m_shader;
		Shader skybox_shader;

		Camera* camera;
		//GLFWwindow*& window;
		std::unique_ptr<Skybox> skybox;
		std::unique_ptr<Model> model;

		float viewAngle = 45.0f;
		glm::mat4 m_View, m_Proj;
		glm::vec3 m_Translation;
		glm::vec3 m_Rotation;

		KeyboardController* keyboardController;
		void ResetPosition();
		void InitController();
		void UnRegisterController();
	};

}