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

	// Ten konstruktor nie dziala tak, jak tego chce :(
	Texture::Texture(const std::vector<std::string>& images, GLenum texType, GLuint slot, GLenum format, GLenum pixelType)
	{
		// Assigns the type of the texture to the texture object
		type = texType;

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

		// Loads and assigns each image to the OpenGL Texture object

		for (size_t i = 0; i < images.size(); ++i)
		{
			std::cout << images[i] << std::endl;
			// Stores the width, height, and the number of color channels of the image
			int widthImg, heightImg, numColCh;
			// Flips the image so it appears right side up
			stbi_set_flip_vertically_on_load(true);
			// Reads the image from a file and stores it in bytes
			unsigned char* bytes = stbi_load(images[i].c_str(), &widthImg, &heightImg, &numColCh, 0);
			if (bytes)
			{
				glTexImage2D(
					GL_TEXTURE_2D, // Assuming 2D textures for simplicity, can be modified for other types
					0,
					GL_RGBA,
					widthImg,
					heightImg,
					0,
					format,
					pixelType,
					bytes
				);

				// Deletes the image data as it is already in the OpenGL Texture object
				stbi_image_free(bytes);
			}
			else
			{
				std::cout << "Failed to load texture: " << images[i] << std::endl;
			}
		}

		// Generates MipMaps
		glGenerateMipmap(texType);

		// Unbinds the OpenGL Texture object so that it can't accidentally be modified
		glBindTexture(texType, 0);

		/* Inna wersja
		// Creates the cubemap texture object
		unsigned int cubemapTexture;
		glGenTextures(1, &cubemapTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// These are very important to prevent seams
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		// This might help with seams on some systems
		//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		// Cycles through all the textures and attaches them to the cubemap object
		for (unsigned int i = 0; i < 6; i++)
		{
			int width, height, nrChannels;
			unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				stbi_set_flip_vertically_on_load(false);
				glTexImage2D
				(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0,
					GL_RGB,
					width,
					height,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					data
				);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
				stbi_image_free(data);
				//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			}
		}
		*/
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &ID);
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

	BaseObject::BaseObject() 
		: m_VAO(), m_VBO(nullptr, 0), m_EBO(nullptr, 0)
	{
	}

	void BaseObject::Render(GLfloat* vertices, GLsizei verticesSize, GLuint* indices, GLsizei indicesSize)
	{
		m_VAO.Bind();

		m_VBO.Bind();
		m_VBO.BufferData(vertices, verticesSize);

		m_EBO.Bind();
		m_EBO.BufferData(indices, indicesSize);

		m_VAO.LinkAttrib(m_VBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
		m_VAO.LinkAttrib(m_VBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
		m_VAO.LinkAttrib(m_VBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
		m_VAO.LinkAttrib(m_VBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

		m_VAO.Unbind();
		m_VBO.Unbind();
		m_EBO.Unbind();
	}

	void BaseObject::Draw()
	{
		m_VAO.Bind();
		glDrawElements(GL_TRIANGLES, m_EBO.GetCount(), GL_UNSIGNED_INT, 0);
		m_VAO.Unbind();
	}

	Triangle::Triangle(const glm::mat3x3& positions)
	{
		GLfloat verticesTriangle[] =
		{ //					COORDINATES						/        COLORS        /    TexCoord    /       NORMALS     //
			positions[0][0], positions[0][1], positions[0][2],		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
			positions[1][0], positions[1][1], positions[1][2],		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
			positions[2][0], positions[2][1], positions[2][2],		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f
		};

		GLuint indicesTriangle[] = { 0, 1, 2 };

		Render(verticesTriangle, sizeof(verticesTriangle), indicesTriangle, sizeof(indicesTriangle));
	}
	
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
	
	Map::Map(const glm::mat4x3& positions, Shader& shader)
		: planksTex("textures/planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
		  planksSpec("textures/planksSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE)
	{
		GLfloat verticesMAP[] =
		{ //					COORDINATES						/        COLORS        /    TexCoord    /       NORMALS     //
			positions[0][0], positions[0][1], positions[0][2],		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
			positions[1][0], positions[1][1], positions[1][2],		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
			positions[2][0], positions[2][1], positions[2][2],		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
			positions[3][0], positions[3][1], positions[3][2],		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
		};

		GLuint indicesMAP[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		Render(verticesMAP, sizeof(verticesMAP), indicesMAP, sizeof(indicesMAP));

		planksTex.texUnit(shader, "tex0", 0);
		planksSpec.texUnit(shader, "tex1", 1);
	}

	void Map::Draw()
	{
		planksTex.Bind();
		planksSpec.Bind();

		m_VAO.Bind();
		glDrawElements(GL_TRIANGLES, m_EBO.GetCount(), GL_UNSIGNED_INT, 0);
		m_VAO.Unbind();

		planksTex.Unbind();
		planksSpec.Unbind();
	}

	Piramid::Piramid(const glm::fvec3& pos)
		: brick("textures/brick.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE)
	{
		GLfloat verticesPIRAMID[] =
		{ //				 COORDINATES					/        COLORS      /   TexCoord     /       NORMALS     //
			pos.x - 0.25f, pos.y + 0.0f, pos.z + 0.25f,     0.83f, 0.70f, 0.44f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
			pos.x - 0.25f, pos.y + 0.0f, pos.z - 0.25f,     0.83f, 0.70f, 0.44f,   5.0f, 0.0f,   0.0f, 1.0f, 0.0f,
			pos.x + 0.25f, pos.y + 0.0f, pos.z - 0.25f,     0.83f, 0.70f, 0.44f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
			pos.x + 0.25f, pos.y + 0.0f, pos.z + 0.25f,     0.83f, 0.70f, 0.44f,   5.0f, 0.0f,   0.0f, 1.0f, 0.0f,
			pos.x + 0.00f, pos.y + 0.4f, pos.z + 0.00f,     0.92f, 0.86f, 0.76f,   2.5f, 5.0f,   0.0f, 1.0f, 0.0f
		};

		GLuint indicesPIRAMID[] =
		{
			0, 1, 2,
			0, 2, 3,
			0, 1, 4,
			1, 2, 4,
			2, 3, 4,
			3, 0, 4
		};

		Render(verticesPIRAMID, sizeof(verticesPIRAMID), indicesPIRAMID, sizeof(indicesPIRAMID));

		//brick.texUnit(shader, "tex0", 0);
	}

	void Piramid::Draw()
	{
		brick.Bind();

		m_VAO.Bind();
		glDrawElements(GL_TRIANGLES, m_EBO.GetCount(), GL_UNSIGNED_INT, 0);
		m_VAO.Unbind();

		brick.Unbind();
	}

	void Piramid::Move(const glm::fvec3& offset)
	{
		m_VBO.Bind();

		GLfloat* vertices = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		if (vertices != nullptr)
		{
			for (int i = 0; i < 5; ++i)
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

	Sun::Sun(const glm::fvec3& pos)
		: m_VAO(), m_VBO(nullptr, 0), m_EBO(nullptr, 0)
	{
		GLfloat lightVertices[] =
		{ //     COORDINATES     //
			pos.x - 0.1f, pos.y - 0.1f, pos.z + 0.1f,
			pos.x - 0.1f, pos.y - 0.1f, pos.z - 0.1f,
			pos.x + 0.1f, pos.y - 0.1f, pos.z - 0.1f,
			pos.x + 0.1f, pos.y - 0.1f, pos.z + 0.1f,
			pos.x - 0.1f, pos.y + 0.1f, pos.z + 0.1f,
			pos.x - 0.1f, pos.y + 0.1f, pos.z - 0.1f,
			pos.x + 0.1f, pos.y + 0.1f, pos.z - 0.1f,
			pos.x + 0.1f, pos.y + 0.1f, pos.z + 0.1f
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

		m_VAO.Bind();

		m_VBO.Bind();
		m_VBO.BufferData(lightVertices, sizeof(lightVertices));

		m_EBO.Bind();
		m_EBO.BufferData(lightIndices, sizeof(lightIndices));

		m_VAO.LinkAttrib(m_VBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

		m_VAO.Unbind();
		m_VBO.Unbind();
		m_EBO.Unbind();
	}

	void Sun::Draw()
	{
		m_VAO.Bind();
		glDrawElements(GL_TRIANGLES, m_EBO.GetCount(), GL_UNSIGNED_INT, 0);
		m_VAO.Unbind();
	}

	void Sun::Move(const glm::fvec3& offset)
	{
		m_VBO.Bind();

		GLfloat* vertices = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		if (vertices != nullptr)
		{
			for (int i = 0; i < 8; ++i)
			{
				int startIndex = i * 3;

				vertices[startIndex] += offset.x;
				vertices[startIndex + 1] += offset.y;
				vertices[startIndex + 2] += offset.z;
			}

			glUnmapBuffer(GL_ARRAY_BUFFER);
		}

		m_VBO.Unbind();
	}

	Cube::Cube(const glm::fvec3& pos)
	{
		GLfloat verticesCube[] =
		{ //				COORDINATES			   	  /		  COLORS        /    TexCoord      /       NORMALS        //
			// Front face
			pos.x - 0.1f, pos.y - 0.1f, pos.z + 0.1f,    1.0f, 1.0f, 1.0f,       0.0f, 0.0f,        0.0f, 1.0f, 0.0f,
			pos.x - 0.1f, pos.y + 0.1f, pos.z + 0.1f,    1.0f, 1.0f, 1.0f,       0.0f, 1.0f,        0.0f, 1.0f, 0.0f,
			pos.x + 0.1f, pos.y + 0.1f, pos.z + 0.1f,    1.0f, 1.0f, 1.0f,       1.0f, 1.0f,        0.0f, 1.0f, 0.0f,
			pos.x + 0.1f, pos.y - 0.1f, pos.z + 0.1f,    1.0f, 1.0f, 1.0f,       1.0f, 0.0f,        0.0f, 1.0f, 0.0f,
			// Back face
			pos.x - 0.1f, pos.y - 0.1f, pos.z - 0.1f,    1.0f, 1.0f, 1.0f,       0.0f, 0.0f,        0.0f, -1.0f, 0.0f,
			pos.x - 0.1f, pos.y + 0.1f, pos.z - 0.1f,    1.0f, 1.0f, 1.0f,       0.0f, 1.0f,        0.0f, -1.0f, 0.0f,
			pos.x + 0.1f, pos.y + 0.1f, pos.z - 0.1f,    1.0f, 1.0f, 1.0f,       1.0f, 1.0f,        0.0f, -1.0f, 0.0f,
			pos.x + 0.1f, pos.y - 0.1f, pos.z - 0.1f,    1.0f, 1.0f, 1.0f,       1.0f, 0.0f,        0.0f, -1.0f, 0.0f,
			// Left face
			pos.x - 0.1f, pos.y - 0.1f, pos.z - 0.1f,    1.0f, 1.0f, 1.0f,       0.0f, 0.0f,        -1.0f, 0.0f, 0.0f,
			pos.x - 0.1f, pos.y + 0.1f, pos.z - 0.1f,    1.0f, 1.0f, 1.0f,       0.0f, 1.0f,        -1.0f, 0.0f, 0.0f,
			pos.x - 0.1f, pos.y + 0.1f, pos.z + 0.1f,    1.0f, 1.0f, 1.0f,       1.0f, 1.0f,        -1.0f, 0.0f, 0.0f,
			pos.x - 0.1f, pos.y - 0.1f, pos.z + 0.1f,    1.0f, 1.0f, 1.0f,       1.0f, 0.0f,        -1.0f, 0.0f, 0.0f,
			// Right face
			pos.x + 0.1f, pos.y - 0.1f, pos.z + 0.1f,    1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         1.0f, 0.0f, 0.0f,
			pos.x + 0.1f, pos.y + 0.1f, pos.z + 0.1f,    1.0f, 1.0f, 1.0f,       0.0f, 1.0f,         1.0f, 0.0f, 0.0f,
			pos.x + 0.1f, pos.y + 0.1f, pos.z - 0.1f,    1.0f, 1.0f, 1.0f,       1.0f, 1.0f,         1.0f, 0.0f, 0.0f,
			pos.x + 0.1f, pos.y - 0.1f, pos.z - 0.1f,    1.0f, 1.0f, 1.0f,       1.0f, 0.0f,         1.0f, 0.0f, 0.0f,
			// Top face
			pos.x - 0.1f, pos.y + 0.1f, pos.z + 0.1f,    1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f, 0.0f, 1.0f,
			pos.x - 0.1f, pos.y + 0.1f, pos.z - 0.1f,    1.0f, 1.0f, 1.0f,       0.0f, 1.0f,         0.0f, 0.0f, 1.0f,
			pos.x + 0.1f, pos.y + 0.1f, pos.z - 0.1f,    1.0f, 1.0f, 1.0f,       1.0f, 1.0f,         0.0f, 0.0f, 1.0f,
			pos.x + 0.1f, pos.y + 0.1f, pos.z + 0.1f,    1.0f, 1.0f, 1.0f,       1.0f, 0.0f,         0.0f, 0.0f, 1.0f,
			// Bottom face
			pos.x - 0.1f, pos.y - 0.1f, pos.z - 0.1f,    1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f, 0.0f, -1.0f,
			pos.x - 0.1f, pos.y - 0.1f, pos.z + 0.1f,    1.0f, 1.0f, 1.0f,       0.0f, 1.0f,         0.0f, 0.0f, -1.0f,
			pos.x + 0.1f, pos.y - 0.1f, pos.z + 0.1f,    1.0f, 1.0f, 1.0f,       1.0f, 1.0f,         0.0f, 0.0f, -1.0f,
			pos.x + 0.1f, pos.y - 0.1f, pos.z - 0.1f,    1.0f, 1.0f, 1.0f,       1.0f, 0.0f,         0.0f, 0.0f, -1.0f
		};

		GLuint indicesCube[] =
		{
			// Front face
			0, 1, 2,
			2, 3, 0,
			// Back face
			4, 5, 6,
			6, 7, 4,
			// Left face
			8, 9, 10,
			10, 11, 8,
			// Right face
			12, 13, 14,
			14, 15, 12,
			// Top face
			16, 17, 18,
			18, 19, 16,
			// Bottom face
			20, 21, 22,
			22, 23, 20
		};

		Render(verticesCube, sizeof(verticesCube), indicesCube, sizeof(indicesCube));
	}
	
	void Cube::Move(const glm::fvec3& offset)
	{
		m_VBO.Bind();

		GLfloat* vertices = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		if (vertices != nullptr)
		{
			for (int i = 0; i < 24; ++i) 
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

	Budynek::Budynek(const glm::fvec3& pos, Shader& shader) : Cube(pos), 
		texture(texturePaths, GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE)
	{
		texture.texUnit(shader, "tex0", 0);
	}

	void Budynek::Draw()
	{
		texture.Bind();

		m_VAO.Bind();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, m_EBO.GetCount(), GL_UNSIGNED_INT, 0);
		m_VAO.Unbind();

		texture.Unbind();
	}


}