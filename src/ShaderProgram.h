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

	bool CreateProgram(std::string vert, std::string frag);
	void DeleteShaders();
	void DeleteProgram();

	void SetUniform(const GLchar* name, const glm::vec2& vec);
	void SetUniform(const GLchar* name, const glm::vec3& vec);
	void SetUniform(const GLchar* name, const glm::vec4& vec);
	void SetUniform(const GLchar* name, const GLfloat& val);
	void SetUniform(const GLchar* name, const glm::mat4& mat);
	void SetUniformSampler(const GLchar* name, const GLint& slot);
	void Use();

	GLuint GetProgram() const;
	GLint GetUniformLocation(const GLchar* name);
	
private:
	bool LoadShaders(std::string vert, std::string frag);
	bool LoadSpecificShader(const std::string, ShaderType, GLuint);

	std::map<std::string, GLint> m_UniformLocations;
};

