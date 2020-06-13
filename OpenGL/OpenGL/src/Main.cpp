#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "window/window.h"
int main(void)
{
    Window* window=new Window(1280,720);
    
    bool windowShouldClose = window->shouldClose();
    while (!window->shouldClose())
    {
        window->Update();
    }   

    return 0;
}