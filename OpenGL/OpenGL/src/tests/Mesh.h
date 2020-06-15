#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Texture.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct TextureInfo {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<TextureInfo> textures);
	//Mesh(const Mesh& mesh);
	~Mesh();

	void Draw();
	void Destroy();
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureInfo> textures;
	//unsigned int VAO;

private:
	std::shared_ptr<VertexArray> m_VAO;
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	
	glm::vec3 m_Translation;
	glm::mat4 m_Proj;
	glm::mat4 m_View;
	//unsigned int VBO, EBO;
private:

	void SetupMesh();

};

