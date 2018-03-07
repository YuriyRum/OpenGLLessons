#pragma once

#include "GL/glew.h"
#include <string>
#include "glm/glm.hpp"
#include <map>

class ShaderProgram
{
private:
	GLuint m_Program;
	GLuint m_VertexShader;
	GLuint m_FragmentShader;

public:
	enum class ShaderType
	{
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

	ShaderProgram();
	~ShaderProgram();

	bool CreateProgram();
	void DeleteShaders();
	void DeleteProgram();

	void SetUniform(const GLchar* name, const glm::vec2& vec);
	void SetUniform(const GLchar* name, const glm::vec3& vec);
	void SetUniform(const GLchar* name, const glm::vec4& vec);
	void Use();

	GLuint GetProgram() const;
	GLint GetUniformLocation(const GLchar* name);
	
private:
	bool LoadShaders();
	bool LoadSpecificShader(const std::string, ShaderType, GLuint);

	std::map<std::string, GLint> m_UniformLocations;
};

