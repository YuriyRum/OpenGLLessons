#pragma once

#include <vector>
#include <string>
#include "GL/glew.h"
#include "glm/glm.hpp"

std::vector<std::string> split(std::string s, std::string t);

struct Vertex
{
	glm::vec3 position;
	glm::vec2 textCoords;
	//glm::vec3 normal;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool LoadObj(const std::string& filename);
	void Draw();

private:
	void InitBuffers();

	bool mLoaded;
	std::vector<Vertex> mVertices;

	GLuint mVBO, mVAO;
};

