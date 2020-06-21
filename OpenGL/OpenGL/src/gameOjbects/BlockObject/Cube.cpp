#include "cube.h"

Cube::Cube()
	:Position(defaultPosition)
{
    SetupMesh();
    BindTexture("res/textures/grass.png");
}

Cube::~Cube()
{
}

void Cube::SetupMesh()
{
    VertexBufferLayout layout;
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(&cubeVertices[0], sizeof(cubeVertices));

    layout.Push<float>(3);
    layout.Push<float>(2);
    m_VAO->AddBuffer(*m_VBO, layout);
}

void Cube::Load()
{
  
}

void Cube::BindTexture(const char* filepath)
{
    m_Texture = std::make_unique<Texture>(filepath);
}

void Cube::Update()
{
}

void Cube::Draw(Shader& shader)
{
    shader.Bind();
    shader.SetUniform1i("u_Texture", 0);
    m_Texture->Bind();
    Renderer renderer;
    renderer.DrawCube(*m_VAO, shader);
    m_VAO->UnBind();
    m_Texture->UnBind();
}


