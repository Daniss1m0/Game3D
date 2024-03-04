#pragma once

#include <cstdint>
#include <string>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace eng
{
	class Window
	{
	public:
		Window(std::uint32_t width, std::uint32_t height, const std::string& title);
		Window() = default;

		inline bool ShouldClose() const { return glfwWindowShouldClose(m_Window); }

		void HandleEvents();

		void SetCurrent();

		void SwapBuffers();

		GLFWwindow* GetWindow() const {
			return m_Window;
		}

	private:
		std::uint32_t m_Width = 0, m_Height = 0;

		std::string m_Title = "";

		GLFWwindow* m_Window = nullptr;
	};

}