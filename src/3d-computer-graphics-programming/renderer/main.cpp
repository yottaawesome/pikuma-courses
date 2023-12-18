// We don't need to explicitly link to the sdl2 libs as vcpkg takes care of this for us.
// If using subsystem:console, define this to avoid undefined symbol linker errors.
// See https://stackoverflow.com/a/50087608/7448661.
// With subsystem:windows, you need to use WinMain() and not regular main(), but 
// the define is not required.
//#define SDL_MAIN_HANDLED

import shared;

bool is_running = false;
util::sdl_window_unique_ptr window;
util::sdl_renderer_unique_ptr renderer;

bool initialize_window()
{
    if (sdl::SDL_Init(sdl::sdl_init_everything))
    {
        util::print_last_error();
        return false;
    }

    // Create a window
    // https://wiki.libsdl.org/SDL2/SDL_CreateWindow
    window = util::sdl_window_unique_ptr(
        sdl::SDL_CreateWindow(
            nullptr,
            sdl::sdl_windowpos_centered,
            sdl::sdl_windowpos_centered,
            800,
            600,
            sdl::SDL_WindowFlags::SDL_WINDOW_BORDERLESS
        )
    );
    if (not window)
    {
        util::print_last_error();
        return false;
    }

    // Create a SDL renderer
    // https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
    renderer = util::sdl_renderer_unique_ptr(sdl::SDL_CreateRenderer(window.get(), -1, 0));
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
    sdl::SDL_Event event;
    sdl::SDL_PollEvent(&event);

    switch (event.type)
    {
        case sdl::SDL_EventType::SDL_QUIT:
        {
            is_running = false;
        }
        break;

        case sdl::SDL_EventType::SDL_KEYDOWN:
        {
            if (event.key.keysym.sym == sdl::SDL_KeyCode::SDLK_ESCAPE)
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
    sdl::SDL_SetRenderDrawColor(renderer.get(), 255, 0, 0, 255);
    sdl::SDL_RenderClear(renderer.get());
    sdl::SDL_RenderPresent(renderer.get());
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
