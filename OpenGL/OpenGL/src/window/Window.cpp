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
    
}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

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
        /* Render here */
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        GLCall(glClear(GL_DEPTH_BUFFER_BIT));
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
        glfwSwapBuffers(window);
        glfwPollEvents();
    //}
}

void Window::Draw()
{

}