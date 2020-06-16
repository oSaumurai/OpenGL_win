#pragma once
#include "Test.h"
#include "levelLoader/Model.h"
#include <memory>
#include <fstream>
#include <strstream>

#include "Camera/Camera.h"
#include "controller/KeyboardController.h"
#include "controller/commands/Command_Include.h"


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

		Camera* camera;
		std::unique_ptr<Model> model;

		glm::mat4 m_View, m_Proj;
		glm::vec3 m_Translation;
		float m_Rotation;

		KeyboardController* keyboardController;
		void InitController();
		void ResetPosition();
	};

}