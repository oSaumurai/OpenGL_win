#pragma once
#include "Test.h"
#include "Camera/Camera.h"
#include "controller/KeyboardController.h"
#include "controller/commands/Command_Include.h"
#include "gameOjbects/Skybox/Skybox.h"
#include "Texture.h"

#include "levelLoader/Model.h"
#include "window/Window.h"

namespace test {
	class PBR_Test: public Test {
	public:
		PBR_Test();
		~PBR_Test();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		//Camera camera;
		KeyboardController* keyboardController;
		Camera* camera;

		Shader skybox_shader{ "res/shader/skybox.shader" };
		Skybox skybox{ "res/object/skybox/hdri", true};

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;

		float viewAngle = 60.0f;

		glm::mat4 m_View{ glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6.0f)) }, m_Proj{ glm::perspective(glm::radians(60.f), 1.0f, 0.1f, 100.0f) };
		glm::vec3 m_Translation;
		glm::vec3 m_Rotation;
		glm::vec3 light_Pos;

	private:
		void InitController();
		void UnRegisterController();
	};
}