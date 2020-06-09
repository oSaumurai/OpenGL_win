#include "Window.h"
#include <iostream>

Window::Window(int height, int width)
    :window_height(height),window_witdth(width)
{
    initGLfw();
    initOpenGLOption();
}

Window::~Window()
{
    glfwTerminate();
    glfwDestroyWindow(window);
}

void Window::Update()
{
}

void Window::Draw()
{
}



void Window::initGLfw()
{
    if (!glfwInit())
    {
        __debugbreak();
        //std::cout << "ERROR loading Glew" << std::endl;
        glfwTerminate();
    }
}

void Window::initOpenGLOption()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void initGLew() 
{
    if (glewInit() != GLEW_OK)
        std::cout << "not ok" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
}
