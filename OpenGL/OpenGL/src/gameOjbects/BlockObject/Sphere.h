#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "VertexArray.h"
#include "gameOjbects/AbstractObject.h"
#include "IndexBuffer.h"
class Sphere : public AbstractObject{
public:
    Sphere();
    ~Sphere();

    void SetupMesh() override;
    void Draw(Shader& shader) override;
    void Update() override;
private:
    void SetupTriangle();
    std::vector<float> data;
    unsigned int indexCount;
    std::unique_ptr<IndexBuffer> ib;
    std::vector<unsigned int> indices;
};

