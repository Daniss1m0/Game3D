#include "buffers.h"

namespace eng::inter
{
	VBO::VBO(GLfloat* vertices, GLsizeiptr size)
	{
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	// Binds the VBO
	void VBO::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}

	// Unbinds the VBO
	void VBO::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Deletes the VBO
	void VBO::Delete()
	{
		glDeleteBuffers(1, &ID);
	}

	VAO::VAO()
	{
		glGenVertexArrays(1, &ID);
	}

	// Links a VBO to the VAO using a certain layout
	void VAO::LinkVBO(VBO& VBO, GLuint layout)
	{
		VBO.Bind();
		glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(layout);
		VBO.Unbind();
	}

	// Binds the VAO
	void VAO::Bind()
	{
		glBindVertexArray(ID);
	}

	// Unbinds the VAO
	void VAO::Unbind()
	{
		glBindVertexArray(0);
	}

	// Deletes the VAO
	void VAO::Delete()
	{
		glDeleteVertexArrays(1, &ID);
	}

	EBO::EBO(GLuint* indices, GLsizeiptr size)
	{
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	// Binds the EBO
	void EBO::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	}

	// Unbinds the EBO
	void EBO::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Deletes the EBO
	void EBO::Delete()
	{
		glDeleteBuffers(1, &ID);
	}

	GLsizei EBO::GetCount() const
	{
		GLsizei count;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &count);
		return count / sizeof(GLuint);
	}


	
	EBO::EBO()
	{
		glGenBuffers(1, &ID);
	}


	GLuint VAO::GetId() const
	{
		return ID;
	}
}