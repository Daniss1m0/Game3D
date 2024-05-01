#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <Engine/internal/buffers.h>
#include <Engine/Renderer.h>
#include <Engine/Shader.h>

//TODO: Dziedziczenie. Np. Draw() dla kazdego.

namespace eng
{
	class BaseObject {};

	class Texture : public BaseObject
	{
	public:
		GLuint ID;
		GLenum type;
		GLuint unit;

		Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);
		void texUnit(Shader& shader, const char* uniform, GLuint unit);

		void Bind();
		void Unbind();
		void Delete();
	};

	class Triangle : public BaseObject
	{
	public:
		Triangle(const GLfloat* vertices, GLsizeiptr verticesSize, const GLuint* indices, GLsizeiptr indicesSize);
		~Triangle();

		void Draw();
		void Draw(Renderer* renderer);

		void Move(const glm::fvec3& offset);
		//void Rotate();

	private:
		inter::VAO m_VAO;
		inter::VBO m_VBO;
		inter::EBO m_EBO;
	};

	class Piramid : public BaseObject 
	{
		
	};

	class Cube : public BaseObject
	{
		
	};

	class Budynek : public Cube {};

	class Szkola : public Cube {};

	class Szpital : public Cube {};

	class Map : public BaseObject
	{

	};

	class Sun : public BaseObject
	{

	};
}