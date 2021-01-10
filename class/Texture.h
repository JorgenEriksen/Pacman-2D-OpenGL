#pragma once

#include <string>

#include "GL/glew.h"

struct TextureStruct
{
	int Width, Height, BPP;
	unsigned int ID;

	void apply(unsigned int slot) const;
};

class Texture
{
private:
public:

	TextureStruct create(const std::string& path);
	Texture();
	~Texture();
};