#include "TestLoader.h"
#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
namespace test {

    TestLoader::TestLoader()
        :m_Translation(0, 0, 0), m_Rotation(0.0, 0.0, 0.0),
        m_Proj(glm::perspective(glm::radians(viewAngle), 1280.0f/720.0f , 0.1f, 100.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6.0f))),
        m_shader("res/shader/Assimp.shader"),
        skybox_shader("res/shader/skybox.shader")
    {        
        camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));
        //model = std::make_unique<Model>("res/object/Tree1/Tree1.obj");
        grass = std::make_unique<Grass>();
        alice = std::make_unique<Cube>();
        skybox = std::make_unique<Skybox>("res/object/skybox");
        //model = std::make_unique<Model>("res/object/beach/obj/scene.obj");
        //model = std::make_unique<Model>("res/object/scene/spacestation/Space Station Scene.blend");
        //model = std::make_unique<Model>("res/object/backpack/backpack.obj");
        //model = std::make_unique<Model>("res/object/kouhai-chan/kouhai.obj");

        keyboardController = KeyboardController::getInstance();
        InitController();

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));
        GLCall(glEnable(GL_DEPTH_TEST));
    }

    TestLoader::~TestLoader()
    {
        std::cout << "keyboard unloaded" << std::endl;
        keyboardController->UnRegisterCommand();
        delete camera;
    }

    void TestLoader::OnUpdate(float deltaTime)
    {
        camera->updateCameraVectors();
        m_View = camera->GetViewMartix();
        glm::mat4 model = glm::mat4(1.0f);
        //Model Translation, Rotation , Scale
        model = glm::translate(model, m_Translation);
        model = glm::rotate(model, m_Rotation.x , glm::vec3(1.0, 0.0, 0.0));
        model = glm::rotate(model, m_Rotation.y , glm::vec3(0.0, 1.0, 0.0));
        model = glm::rotate(model, m_Rotation.z , glm::vec3(0.0, 0.0, 1.0));
        m_Proj = glm::perspective(glm::radians(viewAngle), 1.0f, 0.1f, 100.0f);
        glm::mat4 mvp = m_Proj * m_View * model;
        m_shader.Bind();
        m_shader.SetUniformMat4f("u_MVP", mvp);


        //skybox translation
        glm::mat4 model_skybox = glm::mat4(1.0f);
        model_skybox = glm::scale(model_skybox, glm::vec3(100, 100, 100));
        glm::mat4 mvp_skybox = m_Proj * m_View * model_skybox;
        skybox_shader.Bind();
        skybox_shader.SetUniformMat4f("u_MVP", mvp_skybox);
    }
    void TestLoader::OnRender()
    {       
        glDepthFunc(GL_LEQUAL);
        skybox->Draw(skybox_shader);
        glDepthFunc(GL_LESS);
        //model->Draw(m_shader);
        grass->Draw(m_shader);
        alice->Draw(m_shader);
    }
    void TestLoader::OnImGuiRender()
    {
        // Create a window called "Hello, world!" and append into it.
        ImGui::Begin("Hello, world!");
        ImGui::SliderFloat3("Translation x y z", &m_Translation.x, -10.0f, 10.0f);
        ImGui::SliderFloat("Perspective", &viewAngle, -180.0f, 180.0f);
        ImGui::SliderAngle("Rotation on x", &m_Rotation.x);
        ImGui::SliderAngle("Rotation on y", &m_Rotation.y);
        ImGui::SliderAngle("Rotation on z", &m_Rotation.z);
        if (ImGui::Button("Reset Position  "))
            ResetPosition();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    void TestLoader::ResetPosition()
    {
        m_Translation = glm::vec3(0, 0, 0);
        m_Rotation = glm::vec3(0, 0, 0);
        m_Proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6.0f));
        camera->ResetPosition();
    }

    void TestLoader::InitController()
    {
        std::cout << "keyboard Reloaded" << std::endl;
        keyboardController->RegisterCommand(GLFW_KEY_W, new MoveForwardCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_S, new MoveBackCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_A, new MoveLeftCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_D, new MoveRightCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_SPACE, new MoveUpCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_Q, new MoveDownCommand(camera));
    }

    void TestLoader::UnRegisterController() 
    {
        keyboardController->UnRegisterCommand();
    }
}