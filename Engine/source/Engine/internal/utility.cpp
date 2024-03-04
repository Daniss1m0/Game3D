#include "utility.h"

namespace eng
{
    void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        std::cout << "OpenGL callback: " << message << std::endl;
    }
}