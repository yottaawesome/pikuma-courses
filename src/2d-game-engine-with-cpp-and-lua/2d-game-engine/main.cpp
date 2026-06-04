#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <SDL3/sdl.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <sol/sol.hpp>
import std;
import engine;

void Init()
{
	auto lua = sol::state{};
	lua.open_libraries(sol::lib::base, sol::lib::package);

	auto velocity = glm::vec2{ 5.0f, -2.0f };
	velocity = glm::normalize(velocity);

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
}



auto wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine,
    int nCmdShow
) -> int
try
{
	auto game = Engine::Game{};
	game.Initialize();
	game.Run();
	game.Destroy();
    return 0;
}
catch (const std::exception& e)
{
	MessageBoxA(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
	return -1;
}
catch (...)
{
	MessageBoxA(nullptr, "An unknown error occurred.", "Error", MB_OK | MB_ICONERROR);
	return -1;
}