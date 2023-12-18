import std;
// If using subsystem:console, define this to avoid undefined symbol linker errors otherwise.
// See https://stackoverflow.com/a/50087608/7448661.
// With subsystem:windows, you need to use WinMain() and not regular main().
//#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

int WinMain(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    std::cout << "Hello World!\n";
    return 0;
}
