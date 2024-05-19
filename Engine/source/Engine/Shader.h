#pragma once

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

#include <glad/glad.h>

namespace eng
{
	class Shader
	{
	public:
		GLuint ID;
		Shader(const char* vertexFile, const char* fragmentFile);
		~Shader();

		void Activate();

	private:
		void compileErrors(unsigned int shader, const char* type);
	};

	std::string get_file_contents(const char* filename);
}