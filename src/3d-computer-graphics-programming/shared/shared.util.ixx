export module shared.util;
import std;
import shared.sdl;

export namespace util
{
    consteval bool IsDebug() noexcept
    {
#ifdef _DEBUG
        return true;
#else
        return false;
#endif // _DEBUG
    }

    consteval bool IsRelease() noexcept
    {
        return not IsDebug();
    }

    void print_last_error(const std::source_location location = std::source_location::current())
    {
        std::cerr << std::format(
            "SDL failed [{}] at {}:{}:{}\n",
            sdl::SDL_GetError(),
            location.file_name(),
            location.function_name(),
            location.line()
        );
    }

    struct sdl_window_deleter
    {
        void operator()(sdl::SDL_Window* window) { sdl::SDL_DestroyWindow(window); }
    };
    using sdl_window_unique_ptr = std::unique_ptr<sdl::SDL_Window, sdl_window_deleter>;

    struct sdl_renderer_deleter
    {
        void operator()(sdl::SDL_Renderer* renderer) { sdl::SDL_DestroyRenderer(renderer); }
    };
    using sdl_renderer_unique_ptr = std::unique_ptr<sdl::SDL_Renderer, sdl_renderer_deleter>;
}