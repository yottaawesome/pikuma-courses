import std;
import engine;

//void Init()
//{
//	auto lua = sol::state{};
//	lua.open_libraries(sol::lib::base, sol::lib::package);
//}

auto wWinMain(Win32::HINSTANCE, Win32::HINSTANCE, Win32::LPWSTR, int) -> int
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
	Win32::ShowError(e.what());
	return 1;
}
catch (...)
{
	Win32::ShowError("An unknown error occurred.");
	return 2;
}
