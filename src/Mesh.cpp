#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>

std::vector<std::string> split(std::string s, std::string t)
{
	std::vector<std::string> res;

	while (1)
	{
		int pos = s.find(t);
		if (pos == -1)
		{
			res.push_back(s);
			break;
		}
		res.push_back(s.substr(0, pos));
		s = s.substr(pos + 1 , s.size() - pos - 1);
	}
	return res;
}

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
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> vertexIndeces, textureIndeces, normalIndeces;
	

	std::string lineBuffer;
	while (std::getline(stream, lineBuffer))
	{
		std::stringstream ss(lineBuffer);
		std::string cmd;

		ss >> cmd;
		//vertices
		if (cmd == "v")
		{			
			glm::vec3 vertex;
			int dim = 0;
			while (dim < 3 && ss >> vertex[dim])
			{
				dim++;
			}

			vertices.push_back(vertex);
		}
		else if (cmd == "vt")
		{			
			glm::vec2 uv;
			int dim = 0;
			while (dim < 2 && ss >> uv[dim])
			{
				dim++;
			}
			textCoords.push_back(uv);
		}
		else if (cmd == "vn")
		{
			glm::vec3 norm;
			int dim = 0;
			while (dim < 3 && ss >> norm[dim])
			{
				dim++;
			}

			norm = glm::normalize(norm);
			normals.push_back(norm);
		}
		else if (cmd == "f")
		{			
			std::string faceData;
			int vertexIndex, uvIndex, normalIndex; 

			while (ss >> faceData)
			{
				std::vector<std::string> data = split(faceData, "/");

				if (data[0].size() > 0)
				{
					sscanf_s(data[0].c_str(), "%d", &vertexIndex);
					vertexIndeces.push_back(vertexIndex);
				}
				if (data.size() >= 1)
				{
					if (data[1].size() > 0)
					{
						sscanf_s(data[1].c_str(), "%d", &uvIndex);
						textureIndeces.push_back(uvIndex);
					}
					if (data.size() >= 2 && data[2].size() > 0)
					{
						sscanf_s(data[2].c_str(), "%d", &normalIndex);
						normalIndeces.push_back(normalIndex);
					}
				}
			}
		}
	}
	stream.close();	

	for (unsigned int i = 0; i < vertexIndeces.size(); i++) 
	{
		Vertex meshVertex;
		
		if (vertices.size() > 0)
		{
			meshVertex.position = vertices[vertexIndeces[i] - 1];
		}
		
		if (normals.size() > 0)
		{
			meshVertex.normal = normals[normalIndeces[i] - 1];
		}
		
		if (textCoords.size() > 0)
		{
			meshVertex.textCoords = textCoords[textureIndeces[i] - 1];
		}		

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

	// Vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(0));
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Face attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

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