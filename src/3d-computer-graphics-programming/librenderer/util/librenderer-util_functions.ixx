export module librenderer:util_functions;
import std;
import :shared;

export namespace util
{
    inline consteval auto is_debug() noexcept -> bool
    {
#ifdef _DEBUG
        return true;
#else
        return false;
#endif // _DEBUG
    }

    inline consteval auto is_release() noexcept -> bool
    {
        return not is_debug();
    }

    template<typename...TArgs>
    inline auto print_debug_string(std::format_string<TArgs...> fmt, TArgs&&...args) -> std::string
    {
        auto error = std::format("{}\n", std::format(fmt, std::forward<TArgs>(args)...));
        win32::OutputDebugStringA(error.c_str());
        return error;
    }

    auto print_last_error(const std::source_location location = std::source_location::current()) -> std::string
    {
        return print_debug_string(
            "SDL failed [{}] at {}:{}:{}\n",
            sdl::SDL_GetError(),
            location.file_name(),
            location.function_name(),
            location.line()
        );
    }

    template<auto VDeleteFn>
    struct deleter
    {
        static void operator()(auto resource)
        {
            VDeleteFn(resource);
        }
    };
    using sdl_window_unique_ptr = std::unique_ptr<sdl::SDL_Window, deleter<sdl::SDL_DestroyWindow>>;
    using sdl_renderer_unique_ptr = std::unique_ptr<sdl::SDL_Renderer, deleter<sdl::SDL_DestroyRenderer>>;
    using sdl_texture_unique_ptr = std::unique_ptr<sdl::SDL_Texture, deleter<sdl::SDL_DestroyTexture>>;

    struct sdl_context final
    {
        ~sdl_context()
        {
            sdl::SDL_Quit();
        }

        sdl_context(const sdl_context&) = delete;
        sdl_context& operator=(sdl_context&&) = delete;

        sdl_context(int flags) 
        {
            if (sdl::SDL_Init(sdl::sdl_init_everything) != 0)
                throw std::runtime_error("Failed initialisation of SDL2.");
        }
    };
}
