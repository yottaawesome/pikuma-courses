export module renderer:main_app;
import std;
import shared;
import util;

export
{
    constexpr auto number_of_points = 9 * 9 * 9;
    constexpr float fov_factor = 640;

    // An alternative implementation of main_app that does all of its own initialisation.
    struct main_app
    {
        main_app() = default;
        main_app(const main_app&) = delete;
        main_app& operator=(const main_app&) = delete;
        main_app(main_app&&) = default;
        main_app& operator=(main_app&&) = default;

        std::unique_ptr<util::sdl_context> context = 
            []{
                auto ptr = std::make_unique<util::sdl_context>(sdl::sdl_init_everything);
                if (not ptr->successful())
                    throw std::runtime_error(util::print_last_error());
                return ptr;
            }();

        util::basic_rectangle screen_dimensions =
            []() -> util::basic_rectangle 
            {
                sdl::SDL_DisplayMode mode;
                // https://wiki.libsdl.org/SDL2/SDL_GetCurrentDisplayMode
                if (sdl::SDL_GetCurrentDisplayMode(0, &mode))
                    throw std::runtime_error(util::print_last_error());
                return { 
                    .width = static_cast<uint32_t>(mode.w), 
                    .height = static_cast<uint32_t>(mode.h)
                };
            }();

        util::color_buffer main_buffer = 
            { screen_dimensions.width, screen_dimensions.height };

        util::sdl_window_unique_ptr window =
            [](const util::basic_rectangle& screen_dimensions)
            {
                // Create a window
                // https://wiki.libsdl.org/SDL2/SDL_CreateWindow
                auto window = util::sdl_window_unique_ptr(
                    sdl::SDL_CreateWindow(
                        nullptr,
                        sdl::sdl_windowpos_centered,
                        sdl::sdl_windowpos_centered,
                        screen_dimensions.width,
                        screen_dimensions.height,
                        sdl::SDL_WindowFlags::SDL_WINDOW_BORDERLESS
                    )
                );
                if (not window)
                    throw std::runtime_error(util::print_last_error());
                return window;
            }(screen_dimensions);

        util::sdl_renderer_unique_ptr renderer =
            [](const util::sdl_window_unique_ptr& window)
            {
                // Create a SDL renderer
                // https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
                auto renderer = util::sdl_renderer_unique_ptr(sdl::SDL_CreateRenderer(window.get(), -1, 0));
                if (not renderer)
                    throw std::runtime_error(util::print_last_error());
                return renderer;
            }(window);

        util::sdl_texture_unique_ptr color_buffer_texture = 
            [](util::sdl_renderer_unique_ptr& renderer, const util::basic_rectangle& screen_dimensions) 
            {
                return util::sdl_texture_unique_ptr(
                    sdl::SDL_CreateTexture
                    (
                        renderer.get(),
                        sdl::SDL_PixelFormatEnum::SDL_PIXELFORMAT_ARGB8888,
                        sdl::SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING,
                        screen_dimensions.width,
                        screen_dimensions.height
                    )
                );
            }(renderer, screen_dimensions);

        std::array<util::vector_3f, number_of_points> cube_points{};
        util::vector_3f camera_position = { 0, 0, -5 };

        bool is_running = 
            [](std::array<util::vector_3f, number_of_points>& cube_points, const util::sdl_window_unique_ptr& window)
            {
                unsigned count = 0;
                for (float x = -1; x <= 1; x += 0.25)
                    for (float y = -1; y <= 1; y += 0.25)
                        for (float z = -1; z <= 1; z += 0.25)
                            cube_points[count++] = util::vector_3f{ x, y, z };

                if (sdl::SDL_SetWindowFullscreen(window.get(), sdl::SDL_WindowFlags::SDL_WINDOW_FULLSCREEN))
                    throw std::runtime_error(util::print_last_error());

                return true;
            }(cube_points, window);

        std::array<util::vector_2f, number_of_points> projected_cube_points{};
    };
}
