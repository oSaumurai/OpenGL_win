#include "TestLoader.h"
#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
namespace test {

    TestLoader::TestLoader()
        :m_Translation(0,0,0), 
        m_Proj(glm::perspective(90.0f,1.0f,0.1f,100.0f)), 
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6.0f)))
	{
        camera = new Camera(glm::vec3(0.0f, 0.0f, 2.0f));

        //mouseController = MouseController::getInstance();
        keyboardController = KeyboardController::getInstance();
        //InitController();

        model = std::make_unique<Model>("res/object/backpack/backpack.obj");
        
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));
	}

    TestLoader::~TestLoader()
	{
        delete camera;
        model.reset();
	}

	void TestLoader::OnUpdate(float deltaTime)
	{
        camera->updateCameraVectors();
        m_View = camera->GetViewMartix();
	}
	void TestLoader::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
        model->Draw();
	}
	void TestLoader::OnImGuiRender()
	{
        // Create a window called "Hello, world!" and append into it.
        ImGui::Begin("Hello, world!");                          
        ImGui::SliderFloat3("Translation a", &m_Translation.x, -10.0f, 10.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
	}
    
}