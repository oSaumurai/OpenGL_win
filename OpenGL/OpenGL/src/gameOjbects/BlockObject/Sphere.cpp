#include "Sphere.h"
#include <iostream>
Sphere::Sphere() {
    SetupTriangle();
    SetupMesh();
}
Sphere::~Sphere()
{
}
void Sphere::SetupTriangle()
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> normals;

    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359f;
    for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
    {
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            positions.push_back(glm::vec3(xPos, yPos, zPos));
            uv.push_back(glm::vec2(xSegment, ySegment));
            normals.push_back(glm::vec3(xPos, yPos, zPos));
        }
    }

    bool oddRow = false;
    for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
    {
        if (!oddRow) // even rows: y == 0, y == 2; and so on
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        }
        else
        {
            for (int x = X_SEGMENTS; x >= 0; --x)
            {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y * (X_SEGMENTS + 1) + x);
            }
        }
        oddRow = !oddRow;
    }
    indexCount = static_cast<unsigned int>(indices.size());
    for (unsigned int i = 0; i < positions.size(); ++i)
    {
        data.push_back(positions[i].x);
        data.push_back(positions[i].y);
        data.push_back(positions[i].z);
        if (normals.size() > 0)
        {
            data.push_back(normals[i].x);
            data.push_back(normals[i].y);
            data.push_back(normals[i].z);
        }
        if (uv.size() > 0)
        {
            data.push_back(uv[i].x);
            data.push_back(uv[i].y);
        }
    }
}

void Sphere::SetupMesh()
{
    //if (m_VAO == 0)
    //{
    //    glGenVertexArrays(1, &m_VAO);

    //    unsigned int vbo, ebo;
    //    glGenBuffers(1, &vbo);
    //    glGenBuffers(1, &ebo);

    //    
    //    glBindVertexArray(m_VAO);
    //    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    //    unsigned int stride = (3 + 2 + 3) * sizeof(float);
    //    glEnableVertexAttribArray(0);
    //    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    //    glEnableVertexAttribArray(1);
    //    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    //    glEnableVertexAttribArray(2);
    //    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    //}

    //glBindVertexArray(m_VAO);

    Position = glm::vec3(0.0, 1.0, 0.0);
    VertexBufferLayout layout;
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(&data[0], sizeof(float) * data.size());
    ib = std::make_unique<IndexBuffer>(&indices[0], indexCount);
    layout.Push<float>(3);
    layout.Push<float>(3);
    layout.Push<float>(2);
    m_VAO->AddBuffer(*m_VBO, layout);
}

void Sphere::Draw(Shader& shader)
{
    shader.Bind();
    ib->Bind();
    m_VAO->Bind();
    //shader.SetUniform1i("u_Texture", 0);
    //m_Texture->Bind();
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
    m_VAO->UnBind();
    ib->UnBind();
;   m_Texture->UnBind();
}

void Sphere::Update()
{
}
