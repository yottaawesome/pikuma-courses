export module renderer:main_app;
import shared;
import util;
import :buffer_2d;

export
{
    struct main_app
    {
        main_app() = default;
        main_app(const main_app&) = delete;
        main_app& operator=(const main_app&) = delete;
        main_app(main_app&&) = default;
        main_app& operator=(main_app&&) = default;

        operator bool() const noexcept { return is_running; }

        bool is_running = false;
        uint32_t window_width = 0;
        uint32_t window_height = 0;

        // The SDL context for our application
        std::unique_ptr<util::sdl_context> context;
        // The main window
        util::sdl_window_unique_ptr window;
        // The main renderer that works with our window
        util::sdl_renderer_unique_ptr renderer;
        // Our color buffer, we copy this to the color_buffer_texture
        // texture and then pass the texture to the renderer
        color_buffer main_buffer{};
        // We copy the color buffer to this
        util::sdl_texture_unique_ptr color_buffer_texture;
    };
}
