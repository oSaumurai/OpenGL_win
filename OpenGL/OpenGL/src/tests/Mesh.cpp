#include "Mesh.h"
#include <string>
#include <iostream>

Mesh::Mesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, std::vector<TextureInfo> Textures)
	:vertices(Vertices), indices(Indices), textures(Textures),
    m_Translation(0,0,0),
    m_Proj(glm::perspective(90.0f, 1.0f, 0.1f, 100.0f)),
    m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6.0f)))
{
	SetupMesh();
}


Mesh::~Mesh()
{    
    std::cout << "mesh deleted" << std::endl;
}

void Mesh::Destroy()
{
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        GLCall(glDeleteTextures(1, &textures[i].id));
    }
}

void Mesh::Draw()
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
    glm::mat4 mvp = m_Proj * m_View * model;
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_MVP", mvp);
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        std::string number;
        std::string name = textures[i].type; 
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to stream
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to stream
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to stream
        
        // now set the sampler to the correct texture unit
        m_Shader->SetUniform1i(name+number, i);
        //glBindTexture(GL_TEXTURE_2D, textures[i].id);
        glActiveTexture(GL_TEXTURE0 + i);
        GLCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
    }
    Renderer renderer;
    renderer.Draw(*m_VAO, *m_IndexBuffer,*m_Shader);
    
    m_VAO->UnBind();
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetupMesh()
{
    VertexBufferLayout layout;
    m_VAO = std::make_shared<VertexArray>();
    m_VertexBuffer = std::make_shared<VertexBuffer>(&vertices[0], vertices.size() * sizeof(Vertex));
    m_IndexBuffer = std::make_shared<IndexBuffer>(&indices[0], indices.size());
    m_Shader = std::make_shared<Shader>("res/shader/Assimp.shader");
    //m_VAO->Bind();
    layout.Push<float>(3);
    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(3);
    layout.Push<float>(3);
    m_VAO->AddBuffer(*m_VertexBuffer, layout);
    m_Shader->Bind();
}
