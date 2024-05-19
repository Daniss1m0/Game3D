#pragma once

#include <cstdint>
#include <array>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace eng::inter // engine::internal
{
	class VBO
	{
	public:
		GLuint ID;
		VBO(GLfloat* vertices, GLsizeiptr size);
		~VBO();

		void Bind();
		void Unbind();

		void BufferData(GLfloat* vertices, GLsizeiptr size);
	};

	class VAO
	{
	public:
		GLuint ID;
		VAO();
		~VAO();

		void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
		void Bind();
		void Unbind();

		GLuint GetId() const;
	};

	class EBO
	{
	public:
		GLuint ID;
		EBO(GLuint* indices, GLsizeiptr size);
		~EBO();

		void Bind();
		void Unbind();

		void BufferData(GLuint* indices, GLsizeiptr size);
		GLsizei GetCount() const;
	};
}