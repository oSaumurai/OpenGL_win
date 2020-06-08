#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>
void GLClearError()
{
    while (!glGetError);
}

bool GLLogCall(const char* function, const char* file, int errorline)
{
    while (GLenum error = glGetError())
    {
        std::stringstream ss;
        ss << std::hex << error; // decimal_value
        std::string res(ss.str());
        std::string new_string = std::string(4 - res.length(), '0') + res;
        std::ifstream stream("../dependencies/GLEW/include/GL/glew.h");
        std::string line;
        while (getline(stream, line)) {
            if (line.find(new_string) != std::string::npos)
                std::cout << line << std::endl;
        }
        std::cout << "[OpenGL Error] (" << error << ")  " << function
            << " " << errorline << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear()
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawCube(const VertexArray& va, const Shader& shader) const
{
    glEnable(GL_DEPTH_TEST);
    shader.Bind();
    va.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}
