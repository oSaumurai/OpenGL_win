#include "Quad.h"

Quad::Quad()
{
	SetupMesh();
}

Quad::~Quad()
{
}

void Quad::SetupMesh()
{
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(&quadVertices[0], sizeof(quadVertices));

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);

    m_VAO->AddBuffer(*m_VBO, layout);
}

void Quad::Update()
{
}


