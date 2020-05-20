#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (!glGetError);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")  " << function
            << " " << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear()
{
   // GLCall(glClear(GLCOLO));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
