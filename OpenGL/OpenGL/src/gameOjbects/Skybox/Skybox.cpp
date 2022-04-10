#include "Skybox.h"
#include <iostream>

Skybox::Skybox(std::string filepath, bool is32Bits)
{
    std::vector<std::string> faces;
    if (!is32Bits) 
    {
        faces =
        {
            (filepath + "/right.jpg"),
            (filepath + "/left.jpg"),
            (filepath + "/top.jpg"),
            (filepath + "/bottom.jpg"),
            (filepath + "/back.jpg"),
            (filepath + "/front.jpg"),
        };
    }
    else 
    {
        faces = 
        {
            (filepath + "/right.png"),
            (filepath + "/left.png"),
            (filepath + "/top.png"),
            (filepath + "/bottom.png"),
            (filepath + "/back.png"),
            (filepath + "/front.png"),
        };
    }
   
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(skyboxVertices, sizeof(skyboxVertices));

    //m_IndexBuffer = std::make_unique<IndexBuffer>(skyboxIndices, sizeof(skyboxIndices));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    m_VAO->AddBuffer(*m_VBO, layout);
    LoadFromFile(faces, is32Bits);
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

void Skybox::LoadFromFile(std::vector<std::string> faces, bool is32Bits)
{
    GLCall(glGenTextures(1, &m_RenderID));

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data;
        if (!is32Bits)
        {
            data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        }
        else 
        {
            data = reinterpret_cast<unsigned char*>( stbi_loadf(faces[i].c_str(), &width, &height, &nrComponents, 0) );
        }

        if (data)
        {
            GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RenderID));
            if(!is32Bits)
            {
                GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
            }
            else
            {
                //glTextureStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, data);
                GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, data));
            }
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}