#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>

Mesh::Mesh()
	:mLoaded(false)
{

}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
}

bool Mesh::LoadObj(const std::string& filename)
{

	if (filename.find(".obj") == std::string::npos)
	{
		return false;
	}

	std::fstream stream;

	stream.open(filename, std::ios::in|std::ios::binary);

	if (!stream.is_open())
	{
		std::cerr << "Cannot not load obj file/n";
		return false;
	}

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textCoords;
	std::vector<unsigned int> vertexIndeces, textureIndeces;
	

	std::string lineBuffer;
	while (std::getline(stream, lineBuffer))
	{
		//vertices
		if (lineBuffer.substr(0, 2) == "v ")
		{
			std::istringstream v(lineBuffer.substr(2));
			glm::vec3 vertex;
			v >> vertex.x;
			v >> vertex.y;
			v >> vertex.z;
			vertices.push_back(vertex);
		}
		else if (lineBuffer.substr(0, 2) == "vt")
		{
			std::istringstream vt(lineBuffer.substr(3));
			glm::vec2 tc;
			vt >> tc.s;
			vt >> tc.t;
			textCoords.push_back(tc);
		}
		else if (lineBuffer.substr(0, 2) == "f ")
		{
			unsigned int p1, p2, p3; //mesh indeces
			unsigned int t1, t2, t3; //texture indeces
			unsigned int n1, n2, n3; //normals indeces

			const char* face = lineBuffer.c_str();

			int match = sscanf_s(face, "f %i/%i/%i %i/%i/%i %i/%i/%i",
				&p1, &t1, &n1,
				&p2, &t2, &n2,
				&p3, &t3, &n3);
			if (match != 9)
			{
				std::cerr << "Face format of the" << filename << " is incorrect" << match << std::endl;
				return false;
			}

			vertexIndeces.push_back(p1);
			vertexIndeces.push_back(p2);
			vertexIndeces.push_back(p3);

			textureIndeces.push_back(t1);
			textureIndeces.push_back(t2);
			textureIndeces.push_back(t3);

		}
	}
	stream.close();

	for (unsigned int i = 0; i < vertexIndeces.size(); i++) 
	{
		Vertex meshVertex;

		meshVertex.position = vertices[vertexIndeces[i] - 1];
		meshVertex.textCoords = textCoords[textureIndeces[i] - 1];

		mVertices.push_back(meshVertex);
	}

	InitBuffers();

	return (mLoaded = true);
};

void Mesh::InitBuffers()
{
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW); 

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Mesh::Draw()
{
	if (!mLoaded)
	{
		return;
	}
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
	glBindVertexArray(0);
};