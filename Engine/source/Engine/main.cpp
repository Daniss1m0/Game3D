// TODO: Doxygen. ImGUI.

#include "engine.h"
#include "internal/utility.h"

#define WINDOW_WIDTH 1360
#define WINDOW_HEIGHT 768
#define WINDOW_TITLE "Game 3D"

int main()
{
	using namespace eng;
	
	Engine engine(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

	// Debugowanie
	glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	// Część właściwa.
	engine.Run();
}