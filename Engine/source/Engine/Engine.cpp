#include "engine.h"

namespace eng
{
	Engine::Engine(std::uint32_t width, std::uint32_t height, const std::string& title)
		//: m_Camera(glm::fvec3(0.0f, 0.0f, 0.0f), 0.0f)
	{
		if (!glfwInit())
			std::cerr << "Failed to initialize GLFW" << std::endl;
		else
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			m_Window = Window(width, height, title);

			m_Window.SetCurrent();

			if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
				std::cerr << "Failed to initialize GLAD" << std::endl;
		}
	}

	Engine::~Engine()
	{
		glfwTerminate();
	}

	void Engine::Run()
	{
		std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;

		Renderer* renderer = Renderer::Get();

		// ---------------------------------------------
		//		Shadery
		// ---------------------------------------------
		Shader shaderProgram("shaders/test/vertex.glsl", "shaders/test/fragment.glsl");

		GLfloat vertices[] =
		{
			-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
			0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
			0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
			-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
			0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
			0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
		};

		GLuint indices[] =
		{
			0, 3, 5, // Lower left triangle
			3, 2, 4, // Upper triangle
			5, 4, 1 // Lower right triangle
		};

		Triangle triangle1(vertices, sizeof(vertices), indices, sizeof(indices));

		GLfloat vertices2[] =
		{
			0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
			1.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
			1.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
		};

		Triangle triangle2(vertices2, sizeof(vertices2));



		while (!m_Window.ShouldClose())
		{
			// ---------------------------------------------
			//		Logika
			// ---------------------------------------------
			m_Window.HandleEvents();

			// if (glfwGetKey(m_Window.GetWindow(), GLFW_KEY_P) == GLFW_PRESS)
			
			// ---------------------------------------------
			//		Rysowanie
			// ---------------------------------------------
			renderer->Clear(ENG_CLEAR_COLOR * 1.0f);

			shaderProgram.Activate();

			triangle1.Draw_with_Indices();
			triangle2.Draw(renderer);


			m_Window.SwapBuffers();
		}

		shaderProgram.Delete();
	}
	/*
	void Engine::SetFPS(std::uint32_t fps)
	{
		std::cout << "FPS set to: " << fps << std::endl;
		m_Fps = fps;
	}
	*/
}