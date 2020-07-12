#include "TestHDR.h"
#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
namespace test {

    TestHDR::TestHDR()
        :m_Translation(0, 0, 0), m_Rotation(0.0, 0.0, 0.0),
        m_Proj(glm::perspective(glm::radians(viewAngle), 1280.0f / 720.0f, 0.1f, 100.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6.0f))),
        m_shader("res/shader/Assimp.shader"),
        shader("res/shader/HDR/lighting.shader"),
        hdr_shader("res/shader/HDR/hdr.shader"),
        light_Pos(glm::vec3(2.0, 9.0, 4.0))
    {        
        camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));

        alice = std::make_unique<Cube>();
        quad_NDC = std::make_unique<Quad_NDC>();

        keyboardController = KeyboardController::getInstance();
        InitController();

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));
        GLCall(glEnable(GL_DEPTH_TEST));

        glGenFramebuffers(1, &hdrFBO);
        // create floating point color buffer
        glGenTextures(1, &colorBuffer);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // create depth buffer (renderbuffer)
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
        // attach buffers
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        shader.Bind();
        shader.SetUniform1i("diffuseTexture", 0);
        hdr_shader.Bind();
        hdr_shader.SetUniform1i("hdrBuffer", 0);

    }

    TestHDR::~TestHDR()
    {
        std::cout << "keyboard unloaded" << std::endl;
        keyboardController->UnRegisterCommand();
        delete camera;
    }

    void TestHDR::OnUpdate(float deltaTime)
    {
        camera->updateCameraVectors();
        m_View = camera->GetViewMartix();
    }

    void TestHDR::OnRender()
    {       
        glViewport(0, 0, 1280, 720);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(glm::radians(viewAngle), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera->GetViewMartix();
        shader.Bind();
        shader.SetUniformMat4f("projection", projection);
        shader.SetUniformMat4f("view", view);
        // set lighting uniforms
        for (unsigned int i = 0; i < lightPositions.size(); i++)
        {
            shader.SetUniformVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
            shader.SetUniformVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
        }
        //shader.SetUniformVec3("viewPos", camera->GetPosition());
        // render tunnel
        shader.SetUniform1i("inverse_normals", true);
        RenderScene(shader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        

        /// <summary>
        bool hdr = true;
        float exposure = 1.0f;
        glViewport(0, 0, 1280, 720);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        hdr_shader.Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        hdr_shader.SetUniform1i("hdr", hdr);
        hdr_shader.SetUniform1f("exposure", exposure);
        quad_NDC->Draw(hdr_shader);

        //std::cout << "hdr: " << (hdr ? "on" : "off") << "| exposure: " << exposure << std::endl;
        /// </summary>

        
    }

    void TestHDR::OnImGuiRender()
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

    void TestHDR::RenderScene(Shader& shader)
    {
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);   
        model = glm::translate(model, glm::vec3(0.0, 0.5, 0.0));
        shader.SetUniformMat4f("model", model);
        alice->Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0, 0.0, 25.0));
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
        shader.SetUniformMat4f("model", model);
        alice->Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0, 4.0, -1.0));
        shader.SetUniformMat4f("model", model);
        alice->Draw(shader);
    }

    void TestHDR::ResetPosition()
    {
        m_Translation = glm::vec3(0, 0, 0);
        m_Rotation = glm::vec3(0, 0, 0);
        m_Proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6.0f));
        camera->ResetPosition();
    }

    void TestHDR::InitController()
    {
        std::cout << "keyboard Reloaded" << std::endl;
        keyboardController->RegisterCommand(GLFW_KEY_W, new MoveForwardCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_S, new MoveBackCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_A, new MoveLeftCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_D, new MoveRightCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_SPACE, new MoveUpCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_Q, new MoveDownCommand(camera));
    }

    void TestHDR::UnRegisterController() 
    {
        keyboardController->UnRegisterCommand();
    }

}