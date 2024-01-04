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

constexpr auto number_of_points = 9 * 9 * 9;
std::array<vector_3f, number_of_points> cube_points{};

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

    unsigned count = 0;
    for (float x = -1; x <= 1; x += 0.25)
    {
        for (float y = -1; y <= 1; y += 0.25)
        {
            for (float z = -1; z <= 1; z += 0.25)
            {
                cube_points[count++] = vector_3f{ x,y,z };
            }
        }
    }
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

void update(const std::chrono::milliseconds elapsed)
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
    //draw_rect(r.origin.x, r.origin.y, r.width, r.height, 0xffc0c0c0, buffer);
    //draw_rect(50, 50, 60, 50, 0xffc0c0c0, buffer);
    draw_pixel(0, 50, 0xffff0000, buffer);

    render_color_buffer(renderer, buffer, color_buffer_texture);

    buffer.fill(0xff000000);

    sdl::SDL_RenderPresent(renderer);
}

//int main(int argc, char* argv[]) // use this on subsystem:console
int WinMain(int argc, char* argv[])
{
    initialize_window(app);

    setup();

    std::chrono::milliseconds elapsed{ 0 };
    while (app.is_running)
    {
        std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
        process_input();
        update(elapsed);
        render(
            app.renderer.get(), 
            app.color_buffer_texture.get(), 
            app.main_buffer
        );
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(begin - std::chrono::high_resolution_clock::now());
    }

    teardown();

    return 0;
}
