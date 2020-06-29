#pragma once
class TextureFactory
{
public:
	static TextureFactory* getInstance();
	~TextureFactory();

	void LoadTexture();
private:
	static TextureFactory* instance;
	TextureFactory();
};

