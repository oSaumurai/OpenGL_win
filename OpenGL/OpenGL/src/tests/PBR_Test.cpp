#include "PBR_Test.h"
namespace test {
    PBR_Test::PBR_Test() {
        camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));
        keyboardController = KeyboardController::getInstance();
        InitController();
    }

    PBR_Test::~PBR_Test()
    {
        UnRegisterController();
    }

    void PBR_Test::OnUpdate(float deltaTime)
    {
        camera->updateCameraVectors();
        m_View = camera->GetViewMartix();

        glm::mat4 model_skybox = glm::mat4(1.0f);
        model_skybox = glm::scale(model_skybox, glm::vec3(100, 100, 100));
        glm::mat4 mvp_skybox = m_Proj * m_View * model_skybox;
        skybox_shader.Bind();
        skybox_shader.SetUniformMat4f("u_MVP", mvp_skybox);
    }

    void PBR_Test::OnRender()
    {
        glDepthFunc(GL_LEQUAL);
        skybox.Draw(skybox_shader);
        glDepthFunc(GL_LESS);
    }

    void PBR_Test::OnImGuiRender()
    {
        ImGui::Begin("Hello, world!");
        ImGui::SliderFloat3("Translation x y z", &m_Translation.x, -10.0f, 10.0f);
        ImGui::SliderFloat("Perspective", &viewAngle, -180.0f, 180.0f);
        ImGui::SliderAngle("Rotation on x", &m_Rotation.x);
        ImGui::SliderAngle("Rotation on y", &m_Rotation.y);
        ImGui::SliderAngle("Rotation on z", &m_Rotation.z);
        //if (ImGui::Button("Reset Position  "))
        //    ResetPosition();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    void PBR_Test::InitController()
    {
        std::cout << "keyboard Reloaded" << std::endl;
        keyboardController->RegisterCommand(GLFW_KEY_W,     new MoveForwardCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_S,     new MoveBackCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_A,     new MoveLeftCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_D,     new MoveRightCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_SPACE, new MoveUpCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_Q,     new MoveDownCommand(camera));
    }

    void PBR_Test::UnRegisterController()
    {
        keyboardController->UnRegisterCommand();
    }
}