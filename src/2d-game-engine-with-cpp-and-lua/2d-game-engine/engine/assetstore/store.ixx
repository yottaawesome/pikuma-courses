export module engine:assetstore.store;
import std;
import :sdl3;
import :log;

export namespace Engine
{
	class AssetStore
	{
	public:
		~AssetStore()
		{
			Clear();
		}

		void Clear()
		{
			for (auto& [_, texture] : textures)
				SDL::SDL_DestroyTexture(texture);
			textures.clear();
		}

		void AddTexture(
			SDL::SDL_Renderer* renderer, 
			const std::string& assetId, 
			const std::filesystem::path& path
		)
		{
			auto surface = SDL::IMG_Load(path.string().c_str());
			if (not surface)
				throw SDL::SdlError{ "Failed to load image from path: " + path.string() };
			auto texture = SDL::SDL_CreateTextureFromSurface(renderer, surface);
			SDL::SDL_DestroySurface(surface);
			if (not texture)
				throw SDL::SdlError{ "Failed to create texture from surface" };
			textures[assetId] = texture;
		}

		auto GetTexture(const std::string& assetId) -> SDL::SDL_Texture*
		{
			return textures[assetId];
		}

	private:
		std::map<std::string, SDL::SDL_Texture*> textures;
	};
}
