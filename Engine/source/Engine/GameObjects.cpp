#include <Engine/GameObjects.h>

namespace eng
{
	Triangle::Triangle(const GLfloat* vertices, GLsizeiptr verticesSize)
		: m_VAO(), m_VBO(const_cast<GLfloat*>(vertices), verticesSize), m_EBO()
	{
		m_VAO.Bind();
		m_VAO.LinkVBO(m_VBO, 0);
		m_VAO.Unbind();
		m_VBO.Unbind();
	}
	
	Triangle::Triangle(const GLfloat* vertices, GLsizeiptr verticesSize, const GLuint* indices, GLsizeiptr indicesSize)
		: m_VAO(), m_VBO(const_cast<GLfloat*>(vertices), verticesSize), m_EBO(const_cast<GLuint*>(indices), indicesSize) 
	{
		m_VAO.Bind();
		m_VAO.LinkVBO(m_VBO, 0);
		m_VAO.Unbind();
		m_VBO.Unbind();
		m_EBO.Unbind();
	}

	Triangle::~Triangle() 
	{
		m_VAO.Delete();
		m_VBO.Delete();
		m_EBO.Delete();
	}

	void Triangle::Draw(Renderer* renderer)
	{
		renderer->Draw(m_VAO.GetId());
	}

	void Triangle::Draw_with_Indices()
	{
		m_VAO.Bind();
		glDrawElements(GL_TRIANGLES, m_EBO.GetCount(), GL_UNSIGNED_INT, 0);
		m_VAO.Unbind();
	}

}