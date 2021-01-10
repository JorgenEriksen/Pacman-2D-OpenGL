/**
*   Main file for our Pacman Game. This project is mainly a OpenGL project.
*	There are 3 external libraries that help support this application.
*   Currently there are four ghosts and one pacman with one single map.
*	It is possible to add more levels as that code is dynamic in that regard.
*
*  @name Pacman.exe
*  @author(s) Elvis Arifagic, Jørgen Eriksen, Salvador Bascanunan.
*/

#include "Texture.h"
#include "stb_image.h"

/**
*   Takes in an image and creates a texture. 
*	using external library to read in image.
*
*  @param     path - the path for the .png file.
*  @return    A struct that makes up the entire texture data.
*/
TextureStruct Texture::create(const std::string& path)
{
	TextureStruct result;

	unsigned int rendererId = 0;
	std::string FilePath = path;
	unsigned char* LocalBuffer = nullptr;

	auto Width = 0;
	auto Height = 0;
	auto BPP = 0;

	glGenTextures(1, &rendererId);

	stbi_set_flip_vertically_on_load(1);
	LocalBuffer = stbi_load(path.c_str(), &Width, &Height, &BPP, 4);

	result.Width = Width;
	result.Height = Height;
	result.BPP = BPP;
	result.ID = rendererId;

	
	glBindTexture(GL_TEXTURE_2D, rendererId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (LocalBuffer)
	{
		stbi_image_free(LocalBuffer);
	}

	return result;
}

/**
*   Placeholder constructor.
* 
*/
Texture::Texture()
{

}


/**
*   Applys texture at the correct slot.
* 
*	@param slot - the place on the asset you wish to load in.  
*/
void TextureStruct::apply(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, ID);
}
