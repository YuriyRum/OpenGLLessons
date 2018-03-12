#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"

#include "stb/stb_image.h"
#include <iostream>

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
	
	/// invert image
	InvertImage(imageData, width, height);

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

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
	glActiveTexture(GL_TEXTURE0 + unitText);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
}

void Texture::InvertImage(unsigned char* source, const int width, const int height)
{
	unsigned char* top = nullptr;
	unsigned char* bottom = nullptr;
	unsigned char tmp = 0;	
	int widthInBytes = width * 4;

	int halfHeight = height / 2;

	for (int row = 0; row < halfHeight; row++)
	{
		top = source + row * widthInBytes;
		bottom = source + ( height - row - 1 ) * widthInBytes;

		for (int col = 0; col < widthInBytes; col++)
		{
			tmp = *top;
			*top = *bottom;
			*bottom = tmp;

			top++;
			bottom++;
		}
	}

}
