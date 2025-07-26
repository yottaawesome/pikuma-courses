export module librenderer:mainapp;
import std;
import :math;
import :renderer;
import :util;
import :math;
import :raii;

namespace
{
    constexpr auto total_cube_vertices = 8;
    constexpr auto total_cube_faces = 6 * 2;

    auto cube_vertices = std::array<math::vector_3f, total_cube_vertices>{
        math::vector_3f{ .x = -1, .y = -1, .z = -1 }, // 1
        math::vector_3f{ .x = -1, .y = 1, .z = -1 }, // 2
        math::vector_3f{ .x = 1, .y = 1, .z = -1 }, // 3
        math::vector_3f{ .x = 1, .y = -1, .z = -1 }, // 4
        math::vector_3f{ .x = 1, .y = 1, .z = 1 }, // 5
        math::vector_3f{ .x = 1, .y = -1, .z = 1 }, // 6
        math::vector_3f{ .x = -1, .y = 1, .z = 1 }, // 7
        math::vector_3f{ .x = -1, .y = -1, .z = 1 } // 8
    };

    auto cube_faces = std::array<math::face, total_cube_faces>
    {
        math::face{ 1, 2, 3 },
        math::face{ 1, 3, 4 },
        math::face{ 4, 3, 5 },
        math::face{ 4, 5, 6 },
        math::face{ 6, 5, 7 },
        math::face{ 6, 7, 8 },
        math::face{ 8, 7, 2 },
        math::face{ 8, 2, 1 },
        math::face{ 2, 7, 5 },
        math::face{ 2, 5, 3 },
        math::face{ 6, 8, 1 },
        math::face{ 6, 1, 4 }
    };
}

export namespace main_app
{
    constexpr auto number_of_points = 9 * 9 * 9;
    constexpr float fov_factor = 640;
    constexpr int fps = 60;
    constexpr std::chrono::milliseconds frame_target_time{ 1000 / fps };

    std::vector<math::triangle> triangles_to_render; // renderer::mesh_faces.size()

    std::unique_ptr<raii::sdl_context> context = std::make_unique<raii::sdl_context>(sdl::sdl_init_everything);

    math::basic_rectangle screen_dimensions =
        []() -> math::basic_rectangle
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

    math::color_buffer main_buffer =
        { screen_dimensions.width, screen_dimensions.height };

    raii::sdl_window_unique_ptr window =
        [](const math::basic_rectangle& screen_dimensions)
        {
            // Create a window
            // https://wiki.libsdl.org/SDL2/SDL_CreateWindow
            auto window = raii::sdl_window_unique_ptr(
                sdl::SDL_CreateWindow(
                    nullptr,
                    sdl::sdl_windowpos_centered,
                    sdl::sdl_windowpos_centered,
                    screen_dimensions.width-100,
                    screen_dimensions.height-100,
                    sdl::SDL_WindowFlags::SDL_WINDOW_BORDERLESS
                )
            );
            if (not window)
                throw std::runtime_error(util::print_last_error());
            return window;
        }(screen_dimensions);

    raii::sdl_renderer_unique_ptr sdl_renderer =
        [](const raii::sdl_window_unique_ptr& window)
        {
            // Create a SDL renderer
            // https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
            auto renderer = raii::sdl_renderer_unique_ptr(sdl::SDL_CreateRenderer(window.get(), -1, 0));
            if (not renderer)
                throw std::runtime_error(util::print_last_error());
            return renderer;
        }(window);

    raii::sdl_texture_unique_ptr color_buffer_texture =
        [](raii::sdl_renderer_unique_ptr& renderer, const math::basic_rectangle& screen_dimensions)
        {
            return raii::sdl_texture_unique_ptr(
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

    std::array<math::vector_3f, number_of_points> cube_points{};
    math::vector_3f camera_position = { 0, 0, 0 };

    bool is_running =
        [](std::array<math::vector_3f, number_of_points>& cube_points, const raii::sdl_window_unique_ptr& window)
        {
            unsigned count = 0;
            for (float x = -1; x <= 1; x += 0.25)
                for (float y = -1; y <= 1; y += 0.25)
                    for (float z = -1; z <= 1; z += 0.25)
                        cube_points[count++] = math::vector_3f{ x, y, z };

            //if (sdl::SDL_SetWindowFullscreen(window.get(), sdl::SDL_WindowFlags::SDL_WINDOW_FULLSCREEN))
                //throw std::runtime_error(util::print_last_error());

            return true;
        }(cube_points, window);

    std::array<math::vector_2f, number_of_points> projected_cube_points{};

    std::chrono::milliseconds previous_frame_time{ 0 };
    std::chrono::milliseconds elapsed{ 0 };

    renderer::mesh cube_mesh = renderer::load_obj_file("..\\assets\\cube.obj");

    enum class render_mode
    {
        wireframe_with_dot,
        wireframe,
        filled,
        filled_wireframe
    };
    enum class cull_mode
    {
        enabled,
        disabled
    };

    struct renderer_settings
    {
        render_mode rendering_mode = render_mode::filled_wireframe;
        cull_mode culling_mode = cull_mode::enabled;
    } render_settings;
}
