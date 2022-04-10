#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "VertexArray.h"
#include "gameOjbects/AbstractObject.h"
#include "IndexBuffer.h"
class Sphere{
public:
    Sphere();
    ~Sphere();

    void Draw(Shader& shader);
    void Update() ;
private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO;

    VertexBufferLayout layout;
    void SetupMesh();
    void SetupTriangle();
    std::vector<float> data;
    unsigned int indexCount;
    std::unique_ptr<IndexBuffer> ib;
    std::vector<unsigned int> indices;
};

