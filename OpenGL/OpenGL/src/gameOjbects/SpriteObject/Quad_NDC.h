#pragma once
#include "Quad.h"
#include "../AbstractObject.h"
class Quad_NDC :public AbstractObject
{
    const float quadVertices[20] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
public:
	Quad_NDC() {
        SetupMesh();
    };
	~Quad_NDC() {};

    void Draw(Shader& shader) override
    {
        shader.Bind();
        //m_Texture->Bind();
        Renderer renderer;
        renderer.DrawTriangle(*m_VAO, shader);
        m_VAO->UnBind();
        //m_Texture->UnBind();
    };

    void SetupMesh() override 
    {
        m_VAO = std::make_unique<VertexArray>();
        m_VBO = std::make_unique<VertexBuffer>(&quadVertices[0], sizeof(quadVertices));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);

        m_VAO->AddBuffer(*m_VBO, layout);
    };
	void Update() override {};
};