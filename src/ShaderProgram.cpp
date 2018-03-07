#include "ShaderProgram.h"
#include "Loader.h"
#include "iostream"

ShaderProgram::ShaderProgram() : m_VertexShader(glCreateShader(GL_VERTEX_SHADER)), m_FragmentShader(glCreateShader(GL_FRAGMENT_SHADER)), m_Program(glCreateProgram())
{
}


ShaderProgram::~ShaderProgram()
{
	DeleteShaders();
	DeleteProgram();
}

bool ShaderProgram::CreateProgram()
{
	if (!LoadShaders())
	{
		std::cerr << "Error occured during shaders compilation" << std::endl;
		return false;
	};
	
	glAttachShader(m_Program, m_VertexShader);
	glAttachShader(m_Program, m_FragmentShader);
	glLinkProgram(m_Program);

	GLint result;
	GLchar log[512];

	glGetProgramiv(m_Program, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(m_Program, sizeof(log), NULL, log);
		std::cerr << "Program fialed to link " << log << std::endl;
		return false;
	}
	return true;
}

void ShaderProgram::DeleteShaders()
{
	GLint param;
	glGetShaderiv(m_VertexShader, GL_DELETE_STATUS, &param);
	if (param != GL_TRUE)
	{
		glDeleteShader(m_VertexShader);
	}	

	glGetShaderiv(m_FragmentShader, GL_DELETE_STATUS, &param);
	if (param != GL_TRUE)
	{
		glDeleteShader(m_FragmentShader);
	}	
}

void ShaderProgram::DeleteProgram()
{
	GLint param;

	glGetProgramiv(m_Program, GL_DELETE_STATUS, &param);

	if (param != GL_TRUE)
	{
		glDeleteProgram(m_Program);
	}	
}

GLuint ShaderProgram::GetProgram() const{ return m_Program; }; 

GLint ShaderProgram::GetUniformLocation(const GLchar* name) 
{ 
	std::map<std::string, GLint>::iterator it = m_UniformLocations.find(name); 

	if (it == m_UniformLocations.end())
	{
		m_UniformLocations[name] = glGetUniformLocation(m_Program, name);
	}

	return m_UniformLocations[name];
};

void ShaderProgram::SetUniform(const GLchar* name, const glm::vec2& vec)
{
	GLint uniformLocation = GetUniformLocation(name);
	glUniform2f(uniformLocation, vec.x, vec.y);
};

void ShaderProgram::SetUniform(const GLchar* name, const glm::vec3& vec)
{
	GLint uniformLocation = GetUniformLocation(name);
	glUniform3f(uniformLocation, vec.x, vec.y,vec.z);
};

void ShaderProgram::SetUniform(const GLchar* name, const glm::vec4& vec)
{
	GLint uniformLocation = GetUniformLocation(name);
	glUniform4f(uniformLocation, vec.x, vec.y, vec.z, vec.w);
};

void ShaderProgram::Use()
{
	glUseProgram(m_Program);
}

bool ShaderProgram::LoadShaders()
{
	bool returnStatus = true;
	returnStatus &= LoadSpecificShader(".\\bin\\shader.vert", ShaderType::VERTEX, m_VertexShader);
	returnStatus &= LoadSpecificShader(".\\bin\\shader.frag", ShaderType::FRAGMENT, m_FragmentShader);

	return returnStatus;
}

bool ShaderProgram::LoadSpecificShader(const std::string path, ShaderProgram::ShaderType shaderType, GLuint shaderHandle)
{
	Loader loader;	

	const std::string shaderSource = loader.Load(path);

	std::cout << shaderSource << std::endl;

	const char* const source = shaderSource.c_str();
	
	glShaderSource(shaderHandle, 1, &source, NULL);
	glCompileShader(shaderHandle);

	GLint result;
	GLchar log[512];

	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shaderHandle, sizeof(log), NULL, log);
		std::cerr << log << std::endl;
		return false;
	}
	return true;
}