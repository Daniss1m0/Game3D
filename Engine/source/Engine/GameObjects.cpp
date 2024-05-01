#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <Engine/GameObjects.h>

namespace eng
{
	Texture::Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType)
	{
		// Assigns the type of the texture ot the texture object
		type = texType;

		// Stores the width, height, and the number of color channels of the image
		int widthImg, heightImg, numColCh;
		// Flips the image so it appears right side up
		stbi_set_flip_vertically_on_load(true);
		// Reads the image from a file and stores it in bytes
		unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

		// Generates an OpenGL texture object
		glGenTextures(1, &ID);
		// Assigns the texture to a Texture Unit
		glActiveTexture(GL_TEXTURE0 + slot);
		unit = slot;
		glBindTexture(texType, ID);

		// Configures the type of algorithm that is used to make the image smaller or bigger
		glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Configures the way the texture repeats (if it does at all)
		glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
		// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
		// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

		// Assigns the image to the OpenGL Texture object
		glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
		// Generates MipMaps
		glGenerateMipmap(texType);

		// Deletes the image data as it is already in the OpenGL Texture object
		stbi_image_free(bytes);

		// Unbinds the OpenGL Texture object so that it can't accidentally be modified
		glBindTexture(texType, 0);
	}

	void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
	{
		// Gets the location of the uniform
		GLuint texUni = glGetUniformLocation(shader.ID, uniform);
		// Shader needs to be activated before changing the value of a uniform
		shader.Activate();
		// Sets the value of the uniform
		glUniform1i(texUni, unit);
	}

	void Texture::Bind()
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(type, ID);
	}

	void Texture::Unbind()
	{
		glBindTexture(type, 0);
	}

	void Texture::Delete()
	{
		glDeleteTextures(1, &ID);
	}
	
	Triangle::Triangle(const GLfloat* vertices, GLsizeiptr verticesSize, const GLuint* indices, GLsizeiptr indicesSize)
		: m_VAO(), m_VBO(const_cast<GLfloat*>(vertices), verticesSize), m_EBO(const_cast<GLuint*>(indices), indicesSize)
		//m_VAO(), m_VBO(const_cast<GLfloat*>(vertices), verticesSize), m_EBO(const_cast<GLuint*>(indices), indicesSize) // dla wszystkich
	{
		std::cout << verticesSize << "   " << sizeof(vertices) << std::endl; //16,5 dla trojktaa
		std::cout << indicesSize << "   " << sizeof(indices) << std::endl; //1,5 dla trojkata
		m_VAO.Bind();
		m_VAO.LinkAttrib(m_VBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
		m_VAO.LinkAttrib(m_VBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
		m_VAO.LinkAttrib(m_VBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
		m_VAO.LinkAttrib(m_VBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
		m_VAO.Unbind();
		m_VBO.Unbind();
		m_EBO.Unbind();
	}

	Triangle::~Triangle() 
	{
		m_VAO.Delete();
		m_VBO.Delete();
		m_EBO.Delete();
	}

	void Triangle::Draw()
	{
		m_VAO.Bind();
		glDrawElements(GL_TRIANGLES, m_EBO.GetCount(), GL_UNSIGNED_INT, 0);
		m_VAO.Unbind();
	}
	/*
	void Triangle::Draw(Renderer* renderer)
	{
		renderer->Draw(m_VAO.GetId());
	}
	*/

	void Triangle::Move(const glm::fvec3& offset)
	{
		m_VBO.Bind();

		GLfloat* vertices = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		if (vertices != nullptr)
		{
			for (int i = 0; i < 3; ++i)
			{
				int startIndex = i * 11;

				vertices[startIndex] += offset.x;
				vertices[startIndex + 1] += offset.y;
				vertices[startIndex + 2] += offset.z;
			}

			glUnmapBuffer(GL_ARRAY_BUFFER);
		}

		m_VBO.Unbind();
	}

	/* Dla rectangle
	void Triangle::Move(float xOffset, float yOffset, float zOffset) // zrobic const glm::fvec3& offse
	{
		m_VBO.Bind();

		// Map the buffer data to a pointer so we can modify it
		GLfloat* vertices = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		if (vertices != nullptr)
		{
			// Update the positions of the vertices of the first triangle
			for (int i = 0; i < 3; ++i)
			{
				// Calculate the index of the current vertex of the first triangle
				int startIndex = i * 11; // Assuming each vertex has 11 elements (3 for position, 3 for color, 2 for texture, and 3 for normal)

				// Update the position of the vertex
				vertices[startIndex] += xOffset;
				vertices[startIndex + 1] += yOffset;
				vertices[startIndex + 2] += zOffset;
			}

			// Update the positions of the vertices of the second triangle
			for (int i = 3; i < 6; ++i)
			{
				// Calculate the index of the current vertex of the second triangle
				int startIndex = i * 11; // Assuming each vertex has 11 elements (3 for position, 3 for color, 2 for texture, and 3 for normal)

				// Update the position of the vertex
				vertices[startIndex] += xOffset;
				vertices[startIndex + 1] += yOffset;
				vertices[startIndex + 2] += zOffset;
			}

			// Unmap the buffer data
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}

		// Unbind the VBO
		m_VBO.Unbind();
	}
	*/
}