#include "Skybox.h"
#include <iostream>

Skybox::Skybox(std::string filepath)
{
    std::vector<std::string> faces
    {
        (filepath + "/right.jpg"),
        (filepath + "/left.jpg"),
        (filepath + "/top.jpg"),
        (filepath + "/bottom.jpg"),
        (filepath + "/back.jpg"),
        (filepath + "/front.jpg"),
    };
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(skyboxVertices, sizeof(skyboxVertices));

    //m_IndexBuffer = std::make_unique<IndexBuffer>(skyboxIndices, sizeof(skyboxIndices));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    m_VAO->AddBuffer(*m_VBO, layout);
    LoadFromFile(faces);
}

Skybox::~Skybox()
{
    std::cout << "skybox released" << std::endl;
    GLCall(glDeleteTextures(1, &m_RenderID));
}

void Skybox::Draw(Shader& shader)
{
    shader.Bind();
    Renderer renderer;
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RenderID));
    renderer.DrawCube(*m_VAO, shader);
    m_VAO->UnBind(); 
    //GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void Skybox::LoadFromFile(std::vector<std::string> faces)
{
    GLCall(glGenTextures(1, &m_RenderID));

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RenderID));
            GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
}