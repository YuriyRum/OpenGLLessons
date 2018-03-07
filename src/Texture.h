#pragma once

#include "GL/glew.h"
#include <string>

class Texture
{
public:
	Texture();
	virtual ~Texture();
	bool LoadTexture(const std::string& name, bool generateMipMaps);
	void Bind(GLuint unitText = 0);
private:
	GLuint m_Texture;
};

