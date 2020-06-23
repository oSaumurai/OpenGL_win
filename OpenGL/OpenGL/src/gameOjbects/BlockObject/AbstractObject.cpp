#include "AbstractObject.h"

void AbstractObject::Draw(Shader& shader)
{
    shader.Bind();
    shader.SetUniform1i("u_Texture", 0);
    m_Texture->Bind();
    Renderer renderer;
    renderer.DrawCube(*m_VAO, shader);
    m_VAO->UnBind();
    m_Texture->UnBind();
}

void AbstractObject::BindTexture(const char* filepath)
{
    m_Texture = std::make_unique<Texture>(filepath);
}
