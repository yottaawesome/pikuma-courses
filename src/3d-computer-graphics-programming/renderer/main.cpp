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

bool initialize_window(main_app& app)
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

    app.window_width = mode.w;
    app.window_height = mode.h;

    app.main_buffer = { app.window_width, app.window_height};

    // Create a window
    // https://wiki.libsdl.org/SDL2/SDL_CreateWindow
    app.window = util::sdl_window_unique_ptr(
        sdl::SDL_CreateWindow(
            nullptr,
            sdl::sdl_windowpos_centered,
            sdl::sdl_windowpos_centered,
            app.window_width,
            app.window_height,
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

    return app.is_running = true;
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
            app.window_width,
            app.window_height
        )
    );
}

void draw_line_grid(const int32_t step, const uint32_t color, color_buffer& buffer)
{
    for (uint32_t row = 0; row < buffer.height(); row++)
    {
        for (uint32_t column = 0; column < buffer.width(); column++)
        {
            if (row == 0)
                buffer.set(row, column, color);
            else if (column == 0)
                buffer.set(row, column, color);
            else if (std::div(row + 1ui32, step).rem == 0)
                buffer.set(row, column, color);
            else if (std::div(column + 1ui32, step).rem == 0)
                buffer.set(row, column, color);
        }
    }
}

void draw_dot_grid(const int32_t step, const uint32_t color, color_buffer& buffer)
{
    for (uint32_t row = 0; row < buffer.height(); row += 10)
    {
        for (uint32_t column = 0; column < buffer.width(); column += 10)
        {
            buffer.set(row, column, color);
        }
    }
}

void draw_rect(
    const uint32_t x,
    const uint32_t y,
    const uint32_t width,
    const uint32_t height,
    const uint32_t color,
    color_buffer& buffer
)
{
    for (uint32_t row = y; row < y + width and row < buffer.height(); row++)
    {
        for (uint32_t column = x; column < x + height and column < buffer.width(); column++)
        {
            buffer.set(row, column, color);
        }
    }
}

void render_color_buffer(
    sdl::SDL_Renderer* renderer,
    color_buffer& buffer,
    SDL_Texture* color_buffer_texture
)
{
    sdl::SDL_UpdateTexture(
        color_buffer_texture,
        nullptr,
        buffer.raw_buffer(),
        buffer.pitch()
    );
    sdl::SDL_RenderCopy(
        renderer,
        color_buffer_texture,
        nullptr, 
        nullptr
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

void render(sdl::SDL_Renderer* renderer, color_buffer& buffer)
{
    sdl::SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    sdl::SDL_RenderClear(renderer);

    //draw_line_grid(10, 0xffc0c0c0, app.main_buffer);
    draw_dot_grid(10, 0xffc0c0c0, buffer);
    draw_rect(50, 50, 100, 100, 0xffc0c0c0, buffer);

    render_color_buffer(app.renderer.get(), app.main_buffer, app.color_buffer_texture.get());

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
        render(app.renderer.get(), app.main_buffer);
    }

    teardown();

    return 0;
}
