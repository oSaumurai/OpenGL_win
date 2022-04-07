#pragma once
#include "glm/glm.hpp"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"

#include <memory>

class AbstractObject {

public:
	glm::vec3 Position;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<Texture> m_Texture;

	virtual void Update() = 0;
	virtual void Draw(Shader& shader);

protected:
	void BindTexture(const char* filepath);
	virtual void SetupMesh() = 0;
};