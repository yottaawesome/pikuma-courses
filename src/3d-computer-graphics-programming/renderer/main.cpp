// We don't need to explicitly link to the sdl2 libs as vcpkg takes care of this for us.
// If using subsystem:console, define this to avoid undefined symbol linker errors.
// See https://stackoverflow.com/a/50087608/7448661.
// With subsystem:windows, you need to use WinMain() and not regular main(), but 
// the define is not required.
//#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

import std;
import util;

bool is_running = false;
util::sdl_window_unique_ptr window;
util::sdl_renderer_unique_ptr renderer;

bool initialize_window()
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        util::print_last_error();
        return false;
    }

    // Create a window
    // https://wiki.libsdl.org/SDL2/SDL_CreateWindow
    window = util::sdl_window_unique_ptr(
        SDL_CreateWindow(
            nullptr,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            800,
            600,
            SDL_WINDOW_BORDERLESS
        )
    );
    if (not window)
    {
        util::print_last_error();
        return false;
    }

    // Create a SDL renderer
    // https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
    renderer = util::sdl_renderer_unique_ptr(SDL_CreateRenderer(window.get(), -1, 0));
    if (not renderer)
    {
        util::print_last_error();
        return false;
    }

    return true;
}

void setup()
{

}

void process_input()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
        case SDL_QUIT:
        {
            is_running = false;
        }
        break;

        case SDL_KEYDOWN:
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                is_running = false;
        }
        break;
    }
}

void update()
{

}

void render()
{
    SDL_SetRenderDrawColor(renderer.get(), 255, 0, 0, 255);
    SDL_RenderClear(renderer.get());
    SDL_RenderPresent(renderer.get());
}

//int main(int argc, char* argv[]) // use this on subsystem:console
int WinMain(int argc, char* argv[])
{
    is_running = initialize_window();

    setup();

    while (is_running)
    {
        process_input();
        update();
        render();
    }

    return 0;
}
