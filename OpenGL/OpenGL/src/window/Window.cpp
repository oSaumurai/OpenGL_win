#include "Window.h"
#include <iostream>

Window::Window()
    :window_height(HEIGHT), window_witdth(WIDTH)
{
    initGLfw();
    initOpenGLOption();

    window = glfwCreateWindow(window_height, window_witdth, "My GLLLL", NULL, NULL);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    initGLew();
    initGUI();
    glViewport(0, 0, window_height, window_witdth);
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
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::OnWindowUpdate()
{
}

void Window::Update()
{
}

void Window::Draw()
{
}

void Window::OnFinished()
{

}
