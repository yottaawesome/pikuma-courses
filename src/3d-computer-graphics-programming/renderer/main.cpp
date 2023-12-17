#include <iostream>
#include <SDL2/SDL.h>

#ifdef _DEBUG // we get undefined symbol linker errors otherwise
#pragma comment(lib, "SDL2d.lib")
#pragma comment(lib, "sdl2maind.lib")
#else
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "sdl2main.lib")
#endif

int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";
    return 0;
}
