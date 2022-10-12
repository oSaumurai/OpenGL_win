#include "PBR_Test.h"
namespace test {
    PBR_Test::PBR_Test() {
        camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));
        keyboardController = KeyboardController::getInstance();
        InitController();

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));
        GLCall(glEnable(GL_DEPTH_TEST));
        PBR_shader.Bind();
        PBR_shader.SetUniform1i("irradianceMap", 0);
        PBR_shader.SetUniform1i("prefilterMap", 1);
        PBR_shader.SetUniform1i("brdfLUT", 2);
        PBR_shader.SetUniform1i("albedoMap", 3);
        PBR_shader.SetUniform1i("normalMap", 4);
        PBR_shader.SetUniform1i("metallicMap", 5);
        PBR_shader.SetUniform1i("roughnessMap", 6);
        initMap();
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
        //model_skybox = glm::scale(model_skybox, glm::vec3(100, 100, 100));
        glm::mat4 mvp_skybox = m_Proj * m_View * model_skybox;
        skybox_shader.Bind();
        skybox_shader.SetUniformMat4f("u_MVP", mvp_skybox);

        PBR_shader.Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);

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

        PBR_shader.SetUniform1i("albedoMap", 3);
        PBR_shader.SetUniform1i("normalMap", 4);
        PBR_shader.SetUniform1i("metallicMap", 5);
        PBR_shader.SetUniform1i("roughnessMap", 6);
        //PBR_shader.SetUniform1i("aoMap", 4);

        albedo.Bind(3);
        normal.Bind(4);
        metallic.Bind(5);
        roughness.Bind(6);
        //ao.Bind(4);

        glm::vec3 lightPositions[] = {
            glm::vec3(0.0f, 0.0f, 10.0f),
        };
        glm::vec3 lightColors[] = {
            glm::vec3(150.0f, 150.0f, 150.0f),
        };
        PBR_shader.SetUniformVec3("lightPositions", lightPositions[0]);
        PBR_shader.SetUniformVec3("lightColors", lightColors[0]);

    }

    void PBR_Test::OnRender()
    {        
        //albedo.UnBind();
        //normal.UnBind();
        //metallic.UnBind();
        //roughness.UnBind();

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
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_Translation);
        model = glm::translate(model, glm::vec3(0.0, 0.5, 0.0));

        sphere.Draw(PBR_shader);

        model = glm::translate(model, glm::vec3(3.0, 0.0, 0.0));
        PBR_shader.SetUniformMat4f("model", model);
        copper_albedo.Bind(3);
        copper_normal.Bind(4);
        copper_metallic.Bind(5);
        copper_roughness.Bind(6);
        sphere.Draw(PBR_shader);


        //for (unsigned int i = 0; i < 4; ++i) {
        //    model = glm::translate(model, glm::vec3(3.0, 0.0, 0.0));
        //    PBR_shader.SetUniformMat4f("model", model);
        //    sphere.Draw(PBR_shader);
        //}

        // potion //
        model = glm::translate(model, glm::vec3(0.f, 0.f, -10.f));
        glm::mat4 mvp = m_Proj * m_View * model;
        model_shader.Bind();
        model_shader.SetUniformMat4f("u_MVP", mvp);
        potion.Draw(model_shader);

        // skybox //
        glDepthFunc(GL_LEQUAL);
        skybox.Draw(skybox_shader);
        glDepthFunc(GL_LESS);

    }

    void PBR_Test::initMap()
    {
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

        glm::mat4 model_skybox = glm::mat4(1.0f);
        
        glGenTextures(1, &envCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA32F, 512, 512, 0, GL_RGBA, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // project cubmap to framebuffer
        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] =
        {
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        //unsigned int envCubemap = skybox.m_RenderID;
        // convert HDR equirectangular environment map to cubemap equivalent
        //skybox_shader.Bind();
        //skybox_shader.SetUniform1i("equirectangularMap", 0);
        //skybox_shader.SetUniformMat4f("projection", captureProjection);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, hdrTexture);

        glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
        //glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        //for (unsigned int i = 0; i < 6; ++i)
        //{
        //    glm::mat4 mvp = captureProjection * captureViews[i] * model_skybox;
        //    skybox_shader.SetUniformMat4f("u_MVP", mvp);
        //    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        //        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, hdrTexture, 0);
        //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //    //renderCube(); // renders a 1x1 cube
        //    skybox.Draw(skybox_shader);
        //}
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        //glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        // irradiance map texture generattion//
        // -----------------------------------------------------------------------------
        envCubemap = skybox.m_RenderID;
        glGenTextures(1, &irradianceMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA32F, 32, 32, 0, GL_RGBA, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, 32, 32);

        // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
        // -----------------------------------------------------------------------------
        irradianceShader.Bind();
        irradianceShader.SetUniform1i("environmentMap", 0);
        irradianceShader.SetUniformMat4f("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            irradianceShader.SetUniformMat4f("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            skybox.Draw(irradianceShader);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
        // --------------------------------------------------------------------------------
        glGenTextures(1, &prefilterMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA32F, 128, 128, 0, GL_RGBA, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
        // ----------------------------------------------------------------------------------------------------
        prefilterShader.Bind();
        prefilterShader.SetUniform1i("environmentMap", 0);
        prefilterShader.SetUniformMat4f("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        unsigned int maxMipLevels = 5;
        for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
        {
            // reisze framebuffer according to mip-level size.
            unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
            unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, mipWidth, mipHeight);
            glViewport(0, 0, mipWidth, mipHeight);

            float roughness = (float)mip / (float)(maxMipLevels - 1);
            prefilterShader.SetUniform1f("roughness", roughness);
            for (unsigned int i = 0; i < 6; ++i)
            {
                prefilterShader.SetUniformMat4f("view", captureViews[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                skybox.Draw(prefilterShader);
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // pbr: generate a 2D LUT from the BRDF equations used.
        // ----------------------------------------------------
        glGenTextures(1, &brdfLUTTexture);

        // pre-allocate enough memory for the LUT texture.
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
        // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, 512, 512);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

        glViewport(0, 0, 512, 512);
        BRDF_shader.Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderQuad();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 1920, 1080);
    }

    void PBR_Test::renderQuad()
    {
        if (quadVAO == 0)
        {
            float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

    //To be abstract
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