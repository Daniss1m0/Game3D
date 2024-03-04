// TODO: Zmieniłem strukturę projektu. Więc dużo plików tutaj jeszcze nie ma, aby każdy członek zespołu wiedział od czego zacząć. 
// Wszystkie potrzebne pliki można pobrać z repo Silnika3D (https://github.com/thekumor/PSK_3DEngine/tree/main/Engine/source/Engine).

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
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	// Część właściwa.
	engine.Run();
}