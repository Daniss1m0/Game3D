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

namespace eng
{
	class BaseObject {};

	class Triangle : public BaseObject 
	{
	public:
		Triangle(const GLfloat* vertices, GLsizeiptr verticesSize);
		Triangle(const GLfloat* vertices, GLsizeiptr verticesSize, const GLuint* indices, GLsizeiptr indicesSize);
		~Triangle();

		void Draw(Renderer* renderer);
		void Draw_with_Indices();

	private:
		inter::VAO m_VAO;
		inter::VBO m_VBO;
		inter::EBO m_EBO;
	};
}