export module librenderer:sdl.texture;
import std;
import :sdl.exports;
import :sdl.raii;
import :sdl.error;

export namespace sdl
{
	class texture final
	{
	public:
		texture(
			SDL_Renderer* renderer,
			std::uint32_t format,
			int access,
			int width,
			int height
		) : sdlTexture{ create(renderer, format, access, width, height) }
		{
		}

		static auto create(
			SDL_Renderer* renderer,
			std::uint32_t format,
			int access,
			int width,
			int height
		) -> texture_unique_ptr
		{
			// Create a SDL texture
			// https://wiki.libsdl.org/SDL2/SDL_CreateTexture
			auto texture = texture_unique_ptr(
				SDL_CreateTexture(renderer, format, access, width, height)
			);
			if (not texture)
				throw std::runtime_error(sdl::print_last_error());
			return texture;
		}

		constexpr auto get(this const texture& self) noexcept -> SDL_Texture*
		{
			return self.sdlTexture.get();
		}

		constexpr auto update_from_buffer(
			this auto&& self,
			auto& buffer,
			const SDL_Rect* rect = nullptr
		) -> void
		{
			if (SDL_UpdateTexture(self.get(), rect, buffer.data(), buffer.pitch()) != 0)
				throw std::runtime_error(sdl::print_last_error());
		}
	private:
		sdl::texture_unique_ptr sdlTexture;
	};
}
