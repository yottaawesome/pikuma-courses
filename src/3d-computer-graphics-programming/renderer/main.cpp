// We don't need to explicitly link to the sdl2 libs as vcpkg takes care of this for us.
// If using subsystem:console, define this to avoid undefined symbol linker errors.
//#define SDL_MAIN_HANDLED
// See https://stackoverflow.com/a/50087608/7448661.
// With subsystem:windows, you need to use WinMain() and not regular main(), but 
// the define is not required.

import shared;
import util;
import renderer;

main_app app{};

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
            app.window_width,
            app.window_height
        )
    );
}

void teardown()
{
    // currently, nothing to do here as everything is managed
    // via smart pointers
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

void update()
{
}

void render(
    sdl::SDL_Renderer* renderer, 
    sdl::SDL_Texture* color_buffer_texture, 
    color_buffer& buffer
)
{
    sdl::SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    sdl::SDL_RenderClear(renderer);

    //draw_line_grid(10, 0xffc0c0c0, app.main_buffer);
    draw_dot_grid(10, 0xffc0c0c0, buffer);
    draw_rect(50, 50, 100, 100, 0xffc0c0c0, buffer);

    render_color_buffer(renderer, buffer, color_buffer_texture);

    buffer.fill(0xff000000);

    sdl::SDL_RenderPresent(renderer);
}

//int main(int argc, char* argv[]) // use this on subsystem:console
int WinMain(int argc, char* argv[])
{
    initialize_window(app);

    setup();

    while (app.is_running)
    {
        process_input();
        update();
        render(
            app.renderer.get(), 
            app.color_buffer_texture.get(), 
            app.main_buffer
        );
    }

    teardown();

    return 0;
}
