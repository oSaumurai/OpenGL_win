#include "PBR_Test.h"
namespace test {
    PBR_Test::PBR_Test() {
        camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));
        keyboardController = KeyboardController::getInstance();
        InitController();
        glEnable(GL_DEPTH_TEST);

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));
        GLCall(glEnable(GL_DEPTH_TEST));
    }

    PBR_Test::~PBR_Test()
    {
        UnRegisterController();
    }

    void PBR_Test::OnUpdate(float deltaTime)
    {
        camera->updateCameraVectors();
        m_View = camera->GetViewMartix();
        
        //skybox//

        glm::mat4 model_skybox = glm::mat4(1.0f);
        model_skybox = glm::translate(model_skybox,  camera->GetPosition());
        model_skybox = glm::scale(model_skybox, glm::vec3(100, 100, 100));
        glm::mat4 mvp_skybox = m_Proj * m_View * model_skybox;
        skybox_shader.Bind();
        skybox_shader.SetUniformMat4f("u_MVP", mvp_skybox);

    }

    void PBR_Test::OnRender()
    {        
        //sphere//
        glm::mat4 model = glm::mat4(1.0f);
        //Model Translation, Rotation , Scale
        model = glm::translate(model, m_Translation); 
        model = glm::translate(model, glm::vec3(0.0, 0.5, 0.0));
        //glm::mat4 mvp = m_Proj * m_View * model;

        // PBR //
        PBR_shader.Bind();

        PBR_shader.SetUniformMat4f("projection", m_Proj);
        PBR_shader.SetUniformMat4f("view", m_View);
        PBR_shader.SetUniformMat4f("model", model);
        PBR_shader.SetUniformVec3("camPos", camera->GetPosition());

        PBR_shader.SetUniform1i("albedoMap", 0);
        PBR_shader.SetUniform1i("normalMap", 1);
        PBR_shader.SetUniform1i("metallicMap", 2);
        PBR_shader.SetUniform1i("roughnessMap", 3);
        PBR_shader.SetUniform1i("aoMap", 4);

        albedo.Bind(0);
        normal.Bind(1);
        metallic.Bind(2);
        roughness.Bind(3);
        ao.Bind(4);

        glm::vec3 lightPositions[] = {
            glm::vec3(0.0f, 0.0f, 10.0f),
        };
        glm::vec3 lightColors[] = {
            glm::vec3(150.0f, 150.0f, 150.0f),
        };
        PBR_shader.SetUniformVec3("lightPositions", lightPositions[0]);
        PBR_shader.SetUniformVec3("lightColors", lightColors[0]);
        
        //for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i) {
        //    glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        //    newPos = lightPositions[i];
        //    PBR_shader.SetUniformVec3("lightPositions[" + std::to_string(i) + "]", newPos);
        //    PBR_shader.SetUniformVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

        //    model = glm::mat4(1.0f);
        //    model = glm::translate(model, newPos);
        //    model = glm::scale(model, glm::vec3(0.5f));
        //    PBR_shader.SetUniformMat4f("model", model);
        //    sphere.Draw(PBR_shader);
        //}

        for (unsigned int i = 0; i < 4; ++i) {
            model = glm::translate(model, glm::vec3(3.0, 0.0, 0.0));
            PBR_shader.SetUniformMat4f("model", model);
            sphere.Draw(PBR_shader);
        }
        glDepthFunc(GL_LEQUAL);
        skybox.Draw(skybox_shader);
        glDepthFunc(GL_LESS);
    }

    //To abstract
    void PBR_Test::OnImGuiRender()
    {
        ImGui::Begin("Hello, world!");
        ImGui::SliderFloat3("Translation x y z", &m_Translation.x, -10.0f, 10.0f);
        ImGui::SliderFloat("Perspective",   &viewAngle, -180.0f, 180.0f);
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