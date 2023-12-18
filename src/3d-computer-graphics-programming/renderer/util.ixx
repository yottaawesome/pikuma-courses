module;

#include <SDL2/SDL.h>

export module util;
import std;

export namespace util
{
    void print_last_error(const std::source_location location = std::source_location::current())
    {
        std::cerr << std::format(
            "SDL failed [{}] at {}:{}:{}\n",
            SDL_GetError(),
            location.file_name(),
            location.function_name(),
            location.line()
        );
    }

    struct sdl_window_deleter
    {
        void operator()(SDL_Window* window) { SDL_DestroyWindow(window); }
    };
    using sdl_window_unique_ptr = std::unique_ptr<SDL_Window, sdl_window_deleter>;

    struct sdl_renderer_deleter
    {
        void operator()(SDL_Renderer* renderer) { SDL_DestroyRenderer(renderer); }
    };
    using sdl_renderer_unique_ptr = std::unique_ptr<SDL_Renderer, sdl_renderer_deleter>;

}