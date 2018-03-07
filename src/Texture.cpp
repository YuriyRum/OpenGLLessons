#include "Texture.h"
#include "stb/stb_image.h"
#include <iostream>

#define X_DIRECTION
#define Y_DIRECTION

Texture::Texture() : m_Texture(0)
{
}


Texture::~Texture()
{
	glDeleteTextures(1, &m_Texture);
}

bool Texture::LoadTexture(const std::string& name, bool generateMipMaps)
{
	int width, height, components;

	unsigned char* imageData = stbi_load(name.c_str(), &width, &height, &components, STBI_rgb_alpha);
	if (imageData == NULL)
	{
		std::cerr << "Error loading texture " << name << std::endl;
		return false;
	}		

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	glTextureParameteri(GL_TEXTURE_2D, X_DIRECTION GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, Y_DIRECTION GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	if (generateMipMaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void Texture::Bind(GLuint unitText)
{
	glBindTexture(GL_TEXTURE_2D, m_Texture);
}
