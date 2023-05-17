#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Renderer.h"
#include "Shader.h"

Shader::Shader() {
	m_RendererID = glCreateProgram();
}

Shader::~Shader() {
	GLCall(glDeleteProgram(m_RendererID));
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source) {
	unsigned int id = glCreateShader(type); //  create the vertex shader
	const char * src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int res;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &res));
	if (res == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char *)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader"
				  << "\n";
		std::cout << message << "\n";
		glDeleteShader(id);
		return -1;
	}

	return id;
}

void Shader::addShader(const std::string &filepath, unsigned int type) {

	std::ifstream stream(filepath);

	std::string shader;
	shader = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());

	unsigned int shd = CompileShader(type, shader); //  create the vertex shader

	GLCall(glAttachShader(m_RendererID, shd));
	GLCall(glLinkProgram(m_RendererID));
	GLCall(glValidateProgram(m_RendererID));

	GLCall(glDeleteShader(shd));
}

void Shader::SetUniform1i(const std::string name, int value) {
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string name, float value) {
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string name, float v0, float v1, float v2, float v3) {
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string name, const glm::mat4 &matrix) {
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string &name) {
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}

	GLCall(int loc = glGetUniformLocation(m_RendererID, name.c_str()));
	if (loc == -1) {
		std::cout << "Warning: Uniform '" << name << "' doesn't exist!" << std::endl;
	}
	m_UniformLocationCache[name] = loc;

	return loc;
}
