#include "cube.h"

Cube::Cube()
{
    SetupMesh();
    BindTexture("res/textures/alice.png");
}

Cube::~Cube()
{
}

void Cube::SetupMesh()
{
    Position = glm::vec3(0.0, 0.0, 0.0);
    VertexBufferLayout layout;
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(&cubeVertices[0], sizeof(cubeVertices));

    layout.Push<float>(3);
    layout.Push<float>(2);
    m_VAO->AddBuffer(*m_VBO, layout);
}

void Cube::Update()
{
}
