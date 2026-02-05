export module librenderer:raii;
import std;
import :lib.sdl;

export namespace raii
{
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
