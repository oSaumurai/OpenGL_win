#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

#include <iostream>
VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));	
}

VertexArray::~VertexArray()
{
	std::cout << "VAO deleted" << std::endl;
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
	//can't clean buffer since glswapbuffer would need the data
	//GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i););
		GLCall(glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count* VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
	GLCall(glBindVertexArray(0));
}

