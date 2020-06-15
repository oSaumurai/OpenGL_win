#pragma once
#include "Test.h"
#include "Model.h"
#include <memory>
#include <fstream>
#include <strstream>

#include "Camera/Camera.h"
#include "controller/Mouse_Controller.h"
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
        //Camera camera;
        MouseController* mouseController;
        KeyboardController* keyboardController;
        Camera* camera;
		//Model model;
		std::unique_ptr<Model> model;

		glm::mat4 m_View, m_Proj;
		glm::vec3 m_Translation;
	};
}