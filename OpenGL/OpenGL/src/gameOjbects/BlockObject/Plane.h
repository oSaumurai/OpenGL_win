#pragma once
#include "Cube.h"

class Plane : public Cube
{
const float planeVertices[48] = {
        // positions            // normals         // texcoords
         500.0f, -0.5f,  500.0f,  0.0f, 1.0f, 0.0f,  500.0f,  0.0f,
        -500.0f, -0.5f,  500.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -500.0f, -0.5f, -500.0f,  0.0f, 1.0f, 0.0f,   0.0f, 500.0f,

         500.0f, -0.5f,  500.0f,  0.0f, 1.0f, 0.0f,  500.0f,  0.0f,
        -500.0f, -0.5f, -500.0f,  0.0f, 1.0f, 0.0f,   0.0f, 500.0f,
         500.0f, -0.5f, -500.0f,  0.0f, 1.0f, 0.0f,  500.0f, 500.0f
    };
public:
    Plane();
    ~Plane();

private:
    void SetupMesh() override;
};