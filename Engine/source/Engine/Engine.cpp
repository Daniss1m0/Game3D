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
		{ //               COORDINATES                  /     COLORS           //
			-0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower left corner
			 0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower right corner
			 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,     1.0f, 0.6f,  0.32f, // Upper corner
			-0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner left
			 0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner right
			 0.0f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f  // Inner down
		};

		/* ?
		GLfloat vertices[] =
		{ //               COORDINATES                        /     COLORS           //
			positions[0][0], positions[0][1], positions[0][2],     0.8f, 0.3f,  0.02f, // Lower left corner
			positions[1][0], positions[1][1], positions[1][2],     0.8f, 0.3f,  0.02f, // Lower right corner
			positions[2][0], positions[2][1], positions[2][2],     1.0f, 0.6f,  0.32f, // Upper corner
		};
		*/

		GLuint indices[] =
		{
			0, 3, 5, // Lower left triangle
			3, 2, 4, // Upper triangle
			5, 4, 1 // Lower right triangle
		};

		GLfloat verticesSQUARE[] =
		{ //     COORDINATES     /        COLORS      /   TexCoord  //
			-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
			-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
			 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
			 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
		};

		// Indices for vertices order
		GLuint indicesSQUARE[] =
		{
			0, 2, 1, // Upper triangle
			0, 3, 2 // Lower triangle
		};

		GLfloat verticesPIRAMID[] =
		{ //     COORDINATES     /        COLORS      /   TexCoord  //
			-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
			-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
			 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
			 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
			 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
		};

		// Indices for vertices order
		GLuint indicesPIRAMID[] =
		{
			0, 1, 2,
			0, 2, 3,
			0, 1, 4,
			1, 2, 4,
			2, 3, 4,
			3, 0, 4
		};
		/*
		GLfloat verticesCUBEandPIRAMID[] =
		{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
			-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
			-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
			 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
			 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

			-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
			-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
			 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

			-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
			 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
			 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

			 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
			 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
			 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

			 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
			-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
			 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
		};
		*/
		// Indices for vertices order
		GLuint indicesCUBEandPIRAMID[] =
		{
			0, 1, 2, // Bottom side
			0, 2, 3, // Bottom side
			4, 6, 5, // Left side
			7, 9, 8, // Non-facing side
			10, 12, 11, // Right side
			13, 15, 14 // Facing side
		};

		GLfloat lightVertices[] =
		{ //     COORDINATES     //
			-0.1f, -0.1f,  0.1f,
			-0.1f, -0.1f, -0.1f,
			 0.1f, -0.1f, -0.1f,
			 0.1f, -0.1f,  0.1f,
			-0.1f,  0.1f,  0.1f,
			-0.1f,  0.1f, -0.1f,
			 0.1f,  0.1f, -0.1f,
			 0.1f,  0.1f,  0.1f
		};

		GLuint lightIndices[] =
		{
			0, 1, 2,
			0, 2, 3,
			0, 4, 7,
			0, 7, 3,
			3, 7, 6,
			3, 6, 2,
			2, 6, 5,
			2, 5, 1,
			1, 5, 4,
			1, 4, 0,
			4, 5, 6,
			4, 6, 7
		};

		GLfloat verticesMAP[] =
		{ //     COORDINATES     /        COLORS        /    TexCoord    /       NORMALS     //
			-1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
			-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
			 1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
			 1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
		};

		GLuint indicesMAP[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		GLfloat verticesTriangle[] =
		{ //     COORDINATES     /        COLORS        /    TexCoord    /       NORMALS     //
			-1.0f, 0.0f,  1.0f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
			-1.0f, 0.0f, -1.0f,		1.0f, 1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
			 1.0f, 0.0f, -1.0f,		1.0f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f
		};

		GLuint indicesTriangle[] =
		{
			0, 1, 2
		};

		//Triangle triangle1(vertices, sizeof(vertices), indices, sizeof(indices));

		//Triangle square(verticesSQUARE, sizeof(verticesSQUARE), indicesSQUARE, sizeof(indicesSQUARE));

		//Triangle piramid(verticesPIRAMID, sizeof(verticesPIRAMID), indicesPIRAMID, sizeof(indicesPIRAMID));

		//Triangle cube(verticesCUBEandPIRAMID, sizeof(verticesCUBEandPIRAMID), indicesCUBEandPIRAMID, sizeof(indicesCUBEandPIRAMID));

		//Triangle map(verticesMAP, sizeof(verticesMAP), indicesMAP, sizeof(indicesMAP));
		//glm::fvec3 offset(0.1f, 0.2f, -0.3f);
		//map.Move(offset);

		Triangle triangle2(verticesTriangle, sizeof(verticesTriangle), indicesTriangle, sizeof(indicesTriangle));
		//glm::fvec3 offset(1.0f, 0.5f, 1.0f);
		//triangle2.Move(offset);

		//position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		//positions = glm::mat3x4&;
		//Triangle triangle1(glm::mat3x4& positions, glm::fvec4& color);
		//Cube cube1(position, length, color);

		GLfloat vertices2[] =
		{
			0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
			1.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
			1.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
		};

		//Triangle triangle2(vertices2, sizeof(vertices2));
		
		//Texture wall("textures/pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
		//wall.texUnit(shaderProgram, "tex0", 0);

		//Texture brick("textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
		//brick.texUnit(shaderProgram, "tex0", 0);

		//Texture planksTex("textures/brick.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
		//planksTex.texUnit(shaderProgram, "tex0", 0);
		//Texture planksSpec("textures/brick.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
		//planksSpec.texUnit(shaderProgram, "tex1", 1);

		// Shader for light cube
		Shader lightShader("shaders/test/lightvertex.glsl", "shaders/test/lightfragment.glsl");
		// Generates Vertex Array Object and binds it
		inter::VAO lightVAO;
		lightVAO.Bind();
		// Generates Vertex Buffer Object and links it to vertices
		inter::VBO lightVBO(lightVertices, sizeof(lightVertices));
		// Generates Element Buffer Object and links it to indices
		inter::EBO lightEBO(lightIndices, sizeof(lightIndices));
		// Links VBO attributes such as coordinates and colors to VAO
		lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
		// Unbind all to prevent accidentally modifying them
		lightVAO.Unbind();
		lightVBO.Unbind();
		lightEBO.Unbind();

		glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
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

		Camera camera(1360, 768, glm::vec3(0.0f, 0.0f, 2.0f)); //change

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

			camera.Inputs(m_Window.GetWindow());
			// Updates and exports the camera matrix to the Vertex Shader
			camera.updateMatrix(45.0f, 0.1f, 100.0f);

			shaderProgram.Activate();

			glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

			camera.Matrix(shaderProgram, "camMatrix");

			//TODO: Optymalizowac wywoływanie obiektów. Podzialaz z metodą Draw(renderer).

			//wall.Bind();
			//brick.Bind();
			//planksTex.Bind();
			//planksSpec.Bind();
			//triangle1.Draw_with_Indices();
			//triangle2.Draw(renderer);
			//square.Draw_with_Indices();
			//piramid.Draw_with_Indices();
			//cube.Draw_with_Indices();
			//map.Draw();
			triangle2.Draw();

			// Tells OpenGL which Shader Program we want to use
			lightShader.Activate();
			// Export the camMatrix to the Vertex Shader of the light cube
			camera.Matrix(lightShader, "camMatrix");
			// Bind the VAO so OpenGL knows to use it
			lightVAO.Bind();
			// Draw primitives, number of indices, datatype of indices, index of indices
			glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

			m_Window.SwapBuffers();
		}
		//wall.Delete();
		//brick.Delete();
		//planksTex.Delete();
		//planksSpec.Delete();
		lightVAO.Delete();
		lightVBO.Delete();
		lightEBO.Delete();
		lightShader.Delete();
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