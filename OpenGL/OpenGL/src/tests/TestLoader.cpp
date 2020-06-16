#include "TestLoader.h"
#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
namespace test {

    TestLoader::TestLoader()
        :m_Translation(0, 0, 0), m_Rotation(0.0),
        m_Proj(glm::perspective(90.0f, 1.0f, 0.1f, 100.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6.0f))),
        m_shader("res/shader/Assimp.shader")
	{        
        camera = new Camera(glm::vec3(0.0f, 0.0f, 2.0f));
        model = std::make_unique<Model>("res/object/backpack/backpack.obj");
        keyboardController = KeyboardController::getInstance();
        InitController();
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));
        GLCall(glEnable(GL_DEPTH_TEST));
	}

    TestLoader::~TestLoader()
	{
        delete camera;
    }

	void TestLoader::OnUpdate(float deltaTime)
	{
        camera->updateCameraVectors();
        m_View = camera->GetViewMartix();
        glm::mat4 model = glm::mat4(1.0f);
        //Translation, Rotation , Scale
        model = glm::translate(model, m_Translation);
        model = glm::rotate(model, m_Rotation , glm::vec3(0.0, 1.0, 0.0));

        glm::mat4 mvp = m_Proj * m_View * model;
        m_shader.Bind();
        m_shader.SetUniformMat4f("u_MVP", mvp);
	}
	void TestLoader::OnRender()
	{
        model->Draw(m_shader);
	}
	void TestLoader::OnImGuiRender()
	{
        // Create a window called "Hello, world!" and append into it.
        ImGui::Begin("Hello, world!");                          
        ImGui::SliderFloat3("Translation x y z", &m_Translation.x, -10.0f, 10.0f);
        ImGui::SliderAngle("Rotation on y", &m_Rotation);
        if(ImGui::Button("Reset Position"))
            ResetPosition();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
	}

    void TestLoader::ResetPosition()
    {
        m_Translation = glm::vec3(0, 0, 0);
        m_Rotation = 0.0f;
        m_Proj = glm::perspective(90.0f, 1.0f, 0.1f, 100.0f);
        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6.0f));
        camera->ResetPosition();
    }

    void TestLoader::InitController()
    {
        keyboardController->RegisterCommand(GLFW_KEY_W, new MoveForwardCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_S, new MoveBackCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_A, new MoveLeftCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_D, new MoveRightCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_SPACE, new MoveUpCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_Q, new MoveDownCommand(camera));
    }
}