#pragma once

#include <glad/glad.h>

#include <string>

#include <fstream>
#include <sstream>

class Shader
{
private:

	void checkCompilerErrors(unsigned int shader, std::string type);

public:

	unsigned int ID;

	Shader() = default;

	Shader(const char* vertexPath, const char* fragmentPath);

	Shader(const char *vertexPath, const char *geometryPath, const char *fragmentPath);

	~Shader();

};