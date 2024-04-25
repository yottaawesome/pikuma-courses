export module renderer:main_app;
import std;
import shared;
import util;

namespace
{
    constexpr auto total_cube_vertices = 8;
    constexpr auto total_cube_faces = 6 * 2;

    auto cube_vertices = std::array<util::vector_3f, total_cube_vertices>{
        util::vector_3f{ .x = -1, .y = -1, .z = -1 }, // 1
        util::vector_3f{ .x = -1, .y = 1, .z = -1 }, // 2
        util::vector_3f{ .x = 1, .y = 1, .z = -1 }, // 3
        util::vector_3f{ .x = 1, .y = -1, .z = -1 }, // 4
        util::vector_3f{ .x = 1, .y = 1, .z = 1 }, // 5
        util::vector_3f{ .x = 1, .y = -1, .z = 1 }, // 6
        util::vector_3f{ .x = -1, .y = 1, .z = 1 }, // 7
        util::vector_3f{ .x = -1, .y = -1, .z = 1 } // 8
    };

    auto cube_faces = std::array<util::face, total_cube_faces>
    {
        util::face{ 1, 2, 3 },
        util::face{ 1, 3, 4 },
        util::face{ 4, 3, 5 },
        util::face{ 4, 5, 6 },
        util::face{ 6, 5, 7 },
        util::face{ 6, 7, 8 },
        util::face{ 8, 7, 2 },
        util::face{ 8, 2, 1 },
        util::face{ 2, 7, 5 },
        util::face{ 2, 5, 3 },
        util::face{ 6, 8, 1 },
        util::face{ 6, 1, 4 }
    };
}

export namespace main_app
{
    constexpr auto number_of_points = 9 * 9 * 9;
    constexpr float fov_factor = 640;
    constexpr int fps = 60;
    constexpr std::chrono::milliseconds frame_target_time{ 1000 / fps };

    std::vector<util::triangle> triangles_to_render; // renderer::mesh_faces.size()

    std::unique_ptr<util::sdl_context> context =
        [] {
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

    util::sdl_renderer_unique_ptr sdl_renderer =
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
        }(sdl_renderer, screen_dimensions);

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

    std::chrono::milliseconds previous_frame_time{ 0 };
    std::chrono::milliseconds elapsed{ 0 };

    util::mesh cube_mesh = util::load_obj_file("..\\assets\\cube.obj");
}
