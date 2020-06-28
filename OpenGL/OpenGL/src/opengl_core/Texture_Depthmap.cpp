#include "Texture_Depthmap.h"

Texture_Depthmap::Texture_Depthmap()
	:id(0)
{
	GLCall(glGenFramebuffers(1, &id));

}

Texture_Depthmap::~Texture_Depthmap()
{

}

void Texture_Depthmap::Bind()
{
}

void Texture_Depthmap::UnBind()
{
}
