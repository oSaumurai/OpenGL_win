#include "Game.h"

Game::Game()
{
	initController();
    initTestMember();
}

Game::~Game()
{
    delete currentTest;
    if (currentTest != testMenu)
        delete testMenu;
}

void Game::Init()
{	
	
}

void Game::initController()
{
    mouseController = MouseController::getInstance();
    keyboardController = KeyboardController::getInstance();
    mouseController->AttachWindowInput(window);
    keyboardController->AttachWindowInput(window);
    keyboardController->RegisterCommand(GLFW_KEY_ESCAPE, new ExitCommand(window));
}

void Game::initTestMember()
{
	currentTest = nullptr;
	testMenu = new test::TestMenu(currentTest);
	currentTest = testMenu;

	testMenu->RegisterTest<test::TestClearColor>("Clear Color");
	testMenu->RegisterTest<test::TestTexture2D>("Texture 2D");
	testMenu->RegisterTest<test::TestTexture3D>("Texture 3D");
	testMenu->RegisterTest<test::TestLoader>("ShadowMapping");
    testMenu->RegisterTest<test::TestPointShadow>("Point Shadow");
    testMenu->RegisterTest<test::TestHDR>("HDR Test");
    testMenu->RegisterTest<test::PBR_Test>("PBR Test");
}

void Game::LoadContent()
{
}

void Game::UnLoadContent()
{
}

void Game::OnWindowUpdate()
{
    mouseController->updateInput();
    keyboardController->updateInput();
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Game::Update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if (currentTest)
    {
        currentTest->OnUpdate(0.0f);
        currentTest->OnRender();
        ImGui::Begin("Test");
        if (currentTest != testMenu && ImGui::Button("<-"))
        {
            delete currentTest;
            currentTest = testMenu;
        }
        currentTest->OnImGuiRender();
        ImGui::End();
    }
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	

    /* Swap front and back buffers */
    /* Poll for and process events */
    glGetError();
    GLCall(glfwSwapBuffers(window));
    GLCall(glfwPollEvents());
}

void Game::Draw()
{
}
