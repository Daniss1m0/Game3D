#include "buffers.h"

namespace eng::inter
{
	VBO::VBO(GLfloat* vertices, GLsizeiptr size)
	{
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VBO::~VBO()
	{
		glDeleteBuffers(1, &ID);
	}

	void VBO::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}

	void VBO::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VBO::BufferData(GLfloat* vertices, GLsizeiptr size)
	{
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VAO::VAO()
	{
		glGenVertexArrays(1, &ID);
	}

	VAO::~VAO()
	{
		glDeleteVertexArrays(1, &ID);
	}

	void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
	{
		VBO.Bind();
		glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(layout);
		VBO.Unbind();
	}

	void VAO::Bind()
	{
		glBindVertexArray(ID);
	}

	void VAO::Unbind()
	{
		glBindVertexArray(0);
	}

	EBO::EBO(GLuint* indices, GLsizeiptr size)
	{
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	EBO::~EBO()
	{
		glDeleteBuffers(1, &ID);
	}

	void EBO::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	}

	void EBO::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


	void EBO::BufferData(GLuint* indices, GLsizeiptr size)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	GLsizei EBO::GetCount() const
	{
		GLsizei count;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &count);
		return count / sizeof(GLuint);
	}

	GLuint VAO::GetId() const
	{
		return ID;
	}
}