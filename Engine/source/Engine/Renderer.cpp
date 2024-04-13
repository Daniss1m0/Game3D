#include "Renderer.h"

namespace eng
{
	Renderer* Renderer::Get()
	{
		static Renderer* instance = new Renderer();
		return instance;
	}

	void Renderer::Clear(const glm::fvec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::Draw(GLuint vao)
	{
		glBindVertexArray(vao);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}

}