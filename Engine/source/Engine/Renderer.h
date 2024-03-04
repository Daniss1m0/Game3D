#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glfw/glfw3.h>


namespace eng
{
	class Renderer
	{
	public:
		Renderer(const Renderer&) = delete;
		Renderer& operator=(Renderer&) = delete;

		static Renderer* Get();

		void Clear(const glm::fvec4& color);

		void Draw(std::uint32_t vao);
	private:
		Renderer() = default;
	};

}
