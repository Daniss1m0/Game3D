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
		// Reference ID of the Vertex Buffer Object
		GLuint ID;
		// Constructor that generates a Vertex Buffer Object and links it to vertices
		VBO(GLfloat* vertices, GLsizeiptr size);

		// Binds the VBO
		void Bind();
		// Unbinds the VBO
		void Unbind();
		// Deletes the VBO
		void Delete();
	};

	class VAO
	{
	public:
		// ID reference for the Vertex Array Object
		GLuint ID;
		// Constructor that generates a VAO ID
		VAO();

		// Links a VBO to the VAO using a certain layout
		void LinkVBO(VBO& VBO, GLuint layout);
		// Binds the VAO
		void Bind();
		// Unbinds the VAO
		void Unbind();
		// Deletes the VAO
		void Delete();

		GLuint GetId() const;
	};

	class EBO
	{
	public:
		// ID reference of Elements Buffer Object
		GLuint ID;
		// Constructor that generates a Elements Buffer Object and links it to indices
		EBO(GLuint* indices, GLsizeiptr size);

		// Binds the EBO
		void Bind();
		// Unbinds the EBO
		void Unbind();
		// Deletes the EBO
		void Delete();

		GLsizei GetCount() const;
		EBO();
	};
}