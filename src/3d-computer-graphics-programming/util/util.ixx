export module util;
import std;
import shared.sdl;
import shared.win32;

export namespace util
{
    inline consteval bool is_debug() noexcept
    {
#ifdef _DEBUG
        return true;
#else
        return false;
#endif // _DEBUG
    }

    inline consteval bool is_release() noexcept
    {
        return not is_debug();
    }

    void print_last_error(const std::source_location location = std::source_location::current())
    {
        std::string err = std::format(
            "SDL failed [{}] at {}:{}:{}\n",
            sdl::SDL_GetError(),
            location.file_name(),
            location.function_name(),
            location.line()
        );
        win32::OutputDebugStringA(err.data());
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

    struct sdl_texture_deleter
    {
        void operator()(sdl::SDL_Texture* p) 
        {
            sdl::SDL_DestroyTexture(p);
        }
    };
    using sdl_texture_unique_ptr = std::unique_ptr<sdl::SDL_Texture, sdl_texture_deleter>;

    class sdl_context final
    {
        public:
            ~sdl_context()
            {
                if (init_successful)
                    sdl::SDL_Quit();
            }

            sdl_context(const sdl_context&) = delete;
            sdl_context& operator=(sdl_context&&) = delete;
            sdl_context(sdl_context&& other) 
                : init_successful{ other.init_successful }
            {
                other.init_successful = false;
            }

            sdl_context(int flags) 
                : init_successful{ sdl::SDL_Init(sdl::sdl_init_everything) == 0 }
            {}

        public:
            operator bool() const noexcept
            {
                return init_successful;
            }

            bool successful() const noexcept
            {
                return init_successful;
            }

        private:
            bool init_successful = false;
    };
}