#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <memory>
#include <vector>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Engine/Window.h>
#include <Engine/Renderer.h>
#include <Engine/Shader.h>
#include <Engine/GameObjects.h>
#include <Engine/Camera.h>
#include <Engine/Grid.h>

#define ENG_CLEAR_COLOR glm::fvec4(0.1f, 0.1f, 0.1f, 1.0f)

namespace eng
{
	class Engine
	{
	public:
		Engine(std::uint32_t width, std::uint32_t height, const std::string& title);
		Engine() = default;
		~Engine();

		void Run();
	
	private:
		Window m_Window;
		Camera m_Camera;

		int balance = 10000; //licznik pieniedzy
	};
}