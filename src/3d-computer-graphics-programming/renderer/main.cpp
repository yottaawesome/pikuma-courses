import std;
// we get undefined symbol linker errors otherwise, requires subsystem:console
// https://stackoverflow.com/a/50087608/7448661
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

// Alternative way of solving the above linker issue with subsystem:windows
//#ifdef _DEBUG // we get undefined symbol linker errors otherwise
//#pragma comment(lib, "SDL2d.lib")
//#pragma comment(lib, "sdl2maind.lib")
//#else
//#pragma comment(lib, "SDL2.lib")
//#pragma comment(lib, "sdl2main.lib")
//#endif

int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";
    return 0;
}
