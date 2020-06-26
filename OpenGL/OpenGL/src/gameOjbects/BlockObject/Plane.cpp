#include "Plane.h"

Plane::Plane()
{
    SetupMesh();
    BindTexture("res/textures/cube_color.jpg");
}

Plane::~Plane()
{
}

void Plane::SetupMesh()
{
    Position = glm::vec3(0.0, 0.0, 0.0);
    VertexBufferLayout layout;
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(&planeVertices[0], sizeof(planeVertices));

    layout.Push<float>(3);
    layout.Push<float>(3);
    layout.Push<float>(2);
    m_VAO->AddBuffer(*m_VBO, layout);
}
