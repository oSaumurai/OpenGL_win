#include "TestPointShadow.h"
#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
namespace test {

    TestPointShadow::TestPointShadow()
        :m_Translation(0, 0, 0), m_Rotation(0.0, 0.0, 0.0),
        m_Proj(glm::perspective(glm::radians(viewAngle), 1280.0f / 720.0f, 0.1f, 100.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6.0f))),
        m_shader("res/shader/Assimp.shader"),
        skybox_shader("res/shader/skybox.shader"),
        Grass_shader("res/shader/Grass.shader"),
        debug_quad("res/shader/debug_quad.shader"),
        DepthShader("res/shader/pointshadow_depth.shader"),
        shadowMap_shader("res/shader/pointshadow.shader"),
        light_Pos(glm::vec3(-5.0, 0.0, 0.0))
    {        
        camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));
        shadows_Switch = new bool(true);
        //model = std::make_unique<Model>("res/object/Tree1/Tree1.obj");
        //grass = std::make_unique<Grass>();
        alice = std::make_unique<Cube>();
        //plane = std::make_unique<Plane>();
        quad_NDC = std::make_unique<Quad_NDC>();
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
        GLCall(glEnable(GL_CULL_FACE));
        const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

        GLCall(glGenFramebuffers(1, &depthMapFBO));
        // create depth texture
        GLCall(glGenTextures(1, &depthMap));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap));
        for (unsigned int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        //float borderColor[] = { 1.0,1.0,1.0,1.0 };
        //glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);
        // attach depth texture as FBO's depth buffer
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO));
        GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0));
        GLCall(glDrawBuffer(GL_NONE));
        GLCall(glReadBuffer(GL_NONE));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    TestPointShadow::~TestPointShadow()
    {
        std::cout << "keyboard unloaded" << std::endl;
        keyboardController->UnRegisterCommand();
        delete camera;
        delete shadows_Switch;
    }

    void TestPointShadow::OnUpdate(float deltaTime)
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

        //model = glm::translate(model, glm::vec3(2.0,2.0,2.0));
        //glm::mat4 mvp1 = m_Proj * m_View * model;
        Grass_shader.Bind();
        Grass_shader.SetUniformMat4f("u_MVP", mvp);

        //skybox translation
        glm::mat4 model_skybox = glm::mat4(1.0f);
        model_skybox = glm::scale(model_skybox, glm::vec3(10, 10, 10));
        glm::mat4 mvp_skybox = m_Proj * m_View * model_skybox;
        skybox_shader.Bind();
        skybox_shader.SetUniformMat4f("u_MVP", mvp_skybox);
    }
    
    void TestPointShadow::OnRender()
    {       
        float near_plane = 1.0f;
        float far_plane = 35.0f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)1024 / (float)1024, near_plane, far_plane);
        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(shadowProj * glm::lookAt(light_Pos, light_Pos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(light_Pos, light_Pos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(light_Pos, light_Pos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(light_Pos, light_Pos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(light_Pos, light_Pos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(light_Pos, light_Pos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

        DepthShader.Bind();
        for (unsigned int i = 0; i < 6; ++i)
            DepthShader.SetUniformMat4f("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
        DepthShader.SetUniform1f("far_plane", far_plane);
        DepthShader.SetUniformVec3("lightPos", light_Pos);

        glViewport(0, 0, 1024, 1024);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

            glCullFace(GL_FRONT);
            RenderScene(DepthShader);
            glCullFace(GL_BACK);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //////
        glViewport(0, 0, 1280, 720);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /////
        shadowMap_shader.Bind();
        shadowMap_shader.SetUniform1i("diffuseTexture", 0);
        shadowMap_shader.SetUniform1i("depthMap", 1);
        shadowMap_shader.SetUniformMat4f("projection", m_Proj);
        shadowMap_shader.SetUniformMat4f("view", m_View);
        // set light uniforms
        shadowMap_shader.SetUniformVec3("viewPos", camera->GetPosition());
        shadowMap_shader.SetUniformVec3("lightPos", light_Pos);
        shadowMap_shader.SetUniform1i("shadows", 1);
        shadowMap_shader.SetUniform1f("far_plane", far_plane);


        GLCall(glActiveTexture(GL_TEXTURE1));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap));
        RenderScene(shadowMap_shader);

        //debug usage
        //debug_quad.Bind();
        //debug_quad.SetUniform1f("near_plane", near_plane);
        //debug_quad.SetUniform1f("far_plane", far_plane);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, depthMap);
        //quad_NDC->Draw(debug_quad);
        
    }

    void TestPointShadow::OnImGuiRender()
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

    void TestPointShadow::RenderScene(Shader& shader)
    {
        
        glm::mat4 model = glm::mat4(1.0f);
        shader.Bind();
        glDisable(GL_CULL_FACE);
        model = glm::scale(model, glm::vec3(15.0f));
        shader.SetUniformMat4f("model", model);
        shader.SetUniform1i("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
        alice->Draw(shader);
        //skybox->Draw(shader);
        shader.SetUniform1i("reverse_normals", 0); // and of course disable it
        glEnable(GL_CULL_FACE);
        /////
        //glCullFace(GL_FRONT);
        model = glm::mat4(1.0f);   
        model = glm::translate(model, glm::vec3(0.0, 0.5, 0.0));
        shader.SetUniformMat4f("model", model);
        alice->Draw(shader);

        model = glm::translate(model, glm::vec3(3.0, 3.0, 0.0));
        //glm::mat4 mvp = m_Proj * m_View * model;
        shader.SetUniformMat4f("model", model);
        alice->Draw(shader);


        model = glm::translate(model, glm::vec3(-3.0, 4.0, -1.0));
        //mvp = m_Proj * m_View * model;
        shader.SetUniformMat4f("model", model);
        alice->Draw(shader);

        //glCullFace(GL_BACK);
    }

    void TestPointShadow::ResetPosition()
    {
        m_Translation = glm::vec3(0, 0, 0);
        m_Rotation = glm::vec3(0, 0, 0);
        m_Proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6.0f));
        camera->ResetPosition();
    }

    void TestPointShadow::InitController()
    {
        std::cout << "keyboard Reloaded" << std::endl;
        keyboardController->RegisterCommand(GLFW_KEY_W, new MoveForwardCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_S, new MoveBackCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_A, new MoveLeftCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_D, new MoveRightCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_SPACE, new MoveUpCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_Q, new MoveDownCommand(camera));
        keyboardController->RegisterCommand(GLFW_KEY_1, new ShadowmapSwitchCommand(shadows_Switch));
    }

    void TestPointShadow::UnRegisterController() 
    {
        keyboardController->UnRegisterCommand();
    }

}