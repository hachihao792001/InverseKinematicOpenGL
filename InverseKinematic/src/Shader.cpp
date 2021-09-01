#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>

#include "Renderer.h"

Shader::Shader(const std::string& filePath) : m_FilePath(filePath), m_RendererID(0)
{
	ShaderProgramSource source = ReadShader(filePath);
	m_RendererID = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ReadShader(const std::string& filePath) {
	std::fstream in(m_FilePath, std::ios::in);
	enum class ShaderMode {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderMode currentMode;
	ShaderProgramSource sources;
	sources.vertexSource = "";
	sources.fragmentSource = "";

	while (!in.eof()) {
		std::string currentLine;
		std::getline(in, currentLine);

		if (currentLine.substr(0, 7) == "#shader") {
			if (currentLine.substr(8) == "vertex")
				currentMode = ShaderMode::VERTEX;
			else
				currentMode = ShaderMode::FRAGMENT;
		}
		else {
			if (currentMode == ShaderMode::VERTEX)
				sources.vertexSource += currentLine + "\n";
			else
				sources.fragmentSource += currentLine + "\n";
		}
	}

	in.close();
	return sources;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int shader = glCreateShader(type);

	const char* sourceChar = source.c_str();
	glShaderSource(shader, 1, &sourceChar, nullptr);
	glCompileShader(shader);

	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(shader, length, &length, message);
		std::cout << "Fail to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;

		delete[]message;
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

unsigned int Shader::CreateShader(const std::string& vertexSource, const std::string& fragmentSource) {
	unsigned int program = glCreateProgram();
	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform1f(const std::string& name, float v0)
{
	GLCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform1iv(const std::string& name, const int& count, const int* arr)
{
	GLCall(glUniform1iv(GetUniformLocation(name), count, arr));
}
