#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <Engine/internal/buffers.h>
#include <Engine/Renderer.h>
#include <Engine/Shader.h>

//TODO: Dziedziczenie. Np. Draw() dla kazdego.

namespace eng
{
	class Texture
	{
	public:
		GLuint ID;
		GLenum type;
		GLuint unit;

		Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);
		Texture(const std::vector<std::string>& images, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);
		~Texture();

		void texUnit(Shader& shader, const char* uniform, GLuint unit);
		void Bind();
		void Unbind();
	};

	class BaseObject {
	public:
		void render();
	};

	class Triangle : public BaseObject
	{
	public:
		Triangle(const glm::mat3x3& position);

		void Draw();
		void Draw(Renderer* renderer);

		void Move(const glm::fvec3& offset);
		//void Rotate();

	private:
		inter::VAO m_VAO;
		inter::VBO m_VBO;
		inter::EBO m_EBO;
	};

	class Map : public BaseObject //rectangle wsm
	{
	public:
		Map(const glm::mat4x3& positions, Shader& shader);

		void Draw();

	private:
		inter::VAO m_VAO;
		inter::VBO m_VBO;
		inter::EBO m_EBO;

		Texture planksTex;
		Texture planksSpec;
	};

	class Piramid : public BaseObject
	{
	public:
		Piramid(const glm::mat3x3& position); //dorobic parameter!

		void Draw();

		void Move(const glm::fvec3& offset);
	private:
		inter::VAO m_VAO;
		inter::VBO m_VBO;
		inter::EBO m_EBO;

		Texture brick;
	};

	class Sun : public BaseObject
	{
	public:
		Sun(const glm::mat3x3& position);

		void Draw();

		void Move();
		void CirclesAround(); //dorobic!
	private:
		inter::VAO m_VAO;
		inter::VBO m_VBO;
		inter::EBO m_EBO;
	};

	class Cube : public BaseObject
	{
	public:
		Cube(const glm::fvec3& pos);

		void Draw();
		void Move(const glm::fvec3& offset);

		void BindVAO() { m_VAO.Bind(); }
		void UnbindVAO() { m_VAO.Unbind(); }
		unsigned int GetEBOCount() { return m_EBO.GetCount(); }

	private:
		inter::VAO m_VAO;
		inter::VBO m_VBO;
		inter::EBO m_EBO;
	};

	class Budynek : public Cube {
	public:
		Budynek(const glm::fvec3& pos, Shader& shader);

		void Draw();
		
		std::vector<std::string> texturePaths = {
		"textures/brick.png",
		"textures/planks.png",
		"textures/brick.png",
		"textures/planks.png",
		"textures/brick.png",
		"textures/brick.png"
		};

	private:		
		Texture texture;
	};

	class Szkola : public Cube {};

	class Szpital : public Cube {};

	//i t.d. 
}