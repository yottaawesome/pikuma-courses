// We don't need to explicitly link to the sdl2 libs as vcpkg takes care of this for us.
// If using subsystem:console, define this to avoid undefined symbol linker errors.
//#define SDL_MAIN_HANDLED
// See https://stackoverflow.com/a/50087608/7448661.
// With subsystem:windows, you need to use WinMain() and not regular main(), but 
// the define is not required.

import shared;
import util;
import renderer;

uint32_t window_width = 800;
uint32_t window_height = 600;

main_app app{};

bool initialize_window()
{
    app.context = std::make_unique<util::sdl_context>(sdl::sdl_init_everything);
    if (not app.context->successful())
    {
        util::print_last_error();
        return false;
    }

    sdl::SDL_DisplayMode mode;
    // https://wiki.libsdl.org/SDL2/SDL_GetCurrentDisplayMode
    if (sdl::SDL_GetCurrentDisplayMode(0, &mode))
    {
        util::print_last_error();
        return false;
    }

    window_width = mode.w;
    window_height = mode.h;

    app.main_buffer = {window_width, window_height};

    // Create a window
    // https://wiki.libsdl.org/SDL2/SDL_CreateWindow
    app.window = util::sdl_window_unique_ptr(
        sdl::SDL_CreateWindow(
            nullptr,
            sdl::sdl_windowpos_centered,
            sdl::sdl_windowpos_centered,
            window_width,
            window_height,
            sdl::SDL_WindowFlags::SDL_WINDOW_BORDERLESS
        )
    );
    if (not app.window)
    {
        util::print_last_error();
        return false;
    }

    // Create a SDL renderer
    // https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
    app.renderer = util::sdl_renderer_unique_ptr(sdl::SDL_CreateRenderer(app.window.get(), -1, 0));
    if (not app.renderer)
    {
        util::print_last_error();
        return false;
    }

    if (sdl::SDL_SetWindowFullscreen(app.window.get(), sdl::SDL_WindowFlags::SDL_WINDOW_FULLSCREEN))
        return false;

    return true;
}

void setup()
{
    if (not app.is_running)
        return;

    app.color_buffer_texture = util::sdl_texture_unique_ptr(
        sdl::SDL_CreateTexture
        (
            app.renderer.get(),
            sdl::SDL_PixelFormatEnum::SDL_PIXELFORMAT_ARGB8888,
            sdl::SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING,
            window_width,
            window_height
        )
    );
}

void process_input()
{
    sdl::SDL_Event eventInfo;
    sdl::SDL_PollEvent(&eventInfo);

    switch (eventInfo.type)
    {
        case sdl::SDL_EventType::SDL_QUIT:
        {
            app.is_running = false;
        }
        break;

        case sdl::SDL_EventType::SDL_KEYDOWN:
        {
            if (eventInfo.key.keysym.sym == sdl::SDL_KeyCode::SDLK_ESCAPE)
                app.is_running = false;
        }
        break;
    }
}

void render_color_buffer()
{
    sdl::SDL_UpdateTexture(
        app.color_buffer_texture.get(),
        nullptr,
        app.main_buffer.raw_buffer(),
        app.main_buffer.pitch()
    );
    sdl::SDL_RenderCopy(
        app.renderer.get(),
        app.color_buffer_texture.get(),
        nullptr, 
        nullptr
    );
}

void update()
{

}

void teardown()
{
    // currently, nothing to do here as everything is managed
    // via smart pointers
}

void render()
{
    sdl::SDL_SetRenderDrawColor(app.renderer.get(), 255, 0, 0, 255);
    sdl::SDL_RenderClear(app.renderer.get());

    render_color_buffer();

    app.main_buffer.fill(0xFFFFFF00);

    sdl::SDL_RenderPresent(app.renderer.get());
}

//int main(int argc, char* argv[]) // use this on subsystem:console
int WinMain(int argc, char* argv[])
{
    app.is_running = initialize_window();

    setup();

    while (app.is_running)
    {
        process_input();
        update();
        render();
    }

    teardown();

    return 0;
}
