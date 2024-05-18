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
		BaseObject();

		void Render(GLfloat* vertices, GLsizei verticesSize, GLuint* indices, GLsizei indicesSize);
		virtual void Draw();

	protected:
		inter::VAO m_VAO;
		inter::VBO m_VBO;
		inter::EBO m_EBO;
	};

	class Triangle : public BaseObject
	{
	public:
		Triangle(const glm::mat3x3& position);

		void Move(const glm::fvec3& offset);
	};

	class Map : public BaseObject
	{
	public:
		Map(const glm::mat4x3& pos, Shader& shader);

		void Draw() override;

	private:
		Texture planksTex;
		Texture planksSpec;
	};

	class Piramid : public BaseObject
	{
	public:
		Piramid(const glm::fvec3& pos);

		void Draw() override;
		void Move(const glm::fvec3& offset);

	private:
		Texture brick;
	};

	class Sun : public BaseObject
	{
	public:
		Sun(const glm::fvec3& pos);

		void Draw();
		void Move(const glm::fvec3& offset);

	private:
		inter::VAO m_VAO;
		inter::VBO m_VBO;
		inter::EBO m_EBO;
	};

	class Cube : public BaseObject
	{
	public:
		Cube(const glm::fvec3& pos);

		void Move(const glm::fvec3& offset);
	};

	class Budynek : public Cube {
	public:
		Budynek(const glm::fvec3& pos, Shader& shader);

		void Draw() override;
		
		std::vector<std::string> texturePaths = {
		"textures/planks.png",
		"textures/planks.png",
		"textures/brick.png",
		"textures/planks.png",
		"textures/brick.png",
		"textures/planks.png"
		};

	private:		
		Texture texture;
	};

	class Szkola : public Cube {};

	class Szpital : public Cube {};

	//i t.d. 
}