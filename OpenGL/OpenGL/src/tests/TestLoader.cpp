#include "TestLoader.h"
#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
namespace test {

    TestLoader::TestLoader()
        :m_Translation(0, 0, 0), m_Rotation(0.0, 0.0, 0.0)
    {        
        camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));
        alice = std::make_unique<Cube>();
        plane = std::make_unique<Plane>();
        sphere = std::make_unique<Sphere>();
        quad_NDC = std::make_unique<Quad_NDC>();
        skybox = std::make_unique<Skybox>("res/object/skybox");
        //model = std::make_unique<Model>("res/object/Tree1/Tree1.obj");
        //grass = std::make_unique<Grass>();
        //model = std::make_unique<Model>("res/object/beach/obj/scene.obj");
        //model = std::make_unique<Model>("res/object/scene/spacestation/Space Station Scene.blend");
        //model = std::make_unique<Model>("res/object/backpack/backpack.obj");
        kouhai = std::make_unique<Model>("res/object/kouhai-chan/kouhai.obj");
        keyboardController = KeyboardController::getInstance();
        InitController();

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));
        GLCall(glEnable(GL_DEPTH_TEST));

        const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

        GLCall(glGenFramebuffers(1, &depthMapFBO));
        // create depth texture
        GLCall(glGenTextures(1, &depthMap));
        GLCall(glBindTexture(GL_TEXTURE_2D, depthMap));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        float borderColor[] = { 1.0,1.0,1.0,1.0 };
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);
        // attach depth texture as FBO's depth buffer
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0));
        GLCall(glDrawBuffer(GL_NONE));
        GLCall(glReadBuffer(GL_NONE));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
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

        model = glm::translate(model, glm::vec3(2.0,2.0,2.0));
        //glm::mat4 mvp1 = m_Proj * m_View * model;
        Grass_shader.Bind();
        Grass_shader.SetUniformMat4f("u_MVP", mvp);

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
        //skybox->Draw(skybox_shader);
        glDepthFunc(GL_LESS);
        
        
        //shadow map
        debug_quad.Bind();
        debug_quad.SetUniform1i("depthMap", 0);

        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 15.0f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(light_Pos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        DepthShader.Bind();
        DepthShader.SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, 1024, 1024);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        //glActiveTexture(GL_TEXTURE0);
        //
        glm::mat4 model = glm::mat4(1.0);
        DepthShader.SetUniformMat4f("model", model);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        //
        plane->Draw(DepthShader);
        glBindTexture(GL_TEXTURE_2D, 0);
        glCullFace(GL_FRONT);
        RenderScene(DepthShader);
        glCullFace(GL_BACK);
        //grass->Draw(Grass_shader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //////
        glViewport(0, 0, 1280, 720);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // shadow map //
        shadowMap_shader.Bind();
        shadowMap_shader.SetUniform1i("diffuseTexture", 0);
        shadowMap_shader.SetUniform1i("shadowMap", 1);
        shadowMap_shader.SetUniformMat4f("projection", m_Proj);
        shadowMap_shader.SetUniformMat4f("view", m_View);
        shadowMap_shader.SetUniformMat4f("model", model);
        shadowMap_shader.SetUniformVec3("viewPos", camera->GetPosition());
        shadowMap_shader.SetUniformVec3("lightPos", light_Pos);
        shadowMap_shader.SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        plane->Draw(shadowMap_shader);
        glBindTexture(GL_TEXTURE_2D, 0);

        kouhai->Draw(m_shader);

        // PBR //



        //PBR_shader.Bind();
       /* PBR_shader.SetUniform1i("albedoMap", 0);
        PBR_shader.SetUniform1i("normalMap", 1);
        PBR_shader.SetUniform1i("metallicMap", 2);
        PBR_shader.SetUniform1i("roughnessMap", 3);
        PBR_shader.SetUniform1i("aoMap", 4);

        PBR_shader.SetUniformMat4f("projection", m_Proj);
        PBR_shader.SetUniformMat4f("view", m_View);
        PBR_shader.SetUniformVec3("camPos", camera->GetPosition());*/

      /*  albedo.Bind(0);
        normal.Bind(1);
        metallic.Bind(2);
        roughness.Bind(3);
        ao.Bind(4);*/

        glm::vec3 lightPositions[] = {
            glm::vec3(0.0f, 0.0f, 10.0f),
        };
        glm::vec3 lightColors[] = {
            glm::vec3(150.0f, 150.0f, 150.0f),
        };
        for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
        {
            glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
            newPos = lightPositions[i];
            //PBR_shader.SetUniformVec3("lightPositions[" + std::to_string(i) + "]", newPos);
            //PBR_shader.SetUniformVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

            model = glm::mat4(1.0f);
            model = glm::translate(model, newPos);
            model = glm::scale(model, glm::vec3(0.5f));
            //PBR_shader.SetUniformMat4f("model", model);
            //sphere->Draw(PBR_shader);
        }

        //RenderScene(shadowMap_shader);

        glDepthFunc(GL_LEQUAL);
        skybox->Draw(skybox_shader);
        glDepthFunc(GL_LESS);

        //debug usage
        debug_quad.Bind();
        //debug_quad.SetUniform1f("near_plane", near_plane);
        //debug_quad.SetUniform1f("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        //quad_NDC->Draw(debug_quad);
        
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

    void TestLoader::RenderScene(Shader& shader)
    {
        
        glm::mat4 model = glm::mat4(1.0f);
        /*shader.SetUniformMat4f("model", model);        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        plane->Draw(shader); 
        glBindTexture(GL_TEXTURE_2D, 0);*/

        /////
        //glCullFace(GL_FRONT);
        model = glm::mat4(1.0f);   
        model = glm::translate(model, glm::vec3(0.0, 0.5, 0.0));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        shader.SetUniformMat4f("model", model);
        alice->Draw(shader);
        glBindTexture(GL_TEXTURE_2D, 0);

        model = glm::translate(model, glm::vec3(2.0, 2.5, 0.0));
        //glm::mat4 mvp = m_Proj * m_View * model;
        shader.SetUniformMat4f("model", model);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        alice->Draw(shader);
        glBindTexture(GL_TEXTURE_2D, 0);

        model = glm::translate(model, glm::vec3(-3.0, 3.0, -1.0));
        //mvp = m_Proj * m_View * model;
        shader.SetUniformMat4f("model", model);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        //sphere->Draw(shader);
        glBindTexture(GL_TEXTURE_2D, 0);
        //kouhai->Draw(shader);

        //glCullFace(GL_BACK);
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