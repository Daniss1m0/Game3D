#pragma once

#include <cstdint>
#include <iostream>

#include <glad/glad.h>

namespace eng
{
	void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
}