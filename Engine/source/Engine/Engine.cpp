#include "engine.h"

namespace eng
{
	Engine::Engine(std::uint32_t width, std::uint32_t height, const std::string& title)
		: m_Camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f))
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

		// ---------------------------------------------
		//		Domyślne shadery
		// ---------------------------------------------
		Shader shaderProgram("shaders/test/vertex.glsl", "shaders/test/fragment.glsl");
		Shader lightShader("shaders/test/lightvertex.glsl", "shaders/test/lightfragment.glsl");

		glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec3 lightPos = glm::vec3(0.0f, 1.0f, 0.0f); //zmieniac z sloncem!
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);

		glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 pyramidModel = glm::mat4(1.0f);
		pyramidModel = glm::translate(pyramidModel, pyramidPos);

		lightShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		shaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

		glEnable(GL_DEPTH_TEST);

		// ---------------------------------------------
		//		Tworzenie obiektów gry 
		// ---------------------------------------------

		glm::mat4x3 positions;
		positions[0] = glm::vec3(-1.0f, 0.0f, 1.0f);
		positions[1] = glm::vec3(-1.0f, 0.0f, -1.0f);
		positions[2] = glm::vec3(1.0f, 0.0f, -1.0f);
		positions[3] = glm::vec3(1.0f, 0.0f, 1.0f);

		Map map(positions, shaderProgram);

		glm::fvec3 pos(0.2f, 0.2f, 0.2f);
		glm::fvec3 pos2(0.1f, 0.2f, 0.1f);
		glm::fvec3 posSun(0.0f, 1.0f, 0.0f);
		//Cube cube(pos);
		//cube.Move(glm::vec3(0.5f, 0.0f, 0.5f));

		Budynek bud1(pos2, shaderProgram);
		//bud1.Move(glm::vec3(0.3f, 0.0f, 0.3f));
		//bud1.Move(glm::vec3(0.5f, 0.0f, 0.5f));
		//Triangle triangle2(positions);
		//glm::fvec3 offset(1.0f, 0.0f, 0.0f);
		//triangle2.Move(offset);

		//Piramid piramid(pos2);
		//piramid.Move(offset);

		Sun sun(posSun);

		//sun.Move(offset);

		Renderer* renderer = Renderer::Get();

		float sunRotationAngle = 0.0f;
		float sunRotationSpeed = 0.01f;
		float sunCircleRadius = 0.01f;
		float a = 0.01f;
		float b = 0.01f;

		while (!m_Window.ShouldClose())
		{
			// ---------------------------------------------
			//		Logika
			// ---------------------------------------------
			m_Window.HandleEvents();
			glm::vec3 newPos = glm::vec3(-cos(sunRotationAngle) * sunCircleRadius, -sin(sunRotationAngle) * sunCircleRadius, 0.0f);
			//glm::vec3 newPos = glm::vec3(a * cos(sunRotationAngle), b * sin(sunRotationAngle), 0.000f);
			sun.Move(newPos);
			sunRotationAngle += sunRotationSpeed;
			lightPos += newPos;
			lightModel = glm::translate(lightModel, newPos);
			//lightModel = glm::translate(glm::mat4(1.0f), lightPos);
			glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
			glUniform3f(glGetUniformLocation(lightShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

			// ---------------------------------------------
			//		Rysowanie
			// ---------------------------------------------
			renderer->Clear(ENG_CLEAR_COLOR * 1.0f);

			m_Camera.Inputs(m_Window.GetWindow());
			// Updates and exports the camera matrix to the Vertex Shader
			m_Camera.updateMatrix(45.0f, 0.1f, 100.0f);

			shaderProgram.Activate();

			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
			glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

			glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), m_Camera.Position.x, m_Camera.Position.y, m_Camera.Position.z);

			m_Camera.Matrix(shaderProgram, "camMatrix");

			//TODO: Optymalizowac wywoływanie obiektów. Ulepszyc metode Draw(renderer).

			bud1.Draw();
			map.Draw();

			lightShader.Activate();
			// Export the camMatrix to the Vertex Shader of the light cube
			m_Camera.Matrix(lightShader, "camMatrix");

			sun.Draw();

			m_Window.SwapBuffers();
		}
	}
	/*
	void Engine::SetFPS(std::uint32_t fps)
	{
		std::cout << "FPS set to: " << fps << std::endl;
		m_Fps = fps;
	}
	*/
}