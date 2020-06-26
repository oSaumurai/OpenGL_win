#include "TextureFactory.h"

TextureFactory* TextureFactory::instance = nullptr;

TextureFactory* TextureFactory::getInstance()
{
    if (!instance)
    {
        instance = new TextureFactory();
    }
    return instance;
}

TextureFactory::~TextureFactory()
{
}

void TextureFactory::LoadTexture()
{
}
