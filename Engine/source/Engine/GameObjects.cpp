#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <Engine/GameObjects.h>

namespace eng
{
	Texture::Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType) {
		type = texType;

		int widthImg, heightImg, numColCh;
		stbi_set_flip_vertically_on_load(true);

		unsigned char* bytes = nullptr;
		try {
			bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
			if (!bytes) {
				throw std::runtime_error("Failed to load texture: " + std::string(image));
			}

			std::cout << "Loaded texture: " << image << " (" << widthImg << "x" << heightImg << ", " << numColCh << " channels)" << std::endl;

			glGenTextures(1, &ID);
			glActiveTexture(GL_TEXTURE0 + slot);
			unit = slot;

			glBindTexture(texType, ID);

			glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

			if (format == GL_RGBA && numColCh == 3) {
				format = GL_RGB;
			}

			glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
			glGenerateMipmap(texType);

			stbi_image_free(bytes);
			glBindTexture(texType, 0);
		}
		catch (const std::runtime_error& e) {
			if (bytes) {
				stbi_image_free(bytes);
			}
			std::cerr << e.what() << std::endl;
			throw; // Rethrow the exception to be handled by the calling code
		}
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &ID);
	}

	void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
	{
		GLuint texUni = glGetUniformLocation(shader.ID, uniform);
		shader.Activate();
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

	Rectangle::Rectangle(const glm::mat4x3& positions)
	{
		GLfloat verticesMAP[] =
		{ //					COORDINATES						/        COLORS        /    TexCoord    /       NORMALS     //
			positions[0][0], positions[0][1], positions[0][2],		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
			positions[1][0], positions[1][1], positions[1][2],		1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
			positions[2][0], positions[2][1], positions[2][2],		1.0f, 1.0f, 1.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
			positions[3][0], positions[3][1], positions[3][2],		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
		};

		GLuint indicesMAP[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		Render(verticesMAP, sizeof(verticesMAP), indicesMAP, sizeof(indicesMAP));
	}

	void Rectangle::Move(const glm::fvec3& offset)
	{
		m_VBO.Bind();

		GLfloat* vertices = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		if (vertices != nullptr)
		{
			for (int i = 0; i < 4; ++i)
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
	
	Map::Map(const glm::mat4x3& positions, Shader& shader) : Rectangle(positions),
		planksTex("textures/grs.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
		planksSpec("textures/planksSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE)
	{
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

	Cell::Cell(const glm::mat4x3& pos, const char* image) : Rectangle(pos),
		tex(image, GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE)
	{
	}

	void Cell::Draw()
	{
		tex.Bind();

		m_VAO.Bind();
		glDrawElements(GL_TRIANGLES, m_EBO.GetCount(), GL_UNSIGNED_INT, 0);
		m_VAO.Unbind();

		tex.Unbind();
	}

	Piramid::Piramid(const glm::fvec3& pos)
		: brick("textures/brick.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE)
	{
		GLfloat verticesPIRAMID[] =
		{ //				 COORDINATES					/        COLORS      /   TexCoord     /       NORMALS     //
			pos.x - 0.1f, pos.y - 0.05f, pos.z + 0.1f,     0.83f, 0.70f, 0.44f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
			pos.x - 0.1f, pos.y - 0.05f, pos.z - 0.1f,     0.83f, 0.70f, 0.44f,   5.0f, 0.0f,   0.0f, 1.0f, 0.0f,
			pos.x + 0.1f, pos.y - 0.05f, pos.z - 0.1f,     0.83f, 0.70f, 0.44f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
			pos.x + 0.1f, pos.y - 0.05f, pos.z + 0.1f,     0.83f, 0.70f, 0.44f,   5.0f, 0.0f,   0.0f, 1.0f, 0.0f,
			pos.x + 0.00f, pos.y + 0.1f, pos.z + 0.00f,     0.92f, 0.86f, 0.76f,   2.5f, 5.0f,   0.0f, 1.0f, 0.0f
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
		{ //			   COORDINATES			    //
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

	Cube::Cube(const glm::fvec3& pos, const glm::fvec3& size)
	{
		GLfloat verticesCube[] =
		{ //					COORDINATES					   	  /		  COLORS        /    TexCoord      /       NORMALS        //
			// Front face
			pos.x - size.x/2, pos.y - size.y/2, pos.z + size.z/2,    1.0f, 1.0f, 1.0f,       0.0f, 0.0f,        0.0f, 1.0f, 0.0f,
			pos.x - size.x/2, pos.y + size.y/2, pos.z + size.z/2,    1.0f, 1.0f, 1.0f,       0.0f, 1.0f,        0.0f, 1.0f, 0.0f,
			pos.x + size.x/2, pos.y + size.y/2, pos.z + size.z/2,    1.0f, 1.0f, 1.0f,       1.0f, 1.0f,        0.0f, 1.0f, 0.0f,
			pos.x + size.x/2, pos.y - size.y/2, pos.z + size.z/2,    1.0f, 1.0f, 1.0f,       1.0f, 0.0f,        0.0f, 1.0f, 0.0f,
			// Back face
			pos.x - size.x/2, pos.y - size.y/2, pos.z - size.z/2,    1.0f, 1.0f, 1.0f,       0.0f, 0.0f,        0.0f, -1.0f, 0.0f,
			pos.x - size.x/2, pos.y + size.y/2, pos.z - size.z/2,    1.0f, 1.0f, 1.0f,       0.0f, 1.0f,        0.0f, -1.0f, 0.0f,
			pos.x + size.x/2, pos.y + size.y/2, pos.z - size.z/2,    1.0f, 1.0f, 1.0f,       1.0f, 1.0f,        0.0f, -1.0f, 0.0f,
			pos.x + size.x/2, pos.y - size.y/2, pos.z - size.z/2,    1.0f, 1.0f, 1.0f,       1.0f, 0.0f,        0.0f, -1.0f, 0.0f,
			// Left face
			pos.x - size.x/2, pos.y - size.y/2, pos.z - size.z/2,    1.0f, 1.0f, 1.0f,       0.0f, 0.0f,        -1.0f, 0.0f, 0.0f,
			pos.x - size.x/2, pos.y + size.y/2, pos.z - size.z/2,    1.0f, 1.0f, 1.0f,       0.0f, 1.0f,        -1.0f, 0.0f, 0.0f,
			pos.x - size.x/2, pos.y + size.y/2, pos.z + size.z/2,    1.0f, 1.0f, 1.0f,       1.0f, 1.0f,        -1.0f, 0.0f, 0.0f,
			pos.x - size.x/2, pos.y - size.y/2, pos.z + size.z/2,    1.0f, 1.0f, 1.0f,       1.0f, 0.0f,        -1.0f, 0.0f, 0.0f,
			// Right face
			pos.x + size.x/2, pos.y - size.y/2, pos.z + size.z/2,    1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         1.0f, 0.0f, 0.0f,
			pos.x + size.x/2, pos.y + size.y/2, pos.z + size.z/2,    1.0f, 1.0f, 1.0f,       0.0f, 1.0f,         1.0f, 0.0f, 0.0f,
			pos.x + size.x/2, pos.y + size.y/2, pos.z - size.z/2,    1.0f, 1.0f, 1.0f,       1.0f, 1.0f,         1.0f, 0.0f, 0.0f,
			pos.x + size.x/2, pos.y - size.y/2, pos.z - size.z/2,    1.0f, 1.0f, 1.0f,       1.0f, 0.0f,         1.0f, 0.0f, 0.0f,
			// Top face
			pos.x - size.x/2, pos.y + size.y/2, pos.z + size.z/2,    1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f, 0.0f, 1.0f,
			pos.x - size.x/2, pos.y + size.y/2, pos.z - size.z/2,    1.0f, 1.0f, 1.0f,       0.0f, 1.0f,         0.0f, 0.0f, 1.0f,
			pos.x + size.x/2, pos.y + size.y/2, pos.z - size.z/2,    1.0f, 1.0f, 1.0f,       1.0f, 1.0f,         0.0f, 0.0f, 1.0f,
			pos.x + size.x/2, pos.y + size.y/2, pos.z + size.z/2,    1.0f, 1.0f, 1.0f,       1.0f, 0.0f,         0.0f, 0.0f, 1.0f,
			// Bottom face
			pos.x - size.x/2, pos.y - size.y/2, pos.z - size.z/2,    1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f, 0.0f, -1.0f,
			pos.x - size.x/2, pos.y - size.y/2, pos.z + size.z/2,    1.0f, 1.0f, 1.0f,       0.0f, 1.0f,         0.0f, 0.0f, -1.0f,
			pos.x + size.x/2, pos.y - size.y/2, pos.z + size.z/2,    1.0f, 1.0f, 1.0f,       1.0f, 1.0f,         0.0f, 0.0f, -1.0f,
			pos.x + size.x/2, pos.y - size.y/2, pos.z - size.z/2,    1.0f, 1.0f, 1.0f,       1.0f, 0.0f,         0.0f, 0.0f, -1.0f
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

	Budynek::Budynek(const glm::fvec3& pos) : Cube(pos, glm::fvec3(0.1, 0.1, 0.1)),
		textures{
			Texture("textures/planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("textures/brick.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("textures/planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("textures/brick.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("textures/planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("textures/brick.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE)
		}
	{
	}

	void Budynek::Draw()
	{
		m_VAO.Bind();

		for (int i = 0; i < textures.size(); ++i) 
		{
			textures[i].Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(GLuint)));
			textures[i].Unbind();
		}

		m_VAO.Unbind();
	}

	Sklep::Sklep(const glm::fvec3& pos) : Cube(pos,glm::fvec3(0.1, 0.1, 0.1)),
		textures{
			Texture("textures/shop/front.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("textures/shop/back.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("textures/shop/side.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("textures/shop/side.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("textures/shop/roof.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("textures/shop/floor.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE)
		}
	{
	}

	void Sklep::Draw()
	{
		m_VAO.Bind();

		for (int i = 0; i < textures.size(); ++i) 
		{
			textures[i].Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(GLuint)));
			textures[i].Unbind();
		}

		m_VAO.Unbind();
	}

	Blok::Blok(const glm::fvec3& pos) : Cube(pos,glm::fvec3(0.1,0.15,0.1)),
		textures{
			Texture("textures/flat/front.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("textures/flat/back.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("textures/flat/side.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("textures/flat/side.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("textures/flat/roof.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
			Texture("textures/flat/floor.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE)
		}
	{
	}

	void Blok::Draw()
	{
		m_VAO.Bind();

		for (int i = 0; i < textures.size(); ++i) 
		{
			textures[i].Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(GLuint)));
			textures[i].Unbind();
		}

		m_VAO.Unbind();
	}


}