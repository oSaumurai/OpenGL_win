#include "Window.h"
#include <iostream>

Window::Window(int height, int width)
    :window_height(height),window_witdth(width)
{
    initGLfw();
    initOpenGLOption();

    window = glfwCreateWindow(height, width, "My GLLLL", NULL, NULL);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    initGLew();
    initGUI();
    glViewport(0, 0, 1280, 720);
    mouseController = MouseController::getInstance();
    keyboardController = KeyboardController::getInstance();
    mouseController->AttachWindowInput(window);
    keyboardController->AttachWindowInput(window);
    keyboardController->RegisterCommand(GLFW_KEY_ESCAPE ,new ExitCommand(window));
}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    delete currentTest;
    if (currentTest != testMenu)
        delete testMenu;

    glfwTerminate();
    glfwDestroyWindow(window);
}

void Window::initGLfw()
{
    if (!glfwInit())
    {
        __debugbreak();
        std::cout << "ERROR loading Glew" << std::endl;
        glfwTerminate();
    }
}

void Window::initOpenGLOption()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Window::initGLew()
{
    if (glewInit() != GLEW_OK)
        std::cout << "not ok" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
}

void Window::initGUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
    glGetError();

    currentTest = nullptr;
    testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestTexture2D>("Texture 2D");
    testMenu->RegisterTest<test::TestTexture3D>("Texture 3D");
    testMenu->RegisterTest<test::TestLoader>("Scene loader");
}

void Window::initController()
{
    /*keyboardController->RegisterCommand(GLFW_KEY_W, new MoveForwardCommand(window));
    keyboardController->RegisterCommand(GLFW_KEY_S, new MoveBackCommand(camera));
    keyboardController->RegisterCommand(GLFW_KEY_A, new MoveLeftCommand(camera));
    keyboardController->RegisterCommand(GLFW_KEY_D, new MoveRightCommand(camera));
    keyboardController->RegisterCommand(GLFW_KEY_SPACE, new MoveUpCommand(camera));
    keyboardController->RegisterCommand(GLFW_KEY_Q, new MoveDownCommand(camera));
    keyboardController->RegisterCommand(GLFW_KEY_ESCAPE, new ExitCommand(camera));*/
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::Update()
{
    /* Loop until the user closes the window */
    //if (!shouldClose())
    //{
    mouseController->updateInput();
    keyboardController->updateInput();
    //std::cout << "x axis:: "<< mouseController->mouse_offset_x << std::endl;
        /* Render here */
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    //renderer.Clear();

    //New Frame
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
    GLCall(glfwSwapBuffers(window));
    GLCall(glfwPollEvents());
    //}
}

void Window::Draw()
{

}