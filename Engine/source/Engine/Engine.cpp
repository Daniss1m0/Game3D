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

		std::vector<BaseObject*> UpdatableVector;

		glm::mat4x3 positions;
		positions[0] = glm::vec3(-1.0f, 0.0f, 1.0f);
		positions[1] = glm::vec3(-1.0f, 0.0f, -1.0f);
		positions[2] = glm::vec3(1.0f, 0.0f, -1.0f);
		positions[3] = glm::vec3(1.0f, 0.0f, 1.0f);

		Map* map = new Map(positions, shaderProgram);
		UpdatableVector.push_back(map);

		glm::fvec3 pos(0.7f, 0.0f, 0.3f);
		glm::fvec3 pos2(0.1f, 0.1f, 0.3f);
		glm::fvec3 posSun(0.0f, 1.0f, 0.0f);

		glm::fvec3 posSklep(-0.5f, 0.05f, -0.5f);
		glm::fvec3 posSklep1(-0.35f, 0.05f, -0.5f);
		glm::fvec3 posSklep2(-0.20f, 0.05f, -0.5f);
		glm::fvec3 posSklep3(-0.05f, 0.05f, -0.5f);
		glm::fvec3 posSklep4(0.1f, 0.05f, -0.5f);

		glm::fvec3 posBlok(-0.5f, 0.075f, -0.2f);
		glm::fvec3 posBlok1(-0.35f, 0.075f, -0.2f);
		glm::fvec3 posBlok2(-0.20f, 0.075f, -0.2f);
		glm::fvec3 posBlok3(-0.05f, 0.075f, -0.2f);
		glm::fvec3 posBlok4(0.1f, 0.075f, -0.2f);

		//Cube cube(pos);
		//cube.Move(glm::vec3(0.5f, 0.0f, 0.5f));

		//Budynek bud1(pos2);

		//Sklep sklep1(posSklep);

		//Blok blok1(posBlok);

		//bud1.Move(glm::vec3(0.3f, 0.0f, 0.3f));
		//bud1.Move(glm::vec3(0.5f, 0.0f, 0.5f));
		//Triangle triangle2(positions);
		//glm::fvec3 offset(1.0f, 0.0f, 0.0f);
		//triangle2.Move(offset);

		//Piramid piramid(pos);
		//piramid.Move(offset);

		Sun sun(posSun);

		//sun.Move(offset);

		Renderer* renderer = Renderer::Get();

		float sunRotationAngle = 0.0f;
		float sunRotationSpeed = 0.01f;
		float sunCircleRadius = 0.01f;
		float a = 0.01f;
		float b = 0.01f;

		// Initialize ImGUI
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(m_Window.GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");

		bool buySklep = false;
		bool buyBudynek = false;
		bool buyBlok = false;
		glm::vec3 newHousePosition(0.0f, 0.05f, 0.0f);

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
			glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
			glUniform3f(glGetUniformLocation(lightShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

			if (buySklep && balance >= 100)
			{
				Sklep* newSklep = new Sklep(newHousePosition);
				UpdatableVector.push_back(newSklep);
				newHousePosition.x += 0.2f;
				balance -= 100;
				buySklep = false;
			}
			if (buyBudynek && balance >= 200)
			{
				Budynek* newBudynek = new Budynek(newHousePosition);
				UpdatableVector.push_back(newBudynek);
				newHousePosition.x += 0.2f;
				balance -= 200;
				buyBudynek = false;
			}
			if (buyBlok && balance >= 300)
			{
				Blok* newBlok = new Blok(newHousePosition);
				UpdatableVector.push_back(newBlok);
				newHousePosition.x += 0.2f;
				balance -= 300;
				buyBlok = false;
			}

			// ---------------------------------------------
			//		Rysowanie
			// ---------------------------------------------
			renderer->Clear(ENG_CLEAR_COLOR * 1.0f);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (!ImGui::IsAnyItemActive() && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
				m_Camera.Inputs(m_Window.GetWindow());
			}

			m_Camera.updateMatrix(45.0f, 0.1f, 100.0f);

			shaderProgram.Activate();
			m_Camera.Matrix(shaderProgram, "camMatrix");

			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
			glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

			glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), m_Camera.Position.x, m_Camera.Position.y, m_Camera.Position.z);

			//Wywoływanie obiektów.
			for (const auto& object : UpdatableVector)
				object->Draw();


			ImGui::Begin("My name is window, ImGUI window");
			// Text that appears in the window
			ImGui::Text("Hello there adventurer!");
			// Checkbox that appears in the window
			//ImGui::Checkbox("Draw Triangle", &drawTriangle);
			// Slider that appears in the window
			//ImGui::SliderFloat("Size", &size, 0.5f, 2.0f);
			// Fancy color editor that appears in the window
			//ImGui::ColorEdit4("Color", color);
			// Ends the window
			ImGui::End();

			ImGui::Begin("Shop");
			ImGui::Text("Purchase your buildings here!");
			ImGui::Separator();

			ImGui::Text("Available Buildings:");
			ImGui::Spacing();

			if (ImGui::Button("Buy Sklep"))
				buySklep = true;

			ImGui::SameLine();
			ImGui::Text("Price: 100");
			ImGui::Spacing();

			if (ImGui::Button("Buy Budynek"))
				buyBudynek = true;

			ImGui::SameLine();
			ImGui::Text("Price: 200");
			ImGui::Spacing();

			if (ImGui::Button("Buy Blok"))
				buyBlok = true;

			ImGui::SameLine();
			ImGui::Text("Price: 300");
			ImGui::Spacing();

			ImGui::Separator();
			ImGui::Text("Your balance: %d", balance);
			ImGui::Text("The number of your buildings: %d", UpdatableVector.size() - 1);

			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			lightShader.Activate();
			m_Camera.Matrix(lightShader, "camMatrix");
			
			//Wywoływanie obiektów drugiego shadera
			sun.Draw();

			m_Window.SwapBuffers();
		}

		for (auto object : UpdatableVector) {
			delete object;
		}

		// Deletes all ImGUI instances
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	/*
	void Engine::SetFPS(std::uint32_t fps)
	{
		std::cout << "FPS set to: " << fps << std::endl;
		m_Fps = fps;
	}
	*/
}